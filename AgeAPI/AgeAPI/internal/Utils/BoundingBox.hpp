#pragma once
#include <AgeAPI/internal/Utils/BasicTypeDecls.hpp>
#include <AgeAPI/internal/Utils/Vec.hpp>

namespace AgeAPI
{
    
	template<typename T>
	class BoundingBox
	{
	public:
		T min, max;
	public:

		BoundingBox() : min(0), max(0) {}
		BoundingBox(T min, T max) : min(min), max(max) {}
		BoundingBox(const BoundingBox<T>& other) : min(other.min), max(other.max) {}
		BoundingBox(BoundingBox<T>&& other) noexcept : min(std::move(other.min)), max(std::move(other.max)) {}
		BoundingBox<T>& operator=(const BoundingBox<T>& other) { min = other.min; max = other.max; return *this; }
		BoundingBox<T>& operator=(BoundingBox<T>&& other) noexcept { min = std::move(other.min); max = std::move(other.max); return *this; }
		BoundingBox<T> operator+(const BoundingBox<T>& other) const { return BoundingBox<T>(min + other.min, max + other.max); }
		BoundingBox<T> operator-(const BoundingBox<T>& other) const { return BoundingBox<T>(min - other.min, max - other.max); }
		BoundingBox<T> operator*(const BoundingBox<T>& other) const { return BoundingBox<T>(min * other.min, max * other.max); }
		BoundingBox<T> operator/(const BoundingBox<T>& other) const { return BoundingBox<T>(min / other.min, max / other.max); }
		BoundingBox<T> operator+(T scalar) const { return BoundingBox<T>(min + scalar, max + scalar); }
		BoundingBox<T> operator-(T scalar) const { return BoundingBox<T>(min - scalar, max - scalar); }
		BoundingBox<T> operator*(T scalar) const { return BoundingBox<T>(min * scalar, max * scalar); }
		BoundingBox<T> operator/(T scalar) const { return BoundingBox<T>(min / scalar, max / scalar); }
		BoundingBox<T>& operator+=(const BoundingBox<T>& other) { min += other.min; max += other.max; return *this; }
		BoundingBox<T>& operator-=(const BoundingBox<T>& other) { min -= other.min; max -= other.max; return *this; }
		BoundingBox<T>& operator*=(const BoundingBox<T>& other) { min *= other.min; max *= other.max; return *this; }
		BoundingBox<T>& operator/=(const BoundingBox<T>& other) { min /= other.min; max /= other.max; return *this; }
		BoundingBox<T>& operator+=(T scalar) { min += scalar; max += scalar; return *this; }
		BoundingBox<T>& operator-=(T scalar) { min -= scalar; max -= scalar; return *this; }
		BoundingBox<T>& operator*=(T scalar) { min *= scalar; max *= scalar; return *this; }
		BoundingBox<T>& operator/=(T scalar) { min /= scalar; max /= scalar; return *this; }
		bool operator==(const BoundingBox<T>& other) const { return min == other.min && max == other.max; }
		bool operator!=(const BoundingBox<T>& other) const { return min != other.min || max != other.max; }

		bool Contains(const T& point) const {
			return point >= min && point <= max;
		}
	};

}