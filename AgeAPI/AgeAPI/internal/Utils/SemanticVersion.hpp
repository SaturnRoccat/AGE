#pragma once
#include <AgeAPI/internal/Utils/BasicTypeDecls.hpp>
#include <array>
#include <vector>
#include <format>

namespace AgeAPI
{
	static std::vector<std::string_view> ExplodeString(std::string_view str, std::string_view delimiter)
	{
		std::vector<std::string_view> result;
		size_t pos = 0;
		size_t nextPos = 0;
		while ((nextPos = str.find(delimiter, pos)) != std::string::npos)
		{
			result.push_back(str.substr(pos, nextPos - pos));
			pos = nextPos + delimiter.size();
		}
		result.push_back(str.substr(pos));
		return result;
	}
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
		std::vector<u8> GetVersionVector() const { return { mMajor, mMinor, mPatch }; }
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
			u32 mVersion{ 0 };
		};
	};

}


namespace rapidjson
{


	template<>
	struct TypeTranslation<AgeAPI::SemanticVersion, false>
	{
		static void WriteToJson(const AgeAPI::SemanticVersion value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.SetString(std::format("{}.{}.{}", value.GetMajor(), value.GetMinor(), value.GetPatch()), allocator);
		}

		static AgeAPI::SemanticVersion ReadFromJson(const rapidjson::Value& jsonValue)
		{
			using namespace AgeAPI;
			if (jsonValue.IsArray())
			{
				auto arr = jsonValue.GetArray();
				std::array<u8, 3> version;
				for (int i = 0; i < arr.Capacity(); i++)
				{
					auto& arrVal = arr[i];
					if (!arrVal.IsInt()) [[unlikely]]
						throw std::runtime_error("Value Is Not In The expected Format");

						u8 section = (u8)arrVal.GetInt();
						version[i] = section;
				}
			}
			else if (jsonValue.IsString())
			{
				auto str = jsonValue.GetString();
				auto parts = AgeAPI::ExplodeString(str, ".");
				if (parts.size() != 3) [[unlikely]]
					throw std::runtime_error("Value Is Not In The expected Format");

					std::array<u8, 3> version;
					for (int i = 0; i < 3; i++)
						if (auto fromChar = std::from_chars(parts[i].data(), parts[i].data() + parts[i].size(), version[i]); fromChar.ec != std::errc())
							throw std::runtime_error("Value Is Not In The expected Format");
			}
			else [[unlikely]]
				throw std::runtime_error("Value Is Not In The expected Format");
		}
	};

}


namespace std
{


	template<>
	struct hash<AgeAPI::SemanticVersion>
	{
		std::size_t operator()(const AgeAPI::SemanticVersion& version) const
		{
			return std::hash<AgeAPI::u32>{}(version.GetVersion());
		}
	};
}