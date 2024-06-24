#pragma once
#include <AgeAPI/internal/Types.hpp>

namespace AgeAPI
{
    template<std::floating_point T>
    T Lerp(T a, T b, T t)
    {
        return a + (b - a) * t;
    }

    template<std::integral T>
    T Lerp(T a, T b, float t)
    {
        return a + (b - a) * t;
    }
}