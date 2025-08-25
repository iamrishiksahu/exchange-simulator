#include <cmath>

namespace exch_ns
{
class DoubleHelper
{
    constexpr static double PRECISION  = 1.0E-10;
    constexpr static double MULTIPLIER = 1000000000ULL;
    constexpr static double ROUND      = 5.0E-10;

  public:
    static bool IsEqual(double a, double b) noexcept
    {
        return std::fabs(a - b) < PRECISION;
    }

    static bool IsGreater(double a, double b) noexcept
    {
        return a > b;
    }

    static bool IsGreaterEqual(double a, double b) noexcept
    {
        return IsEqual(a, b) || IsGreater(a, b);
    }

    static bool IsLess(double a, double b) noexcept
    {
        return !IsEqual(a, b) && !IsGreater(a, b);
    }

    static bool IsLessEqual(double a, double b) noexcept
    {
        return !IsGreater(a, b);
    }

    static void RoundToTickUp(double &value, double tick_size)
    {
        double rounded_value = std::round(value / tick_size) * tick_size;

        if (IsGreater(value, rounded_value))
        {
            value = rounded_value += tick_size;
            return;
        }
        value = rounded_value;
    }

    static void RoundToTickDown(double &value, double tick_size)
    {
        double rounded_value = std::round(value / tick_size) * tick_size;

        if (IsLess(value, rounded_value))
        {
            value = rounded_value -= tick_size;
            return;
        }
        value = rounded_value;
    }

    /**
     * @brief Rounds to nearest tick
     * @param double value
     * @param double tick_sze
     */
    static void RoundToTick(double &value, double tick_size)
    {
        value = std::round(std::round((value / tick_size + ROUND) * tick_size) * MULTIPLIER) / MULTIPLIER;
    }
};
}; // namespace exch_ns