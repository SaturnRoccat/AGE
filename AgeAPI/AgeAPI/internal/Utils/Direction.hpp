#pragma once
#include <string>

namespace AgeAPI
{
    enum class SideDirections : unsigned char
    {
        North,
        East,
        South,
        West,
    };

    enum class AllDirections : unsigned char
    {
        North,
        East,
        South,
        West,
        Up,
        Down,
    };

    std::string GetDirectionAsString(AllDirections dir);
    static inline std::string GetDirectionAsString(SideDirections dir) 
    {
        return GetDirectionAsString(static_cast<AllDirections>(dir));
    }
}