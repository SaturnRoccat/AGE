#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <typeinfo>
#include <iostream>

namespace rapidjson
{
	template<typename T>
	struct TypeTranslation
	{
		static void WriteToJson(const T& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			static_assert(std::is_same<T, T>::value, "Type not supported");
		}
	};

	template<>
	struct TypeTranslation<bool>
	{
		static void WriteToJson(const bool& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.SetBool(value);
		}
	};

	template<>
	struct TypeTranslation<int>
	{
		static void WriteToJson(const int& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.SetInt(value);
		}
	};

	template<>
	struct TypeTranslation<std::string>
	{
		static void WriteToJson(const std::string& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.SetString(value, allocator);
		}
	};

	template<>
	struct TypeTranslation<AgeAPI::Identifier>
	{
		static void WriteToJson(const AgeAPI::Identifier& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.SetString(value.GetFullNamespace(), allocator);
		}
	};

	template<>
	struct TypeTranslation<AgeAPI::SemanticVersion>
	{
		static void WriteToJson(const AgeAPI::SemanticVersion value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.SetString(std::format("{}.{}.{}", value.GetMajor(), value.GetMinor(), value.GetPatch()), allocator);
		}
	};
	template<typename T>
	concept IsContainer = requires(T t)
	{
		{ t.size() } -> std::convertible_to<std::size_t>;
		{ t.begin() } -> std::input_or_output_iterator;
		{ t.end() } -> std::input_or_output_iterator;
		{ T::value_type };
		{ T::size_type };
	};

	template<IsContainer T>
	struct TypeTranslation<T>
	{
		static void WriteToJson(const T& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.SetArray();
			for (const auto& element : value)
			{
				rapidjson::Value elementValue;
				TypeTranslation<typename T::value_type>::WriteToJson(element, elementValue, allocator);
				jsonValue.PushBack(elementValue, allocator);
			}
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
			TypeTranslation<T>::WriteToJson(value, jsonValue, allocator);
			rapidjson::Value keyName;
			TypeTranslation<std::string>::WriteToJson(key, keyName, allocator);
			jsonValue.AddMember(keyName, jsonValue, allocator);
		}
	};


}