#include <atomic>
#include <type_traits>
#include <utility>

template <typename T, std::size_t size> class SPSC_Queue
{
    static_assert((size & (size - 1)) == 0, "Size of the SPSC Queue must be power of 2.");

  public:
    SPSC_Queue() = default;

    template <typename U> bool Enqueue(U &&data) noexcept
    {
        const size_t next = Next(writer_.load(std::memory_order_relaxed));

        if (next == reader_.load(std::memory_order_acquire))
        {
            /* Queue full => drop*/
            return false;
        }

        ring_buffer_[writer_.load(std::memory_order_relaxed)] = std::forward<U>(data);
        writer_.store(next, std::memory_order_release);
        return true;
    }

    bool Dequeue(T &data) noexcept
    {
        const size_t curr_reader = reader_.load(std::memory_order_relaxed);

        if (curr_reader == writer_.load(std::memory_order_acquire))
        {
            /* Queue is empty */
            return false;
        }

        data = std::move(ring_buffer_[curr_reader]);
        reader_.store(Next(curr_reader), std::memory_order_release);
        return true;
    }

    bool Empty() noexcept
    {
        return reader_.load(std::memory_order_acquire) == writer_.load(std::memory_order_acquire);
    }

    bool Full() noexcept
    {
        if (Next(writer_.load(std::memory_order_relaxed)) == reader_.load(std::memory_order_acquire))
        {
            return true;
        }
        return false;
    }

  private:
    static constexpr size_t Next(const size_t pos) noexcept
    {
        return (pos + 1) & mask_;
    }

  private:
    T ring_buffer_[size];
    alignas(64) std::atomic<size_t> reader_{0};
    alignas(64) std::atomic<size_t> writer_{0};
    static constexpr size_t mask_ = size - 1;
};