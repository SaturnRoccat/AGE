#pragma once
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <format>
#include <AgeAPI/internal/Error.hpp>

#ifndef TO_UNDERLYING
#define TO_UNDERLYING(x) static_cast<std::underlying_type_t<decltype(x)>>(x)
#endif

#ifndef RAPIDJSON_HAS_STDSTRING
#define RAPIDJSON_HAS_STDSTRING 1
#define RAPIDJSON_HAS_CXX11_RVALUE_REFS 1
#define RAPIDJSON_HAS_CXX11_NOEXCEPT 1
#define RAPIDJSON_HAS_CXX11_TYPETRAITS 1
#define RAPIDJSON_HAS_CXX11_RANGE_FOR 1
#define RAPIDJSON_HAS_CXX11_UNCAUGHT_EXCEPTION 1
#define RAPIDJSON_HAS_CXX11_USER_DEFINED_LITERALS 1
#define RAPIDJSON_HAS_CXX11_TEMPLATED_LITERALS 1
#define RAPIDJSON_HAS_CXX11_TYPENAME 1
#define RAPIDJSON_HAS_CXX11_EXPLICIT_CONVERSION 1
#endif
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/reader.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/error/en.h>
#include <rapidjson/stringbuffer.h>
#include <AgeAPI/internal/Error.hpp>
#include <format>
#include <optional>


namespace AgeAPI
{
    using u8 = unsigned char;
    using i8 = char;
    using u16 = unsigned short;
    using i16 = short;
    using u32 = unsigned int;
    using i32 = int;
    using u64 = unsigned long long;
    using i64 = long long;
    using f32 = float;
    using f64 = double;
    class Identifier
    {
    public:
        Identifier() = default;
        Identifier(const std::string& fullNamespace) : mFullNamespace(fullNamespace) { mColonPos = mFullNamespace.find(':'); }
        Identifier(const std::string& name, const std::string& namespace_) : mFullNamespace(namespace_ + ":" + name) { mColonPos = mFullNamespace.find(':'); }
        Identifier(const char* fullNamespace) : mFullNamespace(fullNamespace) { mColonPos = mFullNamespace.find(':'); }
        Identifier(const char* name, const char* namespace_) : mFullNamespace(std::format("{}:{}", name, namespace_)) { mColonPos = mFullNamespace.find(':'); }
        Identifier(const Identifier& other) : mFullNamespace(other.mFullNamespace), mColonPos(other.mColonPos) {}

        const std::string_view GetName() const { return mFullNamespace.substr(mColonPos + 1); }
        const std::string_view GetNamespace() const { return mFullNamespace.substr(0, mColonPos); }
        const std::string& GetFullNamespace() const { return mFullNamespace; }

        void SetName(const std::string& name) { mFullNamespace = std::format("{}:{}", GetNamespace(), name); }
        void SetNamespace(const std::string& namespace_) { mFullNamespace = std::format("{}:{}", namespace_, GetName()); }
        void SetFullNamespace(const std::string& fullNamespace)
		{
			mFullNamespace = fullNamespace;
			mColonPos = mFullNamespace.find(':');
		}

        bool operator==(const Identifier& other) const { return mFullNamespace == other.mFullNamespace; }
        bool operator!=(const Identifier& other) const { return mFullNamespace != other.mFullNamespace; }


    private:
        std::string mFullNamespace{};
        u32 mColonPos{0};
    };

    class SemanticVersion
    {
    public:
        SemanticVersion(u8 major, u8 minor, u8 patch) : mMajor(major), mMinor(minor), mPatch(patch) {}
        SemanticVersion(std::array<u8, 3> version) : mVersionArray(version) {}
        SemanticVersion(u32 version = 0) : mVersion(version) {}
        ~SemanticVersion() = default;
        u8 GetMajor() const { return mMajor; }
        u8 GetMinor() const { return mMinor; }
        u8 GetPatch() const { return mPatch; }
        u32 GetVersion() const { return mVersion; }
        std::vector<u8> GetVersionVector() const { return {mMajor, mMinor, mPatch}; }
        std::array<u8, 3> GetVersionArray() const { return mVersionArray; }
        std::string GetString() const { return std::format("{}.{}.{}", mMajor, mMinor, mPatch); }
        void SetMajor(u8 major) { mMajor = major; }
        void SetMinor(u8 minor) { mMinor = minor; }
        void SetPatch(u8 patch) { mPatch = patch; }

        bool operator==(const SemanticVersion& other) const { return mVersion == other.mVersion; }
        bool operator!=(const SemanticVersion& other) const { return mVersion != other.mVersion; }
        bool operator<(const SemanticVersion& other) const { return mVersion < other.mVersion; }
        bool operator>(const SemanticVersion& other) const { return mVersion > other.mVersion; }
        bool operator<=(const SemanticVersion& other) const { return mVersion <= other.mVersion; }
        bool operator>=(const SemanticVersion& other) const { return mVersion >= other.mVersion; }
    private:
        union 
        {
            struct
            {
                u8 mMajor;
                u8 mMinor;
                u8 mPatch;
            };
            std::array<u8, 3> mVersionArray;
            u32 mVersion{0};
        };
    };

    template<typename T>
    class NonOwningPtr
    {
    public:
		NonOwningPtr(T* ptr) : mPtr(ptr) {}
        NonOwningPtr(const NonOwningPtr<T>& other) : mPtr(other.mPtr) {}
        NonOwningPtr(NonOwningPtr<T>&& other) noexcept : mPtr(std::move(other.mPtr)) {}
        NonOwningPtr() = default;
		T* operator->() 
        { 
            #ifndef NDEBUG
            if (!mPtr)
				throw std::runtime_error("NonOwningPtr Destroyed While Still Being Required");
            #endif
            return mPtr;
        }
		T& operator*() 
        { 
            #ifndef NDEBUG
            if (!mPtr)
                throw std::runtime_error("NonOwningPtr Destroyed While Still Being Required");
            #endif
            return *mPtr;
        }
		T* Get() 
        {
            #ifndef NDEBUG
            if (!mPtr)
				throw std::runtime_error("NonOwningPtr Destroyed While Still Being Required");
            #endif

            return mPtr;
        }
        const T* Get() const 
        {
            #ifndef NDEBUG
            if (!mPtr)
                throw std::runtime_error("NonOwningPtr Destroyed While Still Being Required");
            #endif
            return mPtr;
        }
        operator T*() 
        {
            #ifndef NDEBUG
            if (!mPtr)
				throw std::runtime_error("NonOwningPtr Destroyed While Still Being Required");
            #endif
            return mPtr; 
        }

        void operator delete(void* ptr) = delete;
    private:
        T* mPtr{nullptr};


    };

    // CompactOption is the same size as the type it holds 
    template<typename T>
    class CompactOptional
    {
    private:
        union
        {
            T mValue;
            std::array<u8, sizeof(T)> mStorage = std::fill(mStorage.begin(), mStorage.end(), 0xAF);
        };
    public:

        CompactOptional() = default;
		CompactOptional(const T& value) : mValue(value) {}
		CompactOptional(T&& value) : mValue(std::move(value)) {}
		CompactOptional(const CompactOptional<T>& other) : mValue(other.mValue) {}
		CompactOptional(CompactOptional<T>&& other) : mValue(std::move(other.mValue)) {}

		T& operator*() { return mValue; }
		T* operator->() { return &mValue; }
		T& value() { return mValue; }
		const T& Value() const { return mValue; }

		bool HasValue() const { for (u8 byte : mStorage) if (byte != 0xAF) return true; return false; }
        void OwnValue(T& val) { val = std::move(mValue); }
		explicit operator bool() const { return HasValue(); }

		void Reset() { mStorage.fill(0xAF); }
		void Reset(const T& value) { mValue = value; }
		void Reset(T&& value) { mValue = std::move(value); }

		CompactOptional<T>& operator=(const T& value) { mValue = value; return *this; }
		CompactOptional<T>& operator=(T&& value) { mValue = std::move(value); return *this; }
		CompactOptional<T>& operator=(const CompactOptional<T>& other) { mValue = other.mValue; return *this; }
		CompactOptional<T>& operator=(CompactOptional<T>&& other) { mValue = std::move(other.mValue); return *this; }
    };


    enum class Experiments : u8
    {
        HolidayCreatorFeatures = 1 << 0,
        CustomBiomes = 1 << 1,
        UpcomingCreatorFeatures = 1 << 2,
        BetaAPI = 1 << 3,
        ExperimentalCameras = 1 << 4,
        ExperimentsALL = 0xFF
    };
    class ExperimentalSettings
    {
    public:
        ExperimentalSettings() = default;
		ExperimentalSettings(u8 flags) : mExperimentalFlags(flags) {}
        ExperimentalSettings(const std::array<Experiments, 5> flags)
		{
			for (Experiments flag : flags)
                mExperimentalFlags |= TO_UNDERLYING(flag);
        }
        ExperimentalSettings(const Experiments flags) : mExperimentalFlags(TO_UNDERLYING(flags)) {}

		bool IsFlagSet(u8 flag) const { return mExperimentalFlags & flag; }
		void SetFlag(u8 flag) { mExperimentalFlags |= flag; }
		void ClearFlag(u8 flag) { mExperimentalFlags &= ~flag; }
        u8 GetFlags() const { return mExperimentalFlags; }
    private:
        u8 mExperimentalFlags{ 0 };

    };

    struct JsonProxy
    {
        JsonProxy(rapidjson::Value& writeLoc, rapidjson::Document::AllocatorType& allocator) : mWriteLoc(writeLoc), mAllocator(allocator) {}
        rapidjson::Value& mWriteLoc;
        rapidjson::Document::AllocatorType& mAllocator;
    };
    struct DependencyProxy
    {
        rapidjson::Value& mDependencies;
        rapidjson::Document::AllocatorType& mAllocator;
    };
    struct ModuleProxy
    {
        rapidjson::Value& mModules;
        rapidjson::Document::AllocatorType& mAllocator;
    };
    struct MetadataProxy
    {
		rapidjson::Value& mMetadata;
		rapidjson::Document::AllocatorType& mAllocator;
	};
    struct PermutationProxy
	{
		rapidjson::Value& mPermutation;
		rapidjson::Document::AllocatorType& mAllocator;
	};

    class Addon; // Used all over the place


    static std::vector<std::string_view> ExplodeString(std::string_view string, std::string_view delim)
    {
        std::vector<std::string_view> result;
		size_t pos = 0;
		while ((pos = string.find(delim)) != std::string::npos)
		{
			result.push_back(string.substr(0, pos));
			string.remove_prefix(pos + delim.length());
		}
		result.push_back(string);
		return result;
    }



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
        Vec Lerp(const Vec& other, float t) const { return Vec(
            (T)x + (T)(((float)other.x - (float)x) * t),
            (T)y + (T)(((float)other.y - (float)y) * t));
        }
        T Length() const { return std::sqrt(x * x + y * y); }
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
    public:        Vec3T() : x(0), y(0), z(0) {}
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
        T Magnitude() const { return std::sqrt(x * x + y * y + z * z + w * w); }
        T MagnitudeSquared() const { return x * x + y * y + z * z + w * w; }
        Vec Abs() const { return Vec4T<T>(std::abs(x), std::abs(y), std::abs(z), std::abs(w)); }
        Vec Lerp(const Vec& other, float t) const {
            return Vec(
			(T)x + (T)(((float)other.x - (float)x) * t),
			(T)y + (T)(((float)other.y - (float)y) * t),
			(T)z + (T)(((float)other.z - (float)z) * t),
			(T)w + (T)(((float)other.w - (float)w) * t));
        }
        T Length() const { return std::sqrt(x * x + y * y + z * z + w * w); }

        


    };/*
    using Color = Vec4T<float>;*/
    class Color : public Vec4T<float>
    {
    public:
        using Vec = Vec4T<float>;
    public:
        Color() : Vec4T<float>(0, 0, 0, 0) {}
        Color(float r, float g, float b, float a) : Vec4T<float>(r, g, b, a) {}
        Color(const Color& other) : Vec4T<float>(other) {}
        Color(const Vec& other) : Vec4T<float>(other) {}
        Color(Vec&& other) noexcept : Vec4T<float>(std::move(other)) {}

        Color GrayScale() const
        {
            float redSmoosh = x * 0.3f;
            float greenSmoosh = y * 0.59f;
            float blueSmoosh = z * 0.11f;
            float gray = redSmoosh + greenSmoosh + blueSmoosh;
            return Color(gray, gray, gray, w);
        }

    };

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

    using IVec2 = Vec2T<i32>;

    template<typename T>
    using AutoFreeRaw = std::unique_ptr<T, decltype(&free)>;
}

namespace std
{
    
    template<>
    struct hash<AgeAPI::Identifier>
    {
        std::size_t operator()(const AgeAPI::Identifier& id) const
        {
            return std::hash<std::string>{}(id.GetFullNamespace());
        }
    };

    template<>
    struct hash<AgeAPI::SemanticVersion>
    {
        std::size_t operator()(const AgeAPI::SemanticVersion& version) const
        {
            return std::hash<AgeAPI::u32>{}(version.GetVersion());
        }
    };
}