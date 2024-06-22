#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <typeinfo>
#include <iostream>
#include <functional>
#include <charconv>
namespace rapidjson
{

	template<typename T, typename = void>
	struct IsContainer : std::false_type {};

	template<typename T>
	struct IsContainer<T, std::void_t<
		decltype(std::declval<T>().size()),
		decltype(std::declval<T>().begin()),
		decltype(std::declval<T>().end()),
		typename T::value_type,
		typename T::size_type>> : std::true_type {};

	template<typename T, bool = IsContainer<T>::value>
	struct TypeTranslation
	{
		static void WriteToJson(const T& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			assert(false);
			static_assert(std::is_same<T, T>::value, "Type not supported");
		}

		static T ReadFromJson(const rapidjson::Value& jsonValue)
		{
			assert(false);
			static_assert(std::is_same<T, T>::value, "Type not supported");
		}
	};

	template<>
	struct TypeTranslation<bool, false>
	{
		static void WriteToJson(const bool& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.SetBool(value);
		}

		static bool ReadFromJson(const rapidjson::Value& jsonValue)
		{
			return jsonValue.GetBool();
		}
	};

	template<std::integral T>
	struct TypeTranslation<T, false>
	{
		static void WriteToJson(const T& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			if constexpr (std::is_signed_v<T>)
				jsonValue.SetInt64(static_cast<T>(value));
			else
				jsonValue.SetUint64(static_cast<T>(value));
		}

		static T ReadFromJson(const rapidjson::Value& jsonValue)
		{
			if constexpr (std::is_signed_v<T>)
				return static_cast<T>(jsonValue.GetInt64());
			else
				return static_cast<T>(jsonValue.GetUint64());
		}
	};

	template<>
	struct TypeTranslation<std::string, true> // So string technically matches out container specialization
	{
		static void WriteToJson(const std::string& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.SetString(value, allocator);
		}

		static std::string ReadFromJson(const rapidjson::Value& jsonValue)
		{
			return jsonValue.GetString();
		}
	};

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
	

	template<typename T>
	struct TypeTranslation<T, true>
	{
		static void WriteToJson(const T& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.SetArray();
			jsonValue.Reserve(value.size(), allocator);
			for (const auto& element : value)
			{
				rapidjson::Value elementValue;
				TypeTranslation<typename T::value_type>::WriteToJson(element, elementValue, allocator);
				jsonValue.PushBack(elementValue, allocator);
			}
		}

		static T ReadFromJson(const rapidjson::Value& jsonValue)
		{
			T result;
			for (const auto& element : jsonValue.GetArray())
				result.push_back(TypeTranslation<typename T::value_type>::ReadFromJson(element));
			return result;
		}
	};

	template<typename T>
	struct ValueWriter
	{
		static rapidjson::Value WriteToJsonValue(const T& value, rapidjson::Document::AllocatorType& allocator)
		{
			rapidjson::Value jsonValue;
			TypeTranslation<T>::WriteToJson(value, jsonValue, allocator);
			return jsonValue;
		}
	};

	template<typename T>
	struct ValueWriteWithKey
	{
		static void WriteToJsonValue(const std::string& key, const T& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			rapidjson::Value jv;
			TypeTranslation<T>::WriteToJson(value, jv, allocator);
			rapidjson::Value keyName;
			TypeTranslation<std::string>::WriteToJson(key, keyName, allocator);
			jsonValue.AddMember(keyName, jv, allocator);
		}

		static void WriteToJsonValue(const std::string& key, const T& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator, 
			std::function<void(const T&, rapidjson::Value&, rapidjson::Document::AllocatorType&)> customWriter)
		{
			rapidjson::Value jv;
			customWriter(value, jv, allocator);
			rapidjson::Value keyName;
			TypeTranslation<std::string>::WriteToJson(key, keyName, allocator);
			jsonValue.AddMember(keyName, jv, allocator);
		}
	};




	//class WrappedJsonObject
	//{
	//private:
	//	rapidjson::Value mVal;
	//	rapidjson::Document::AllocatorType& mAllocator;
	//public:
	//	WrappedJsonObject(rapidjson::Document::AllocatorType& allocator) : mAllocator(allocator)
	//	{
	//		mVal.SetObject();
	//	}

	//	template<typename T>
	//	void AddMember(const std::string& key, const T& value)
	//	{
	//		ValueWriteWithKey<T>::WriteToJsonValue(key, value, mVal, mAllocator);
	//	}

	//	template<typename T>
	//	void AddMember(const std::string& key, const T& value, std::function<void(const T&, rapidjson::Value&, rapidjson::Document::AllocatorType&)> customWriter)
	//	{
	//		ValueWriteWithKey<T>::WriteToJsonValue(key, value, mVal, mAllocator, customWriter);
	//	}

	//	template<typename T>
	//	std::optional<T> operator[](const std::string& key)
	//	{
	//		auto it = mVal.FindMember(key.c_str());
	//		if (it == mVal.MemberEnd())
	//			return std::nullopt;
	//		return TypeTranslation<T>::ReadFromJson(it->value);
	//	}

	//	rapidjson::Value& Get()
	//	{
	//		return mVal;
	//	}

	//	rapidjson::Document::AllocatorType& GetAllocator()
	//	{
	//		return mAllocator;
	//	}
	//};



}