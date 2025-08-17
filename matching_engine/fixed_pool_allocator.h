#pragma once
#include <type_traits>

namespace exch_ns
{
template <typename T> class FixedPoolAllocator
{
    static_assert(std::is_trivially_destructible<T>::value, "FixedPoolAllocator expects trivially destructible type.");

  public:
    FixedPoolAllocator(size_t capacity) : capacity_{capacity}
    {
        /* Allocating a block of memory to reserve int the memory pool */
        const size_t bytes_to_reserve = capacity_ * sizeof(T);
        memory_pool_                  = static_cast<unsigned char *>(new (bytes_to_reserve, std::align_val_t(alignof(T))));

        /* Creating a linked list to track the free blocks of memory */

        head_ = nullptr;
        for (size_t i = 0; i < capacity_; i++)
        {
            size_t offset = memory_pool_ + i * sizeof(T);
            Node  *node   = reinterpret_cast<Node *>(offset);
            node->next    = head_;
            head_         = node;
        }
    }

    ~FixedPoolAllocator()
    {
        delete (memory_pool_, std::align_val_t(alignof(T)));
        memory_pool_ = nullptr;
        head_        = nullptr;
        used_        = 0;
        capacity_    = 0;
    }

    T *Allocate() noexcept
    {
        if (head_ == nullptr)
        {
            /* Log: Out of memory*/
            return nullptr;
        }

        Node *allocation_segment = head_;
        head_                    = head_->next;
        used_++;

        return reinterpret_cast<T *>(allocation_segment);
    }

    void Deallocate(T *ptr) noexcept
    {
        if (ptr == nullptr)
            return;

        Node *node = reinterpret_cast<Node *>(ptr);

        node->next = head_;
        head_      = node;
        used_--;
    }

    size_t Capacity() const noexcept
    {
        return capacity_;
    }

    size_t Used() const noexcept
    {
        return used_;
    }

    size_t Available() const noexcept
    {
        return capacity_ - used_;
    }

    /**
     * @brief Returns true if the resource is owned by this pool
     * @param ptr Pointer to resource
     */
    bool Owns(T *ptr) const noexcept
    {
        auto base = reinterpret_cast<std::uintptr_t>(memory_pool_);
        auto addr = reinterpret_cast<std::uintptr_t>(ptr);
        auto end  = base + capacity_ * sizeof(T);
        return (addr >= base) && (addr < end) && (((addr - base) % sizeof(T)) == 0);
    }

  private:
    struct Node
    {
        Node *next
    };

  private:
    size_t         capacity_;
    unsigned char *memory_pool_;
    Node          *head_;
    size_t         used_{0};
};
} // namespace exch_ns