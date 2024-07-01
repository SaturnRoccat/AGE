#ifndef AGE_API_TYPES

#define AGE_API_TYPES
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>
#include <format>
#include <AgeAPI/internal/Error.hpp>
#include <AgeAPI/internal/Utils/BasicTypeDecls.hpp>
#include <AgeAPI/internal/Utils/SmallUtilities.hpp>
#include <AgeAPI/internal/Utils/SemanticVersion.hpp>
#include <AgeAPI/internal/Utils/Identifier.hpp>
#include <AgeAPI/internal/Utils/ExperimentalSettings.hpp>
#include <AgeAPI/internal/Utils/Vec.hpp>
#include <AgeAPI/internal/Utils/JsonProxy.hpp>
#include <AgeAPI/internal/Utils/Color.hpp>
#include <AgeAPI/internal/Utils/BoundingBox.hpp>
#include <AgeAPI/internal/Utils/ItemGroup.hpp>


#include <filesystem>
#include <uuid_v4.h>
#include <optional>
#include <charconv>
#include <concepts>

namespace AgeAPI
{
	class Addon; // Used all over the place

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

	class MenuCategory
	{
	private:
		bool mIsHiddenInCommands = false;
		ItemGroup mItemGroup = ItemGroup::itemGroup_name_none;
		std::string mCategory = "construction";
	public:
		MenuCategory() = default;
		MenuCategory(const std::string& category, ItemGroup itemGroup = ItemGroup::itemGroup_name_none, bool isHiddenInCommands = false) : mItemGroup(itemGroup), mCategory(category), mIsHiddenInCommands(isHiddenInCommands) {}
		void SetItemGroup(ItemGroup group) { mItemGroup = group; }
		void SetCategory(const std::string& category) { mCategory = category; }
		void SetHiddenInCommands(bool hidden) { mIsHiddenInCommands = hidden; }
		const auto& GetItemGroup() const { return mItemGroup; }
		const auto& GetCategory() const { return mCategory; }
		const auto& IsHiddenInCommands() const { return mIsHiddenInCommands; }
		void WriteToJson(JsonProxy proxy);
	};

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


	static std::string GetUUIDString()
	{
		thread_local static UUIDv4::UUIDGenerator<std::mt19937_64> generator;
		return generator.getUUID().str();

	}

	static const std::string& GetCurrentWorkingDirectory()
	{
		thread_local static std::string cwd = std::filesystem::current_path().string();
		return cwd;
	}


};

namespace rapidjson
{
	template<>
	struct TypeTranslation<AgeAPI::Identifier, false>
	{
		static void WriteToJson(const AgeAPI::Identifier& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.SetString(value.GetFullNamespace(), allocator);
		}

		static AgeAPI::Identifier ReadFromJson(const rapidjson::Value& jsonValue)
		{
			return AgeAPI::Identifier(jsonValue.GetString());
		}
	};

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

#endif // !AGE_API_TYPES_H