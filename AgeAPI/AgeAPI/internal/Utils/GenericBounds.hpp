#pragma once
#include <AgeAPI/internal/Utils/Vec.hpp>

namespace AgeAPI
{
	template<typename T>
	struct GenericBounds
	{
		T min, max;
		GenericBounds() : min(0), max(0) {}
		GenericBounds(T min, T max) : min(min), max(max) {}
		GenericBounds(const GenericBounds<T>& other) : min(other.min), max(other.max) {}
		GenericBounds(GenericBounds<T>&& other) noexcept : min(std::move(other.min)), max(std::move(other.max)) {}
		GenericBounds<T>& operator=(const GenericBounds<T>& other) { min = other.min; max = other.max; return *this; }
		GenericBounds<T>& operator=(GenericBounds<T>&& other) noexcept { min = std::move(other.min); max = std::move(other.max); return *this; }

		bool operator==(const GenericBounds<T>& other) const { return min == other.min && max == other.max; }
		bool operator!=(const GenericBounds<T>& other) const { return min != other.min || max != other.max; }
		bool operator<(const GenericBounds<T>& other) const { return min < other.min && max < other.max; }
		bool operator>(const GenericBounds<T>& other) const { return min > other.min && max > other.max; }
		bool operator<=(const GenericBounds<T>& other) const { return min <= other.min && max <= other.max; }
		bool operator>=(const GenericBounds<T>& other) const { return min >= other.min && max >= other.max; }
	};

	using BlockBounds = GenericBounds<FVec3>;
}