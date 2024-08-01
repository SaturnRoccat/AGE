#pragma once
#include <AgeAPI/internal/Utils/BasicTypeDecls.hpp>
#include <AgeAPI/internal/Error.hpp>
#include <array>
#include <cmath>

namespace AgeAPI
{
	template<typename T>
	class Vec2T
	{
	public:
		using Type = T;
		using Vec = Vec2T<T>;
		T x, y;
	public:
		Vec2T() : x(0), y(0) {}
		Vec2T(T x, T y) : x(x), y(y) {}
		Vec2T(const Vec2T<T>& other) : x(other.x), y(other.y) {}
		Vec2T(Vec2T<T>&& other) noexcept : x(std::move(other.x)), y(std::move(other.y)) {}

		Vec2T<T>& operator=(const Vec2T<T>& other) { x = other.x; y = other.y; return *this; }
		Vec2T<T>& operator=(Vec2T<T>&& other) noexcept { x = std::move(other.x); y = std::move(other.y); return *this; }

		Vec2T<T> operator+(const Vec2T<T>& other) const { return Vec2T<T>(x + other.x, y + other.y); }
		Vec2T<T> operator-(const Vec2T<T>& other) const { return Vec2T<T>(x - other.x, y - other.y); }
		Vec2T<T> operator*(const Vec2T<T>& other) const { return Vec2T<T>(x * other.x, y * other.y); }
		Vec2T<T> operator/(const Vec2T<T>& other) const { return Vec2T<T>(x / other.x, y / other.y); }

		Vec2T<T> operator+(T scalar) const { return Vec2T<T>(x + scalar, y + scalar); }
		Vec2T<T> operator-(T scalar) const { return Vec2T<T>(x - scalar, y - scalar); }
		Vec2T<T> operator*(T scalar) const { return Vec2T<T>(x * scalar, y * scalar); }
		Vec2T<T> operator/(T scalar) const { return Vec2T<T>(x / scalar, y / scalar); }

		Vec2T<T>& operator+=(const Vec2T<T>& other) { x += other.x; y += other.y; return *this; }
		Vec2T<T>& operator-=(const Vec2T<T>& other) { x -= other.x; y -= other.y; return *this; }
		Vec2T<T>& operator*=(const Vec2T<T>& other) { x *= other.x; y *= other.y; return *this; }
		Vec2T<T>& operator/=(const Vec2T<T>& other) { x /= other.x; y /= other.y; return *this; }

		Vec2T<T>& operator+=(T scalar) { x += scalar; y += scalar; return *this; }
		Vec2T<T>& operator-=(T scalar) { x -= scalar; y -= scalar; return *this; }
		Vec2T<T>& operator*=(T scalar) { x *= scalar; y *= scalar; return *this; }
		Vec2T<T>& operator/=(T scalar) { x /= scalar; y /= scalar; return *this; }

		bool operator==(const Vec2T<T>& other) const { return x == other.x && y == other.y; }
		bool operator!=(const Vec2T<T>& other) const { return x != other.x || y != other.y; }
		bool operator<(const Vec2T<T>& other) const { return x < other.x && y < other.y; }
		bool operator>(const Vec2T<T>& other) const { return x > other.x && y > other.y; }
		bool operator<=(const Vec2T<T>& other) const { return x <= other.x && y <= other.y; }
		bool operator>=(const Vec2T<T>& other) const { return x >= other.x && y >= other.y; }

		T Dot(const Vec2T<T>& other) const { return x * other.x + y * other.y; }
		T Cross(const Vec2T<T>& other) const { return x * other.y - y * other.x; }
		T Magnitude() const { return std::sqrt(x * x + y * y); }
		T MagnitudeSquared() const { return x * x + y * y; }
		Vec Abs() const { return Vec2T<T>(std::abs(x), std::abs(y)); }
		Vec Sign() const { return Vec2T<T>(x < 0 ? -1 : 1, y < 0 ? -1 : 1); }
		Vec Lerp(const Vec& other, float t) const {
			return Vec(
				(T)x + (T)(((float)other.x - (float)x) * t),
				(T)y + (T)(((float)other.y - (float)y) * t));
		}
		T Length() const { return std::sqrt(x * x + y * y); }

		std::array<T, 2> const ToArray() {return {x, y};}
	};

    

	template<typename T>
	concept IntegralVec2 = requires(T a)
	{
		std::is_integral_v<typename T::Type>;
		std::is_integral_v<decltype(a.x)>;
		std::is_integral_v<decltype(a.y)>;
		// Make sure has an abs function
		{ a.Abs() } -> std::same_as<T>;

	};

	template<typename T>
	class Vec3T
	{
	public:
		using Type = T;
		using Vec = Vec3T<T>;
		T x, y, z;
	public:        
		Vec3T() : x(0), y(0), z(0) {}
		Vec3T(T x) : x(x), y(x), z(x) {}
		Vec3T(T x, T y, T z) : x(x), y(y), z(z) {}
		Vec3T(const Vec3T<T>& other) : x(other.x), y(other.y), z(other.z) {}
		Vec3T(Vec3T<T>&& other) noexcept : x(std::move(other.x)), y(std::move(other.y)), z(std::move(other.z)) {}
		Vec3T<T>& operator=(const Vec3T<T>& other) { x = other.x; y = other.y; z = other.z; return *this; }
		Vec3T<T>& operator=(Vec3T<T>&& other) noexcept { x = std::move(other.x); y = std::move(other.y); z = std::move(other.z); return *this; }
		Vec3T<T> operator+(const Vec3T<T>& other) const { return Vec3T<T>(x + other.x, y + other.y, z + other.z); }
		Vec3T<T> operator-(const Vec3T<T>& other) const { return Vec3T<T>(x - other.x, y - other.y, z - other.z); }
		Vec3T<T> operator*(const Vec3T<T>& other) const { return Vec3T<T>(x * other.x, y * other.y, z * other.z); }
		Vec3T<T> operator/(const Vec3T<T>& other) const { return Vec3T<T>(x / other.x, y / other.y, z / other.z); }
		Vec3T<T> operator+(T scalar) const { return Vec3T<T>(x + scalar, y + scalar, z + scalar); }
		Vec3T<T> operator-(T scalar) const { return Vec3T<T>(x - scalar, y - scalar, z - scalar); }
		Vec3T<T> operator*(T scalar) const { return Vec3T<T>(x * scalar, y * scalar, z * scalar); }
		Vec3T<T> operator/(T scalar) const { return Vec3T<T>(x / scalar, y / scalar, z / scalar); }
		Vec3T<T>& operator+=(const Vec3T<T>& other) { x += other.x; y += other.y; z += other.z; return *this; }
		Vec3T<T>& operator-=(const Vec3T<T>& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
		Vec3T<T>& operator*=(const Vec3T<T>& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
		Vec3T<T>& operator/=(const Vec3T<T>& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }
		Vec3T<T>& operator+=(T scalar) { x += scalar; y += scalar; z += scalar; return *this; }
		Vec3T<T>& operator-=(T scalar) { x -= scalar; y -= scalar; z -= scalar; return *this; }
		Vec3T<T>& operator*=(T scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
		Vec3T<T>& operator/=(T scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }
		bool operator==(const Vec3T<T>& other) const { return x == other.x && y == other.y && z == other.z; }
		bool operator!=(const Vec3T<T>& other) const { return x != other.x || y != other.y || z != other.z; }
		bool operator<(const Vec3T<T>& other) const { return x < other.x && y < other.y && z < other.z; }
		bool operator>(const Vec3T<T>& other) const { return x > other.x && y > other.y && z > other.z; }
		bool operator<=(const Vec3T<T>& other) const { return x <= other.x && y <= other.y && z <= other.z; }
		bool operator>=(const Vec3T<T>& other) const { return x >= other.x && y >= other.y && z >= other.z; }
		T Dot(const Vec3T<T>& other) const { return x * other.x + y * other.y + z * other.z; }
		T Magnitude() const { return std::sqrt(x * x + y * y + z * z); }
		T MagnitudeSquared() const { return x * x + y * y + z * z; }
		Vec Abs() const { return Vec3T<T>(std::abs(x), std::abs(y), std::abs(z)); }
		Vec Cross(const Vec3T<T>& other) const { return Vec3T<T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x); }
		Vec Lerp(const Vec& other, float t) const {
			  return Vec(
				  (T)x + (T)(((float)other.x - (float)x) * t),
				  (T)y + (T)(((float)other.y - (float)y) * t),
				  (T)z + (T)(((float)other.z - (float)z) * t));
		  }

		std::array<T, 3> ToArray() const {return {x, y, z};}

	};

	template<typename T>
	class Vec4T
	{
	public:
		using Type = T;
		using Vec = Vec4T<T>;
		T x, y, z, w;
	public:
		Vec4T() : x(0), y(0), z(0), w(0) {}
		Vec4T(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
		Vec4T(const Vec&& other) noexcept : x(std::move(other.x)), y(std::move(other.y)), z(std::move(other.z)), w(std::move(other.w)) {}
		Vec4T(const Vec& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

		Vec& operator=(const Vec& other) { x = other.x; y = other.y; z = other.z; w = other.w; return *this; }
		Vec& operator=(Vec&& other) noexcept { x = std::move(other.x); y = std::move(other.y); z = std::move(other.z); w = std::move(other.w); return *this; }
		Vec operator+(const Vec& other) const { return Vec(x + other.x, y + other.y, z + other.z, w + other.w); }
		Vec operator-(const Vec& other) const { return Vec(x - other.x, y - other.y, z - other.z, w - other.w); }
		Vec operator*(const Vec& other) const { return Vec(x * other.x, y * other.y, z * other.z, w * other.w); }
		Vec operator/(const Vec& other) const { return Vec(x / other.x, y / other.y, z / other.z, w / other.w); }
		Vec operator+(T scalar) const { return Vec(x + scalar, y + scalar, z + scalar, w + scalar); }
		Vec operator-(T scalar) const { return Vec(x - scalar, y - scalar, z - scalar, w - scalar); }
		Vec operator*(T scalar) const { return Vec(x * scalar, y * scalar, z * scalar, w * scalar); }
		Vec operator/(T scalar) const { return Vec(x / scalar, y / scalar, z / scalar, w / scalar); }
		Vec& operator+=(const Vec& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
		Vec& operator-=(const Vec& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
		Vec& operator*=(const Vec& other) { x *= other.x; y *= other.y; z *= other.z; w *= other.w; return *this; }
		Vec& operator/=(const Vec& other) { x /= other.x; y /= other.y; z /= other.z; w /= other.w; return *this; }
		Vec& operator+=(T scalar) { x += scalar; y += scalar; z += scalar; w += scalar; return *this; }
		Vec& operator-=(T scalar) { x -= scalar; y -= scalar; z -= scalar; w -= scalar; return *this; }
		Vec& operator*=(T scalar) { x *= scalar; y *= scalar; z *= scalar; w *= scalar; return *this; }
		Vec& operator/=(T scalar) { x /= scalar; y /= scalar; z /= scalar; w /= scalar; return *this; }
		bool operator==(const Vec& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		bool operator!=(const Vec& other) const { return x != other.x || y != other.y || z != other.z || w != other.w; }
		bool operator<(const Vec& other) const { return x < other.x && y < other.y && z < other.z && w < other.w; }
		bool operator>(const Vec& other) const { return x > other.x && y > other.y && z > other.z && w > other.w; }

		T Dot(const Vec& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; }
		T Flatten() const { return x + y + z + w; }
		T Magnitude() const { return std::sqrt(x * x + y * y + z * z + w * w); }
		T MagnitudeSquared() const { return x * x + y * y + z * z + w * w; }
		Vec Abs() const { return Vec4T<T>(std::abs(x), std::abs(y), std::abs(z), std::abs(w)); }
		Vec Lerp(const Vec& other, const float t) const {
			// FIXME: Replace C-style casts with dynamic cast
			return Vec(
				(T)x + (T)(((float)other.x - (float)x) * t),
				(T)y + (T)(((float)other.y - (float)y) * t),
				(T)z + (T)(((float)other.z - (float)z) * t),
				(T)w + (T)(((float)other.w - (float)w) * t));
		}
		T Length() const { return std::sqrt(x * x + y * y + z * z + w * w); }

		std::array<T, 4> const ToArray() {return {x, y, z, w};}
	};

	using IVec2 = Vec2T<i32>;
	using I16Vec2 = Vec2T<i16>;
	using IVec3 = Vec3T<i32>;
	using IVec4 = Vec4T<i32>;
	using UVec2 = Vec2T<u32>;
	using UVec3 = Vec3T<u32>;
	using UVec4 = Vec4T<u32>;
	using FVec2 = Vec2T<float>;
	using FVec3 = Vec3T<float>;
	using FVec4 = Vec4T<float>;

	template<typename T>
	concept BasicVector2 = requires(T a)
	{
		typename T::Type;
		typename T::Vec;

		{ a.x };
		{ a.y };
	};

	template<typename T>
	concept BasicVector3 = requires(T a)
	{
		typename T::Type;
		typename T::Vec;

		{ a.x };
		{ a.y };
		{ a.z };
	};

	template<typename T>
	concept BasicVector4 = requires(T a)
	{
		typename T::Type;
		typename T::Vec;

		{ a.x };
		{ a.y };
		{ a.z };
		{ a.w };
	};


}

namespace rapidjson
{
	// the constant for Vec2 is 2, for Vec3 is 3, for Vec4 is 4

	template<typename T>
	struct InformationIndex<T, std::void_t<
		decltype(std::declval<T>().x),
		decltype(std::declval<T>().y)>> : std::integral_constant<int, 2> {};

	template<typename T>
	struct InformationIndex<T, std::void_t<
		decltype(std::declval<T>().x),
		decltype(std::declval<T>().y),
		decltype(std::declval<T>().z)>> : std::integral_constant<int, 3> {};

	template<typename T>
	struct InformationIndex<T, std::void_t<
		decltype(std::declval<T>().x),
		decltype(std::declval<T>().y),
		decltype(std::declval<T>().z),
		decltype(std::declval<T>().w)>> : std::integral_constant<int, 4> {};

	

	template<AgeAPI::BasicVector2 T>
	struct TypeTranslation<T, 2>
	{
		using Vec = T::Vec;
		static void WriteToJson(const Vec& vector, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			if (jsonValue.IsObject())
			{
				rapidjson::Value xValue;
				TypeTranslation<T::Type>::WriteToJson(vector.x, xValue, allocator);
				jsonValue.AddMember("x", xValue, allocator);
				rapidjson::Value yValue;
				TypeTranslation<T::Type>::WriteToJson(vector.y, yValue, allocator);
				jsonValue.AddMember("y", yValue, allocator);
			}
			else
			{
				jsonValue.SetArray();
				rapidjson::Value xValue;
				TypeTranslation<T::Type>::WriteToJson(vector.x, xValue, allocator);
				jsonValue.PushBack(xValue, allocator);
				rapidjson::Value yValue;
				TypeTranslation<T::Type>::WriteToJson(vector.y, yValue, allocator);
				jsonValue.PushBack(yValue, allocator);
			}
		}

		static T ReadFromJson(const rapidjson::Value& jsonValue)
		{
			if (jsonValue.IsObject())
			{
				if (!jsonValue.HasMember("x") || !jsonValue.HasMember("y"))
					throw std::runtime_error("Invalid JSON object for Vec2");
				return T(TypeTranslation<T::Type>::ReadFromJson(jsonValue["x"]), TypeTranslation<T::Type>::ReadFromJson(jsonValue["y"]));

			}
			else if (jsonValue.IsArray())
			{
				if (jsonValue.Size() != 2)
					throw std::runtime_error("Invalid JSON array for Vec2");
				return T(TypeTranslation<T::Type>::ReadFromJson(jsonValue[0]), TypeTranslation<T::Type>::ReadFromJson(jsonValue[1]));
			}
			else
				throw std::runtime_error("Invalid JSON value for Vec2");

		}
	};

	template<AgeAPI::BasicVector3 T>
	struct TypeTranslation<T, 3>
	{
		using Vec = T::Vec;
		static void WriteToJson(const T& vector, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			using u8 = decltype(T::x);
			auto vecx = vector.x;
			auto vecy = vector.y;
			auto vecz = vector.z;
			if (jsonValue.IsObject())
			{
				rapidjson::Value xValue;
				TypeTranslation<u8>::WriteToJson(vecx, xValue, allocator);
				jsonValue.AddMember("x", xValue, allocator);
				rapidjson::Value yValue;
				TypeTranslation<u8>::WriteToJson(vecy, yValue, allocator);
				jsonValue.AddMember("y", yValue, allocator);
				rapidjson::Value zValue;
				TypeTranslation<u8>::WriteToJson(vecz, zValue, allocator);
				jsonValue.AddMember("z", zValue, allocator);
			}
			else
			{
				jsonValue.SetArray();
				rapidjson::Value xValue;
				TypeTranslation<u8>::WriteToJson(vecx, xValue, allocator);
				jsonValue.PushBack(xValue, allocator);
				rapidjson::Value yValue;
				TypeTranslation<u8>::WriteToJson(vecy, yValue, allocator);
				jsonValue.PushBack(yValue, allocator);
				rapidjson::Value zValue;
				TypeTranslation<u8>::WriteToJson(vecz, zValue, allocator);
				jsonValue.PushBack(zValue, allocator);
			}
		}
		static T ReadFromJson(const rapidjson::Value& jsonValue)
		{
			if (jsonValue.IsObject())
			{
				if (!jsonValue.HasMember("x") || !jsonValue.HasMember("y") || !jsonValue.HasMember("z"))
					throw std::runtime_error("Invalid JSON object for Vec3");
				return T(TypeTranslation<T::Type>::ReadFromJson(jsonValue["x"]), TypeTranslation<T::Type>::ReadFromJson(jsonValue["y"]), TypeTranslation<T::Type>::ReadFromJson(jsonValue["z"]));
			}
			else if (jsonValue.IsArray())
			{
				if (jsonValue.Size() != 3)
					throw std::runtime_error("Invalid JSON array for Vec3");
				return T(TypeTranslation<T::Type>::ReadFromJson(jsonValue[0]), TypeTranslation<T::Type>::ReadFromJson(jsonValue[1]), TypeTranslation<T::Type>::ReadFromJson(jsonValue[2]));
			}
			else
				throw std::runtime_error("Invalid JSON value for Vec3");
		}
	};

	template<AgeAPI::BasicVector4 T>
	struct TypeTranslation<T, 4>
	{
		using Vec = T::Vec;
		static void WriteToJson(const Vec& vector, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			if (jsonValue.IsObject())
			{
				rapidjson::Value xValue;
				TypeTranslation<T::Type>::WriteToJson(vector.x, xValue, allocator);
				jsonValue.AddMember("x", xValue, allocator);
				rapidjson::Value yValue;
				TypeTranslation<T::Type>::WriteToJson(vector.y, yValue, allocator);
				jsonValue.AddMember("y", yValue, allocator);
				rapidjson::Value zValue;
				TypeTranslation<T::Type>::WriteToJson(vector.z, zValue, allocator);
				jsonValue.AddMember("z", zValue, allocator);
				rapidjson::Value wValue;
				TypeTranslation<T::Type>::WriteToJson(vector.w, wValue, allocator);
				jsonValue.AddMember("w", wValue, allocator);
			}
			else
			{
				jsonValue.SetArray();
				rapidjson::Value xValue;
				TypeTranslation<T::Type>::WriteToJson(vector.x, xValue, allocator);
				jsonValue.PushBack(xValue, allocator);
				rapidjson::Value yValue;
				TypeTranslation<T::Type>::WriteToJson(vector.y, yValue, allocator);
				jsonValue.PushBack(yValue, allocator);
				rapidjson::Value zValue;
				TypeTranslation<T::Type>::WriteToJson(vector.z, zValue, allocator);
				jsonValue.PushBack(zValue, allocator);
				rapidjson::Value wValue;
				TypeTranslation<T::Type>::WriteToJson(vector.w, wValue, allocator);
				jsonValue.PushBack(wValue, allocator);
			}
		}
		static T ReadFromJson(const rapidjson::Value& jsonValue)
		{
			if (jsonValue.IsObject())
			{
				if (!jsonValue.HasMember("x") || !jsonValue.HasMember("y") || !jsonValue.HasMember("z") || !jsonValue.HasMember("w"))
					throw std::runtime_error("Invalid JSON object for Vec4");
				return T(TypeTranslation<T::Type>::ReadFromJson(jsonValue["x"]), TypeTranslation<T::Type>::ReadFromJson(jsonValue["y"]), TypeTranslation<T::Type>::ReadFromJson(jsonValue["z"]), TypeTranslation<T::Type>::ReadFromJson(jsonValue["w"]));
			}
			else if (jsonValue.IsArray())
			{
				if (jsonValue.Size() != 4)
					throw std::runtime_error("Invalid JSON array for Vec4");
				return T(TypeTranslation<T::Type>::ReadFromJson(jsonValue[0]), TypeTranslation<T::Type>::ReadFromJson(jsonValue[1]), TypeTranslation<T::Type>::ReadFromJson(jsonValue[2]), TypeTranslation<T::Type>::ReadFromJson(jsonValue[3]));
			}
			else
				throw std::runtime_error("Invalid JSON value for Vec4");
		}
	};
 
}

namespace std
{
	template<AgeAPI::BasicVector2 T>
	struct std::hash<T> 
	{
		size_t operator()(const T& vec) const
		{
			size_t hash = std::hash<typename T::Type>()(vec.x);
			hash ^= std::hash<typename T::Type>()(vec.y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			return hash;
		}
	};
}