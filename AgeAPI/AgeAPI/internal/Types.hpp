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
#include <format>

#ifndef THROW_STRING
#define THROW_STRING(x)\
{\
	err = ErrorString(std::format("Error on line {}, File {}, Error {}", __LINE__, __FILE__, x));\
	return;\
}
#endif

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
        Identifier(const std::string& name, const std::string& ns) : mName(name), mNamespace(ns) {}
        Identifier(const std::string& fullNamespace, ConstructionError& err) {
            auto pos = fullNamespace.find(':');
            if (pos != std::string::npos)
            {
				mNamespace = fullNamespace.substr(0, pos);
				mName = fullNamespace.substr(pos + 1);
                return;
			}
            THROW_STRING(fullNamespace);
        }

        std::string GetNamespace() const { return mNamespace; }
        std::string GetName() const { return mName; }
        std::string GetFullNamespace() const { return std::format("{}:{}", mNamespace, mName); }

        void SetNamespace(const std::string& ns) { mNamespace = ns; }
        void SetName(const std::string& name) { mName = name; }

        bool operator==(const Identifier& other) const{return mNamespace == other.mNamespace && mName == other.mName;}
        bool operator!=(const Identifier& other) const{return !(*this == other);}
    private:
        std::string mNamespace{"age"};
        std::string mName{"identifier"};
    };

    class SemanticVersion
    {
    public:
        SemanticVersion() = default;
        SemanticVersion(u8 major, u8 minor, u8 patch) : mMajor(major), mMinor(minor), mPatch(patch) {}
        SemanticVersion(u32 version) : mVersion(version) {}

        u8 GetMajor() const { return mMajor; }
        u8 GetMinor() const { return mMinor; }
        u8 GetPatch() const { return mPatch; }
        u32 GetVersion() const { return mVersion; }
        std::vector<u8> GetVersionVector() const { return {mMajor, mMinor, mPatch}; }
        std::array<u8, 3> GetVersionArray() const { return mVersionArray; }

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
                u8 mPatch;
                u8 mMinor;
                u8 mMajor;
            };
            std::array<u8, 3> mVersionArray;
            u32 mVersion{0};
        };
    };

    enum class Experiments : u8
    {
        HolidayCreatorFeatures = 1 << 0,
        CustomBiomes = 1 << 1,
        UpcomingCreatorFeatures = 1 << 2,
        BetaAPI = 1 << 3,
        ExperimentalCameras = 1 << 4,
        ALL = 0xFF
    };

    class ExperimentalSettings
    {
    public:
        ExperimentalSettings() = default;
		ExperimentalSettings(u8 flags) : mExperimentalFlags(flags) {}

		bool IsFlagSet(u8 flag) const { return mExperimentalFlags & flag; }
		void SetFlag(u8 flag) { mExperimentalFlags |= flag; }
		void ClearFlag(u8 flag) { mExperimentalFlags &= ~flag; }
        u8 GetFlags() const { return mExperimentalFlags; }
    private:
        u8 mExperimentalFlags{ 0 };

    };

    struct BlockJsonProxy
    {
        rapidjson::Document& mDoc;
        rapidjson::Value& mDescription;
        rapidjson::Value& mMenuCategory;
        rapidjson::Value& mComponents;
        rapidjson::Value& mStates;
        rapidjson::Value& mPermutations;
        rapidjson::Value& mTraits;
        rapidjson::Document::AllocatorType& mAllocator;
    };
    struct DependencyProxy
    {
        rapidjson::Document& mDoc;
        rapidjson::Value& mDependencies;
        rapidjson::Document::AllocatorType& mAllocator;
    };
    struct ModuleProxy
    {
        rapidjson::Document& mDoc;
        rapidjson::Value& mModules;
        rapidjson::Document::AllocatorType& mAllocator;
    };
    struct MetadataProxy
    {
		rapidjson::Document& mDoc;
		rapidjson::Value& mMetadata;
		rapidjson::Document::AllocatorType& mAllocator;
	};
    class Addon; // Used all over the place

}

namespace std
{
    std::string to_string(const AgeAPI::Identifier& id) { return id.GetFullNamespace(); }
    std::string to_string(const AgeAPI::SemanticVersion& version) { return std::format("{}.{}.{}", version.GetMajor(), version.GetMinor(), version.GetPatch()); }

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