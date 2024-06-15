#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <typeinfo>
#include <iostream>
#include <functional>

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
	};

	template<>
	struct TypeTranslation<bool, false>
	{
		static void WriteToJson(const bool& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.SetBool(value);
		}
	};

	template<std::integral T>
	struct TypeTranslation<T, false>
	{
		static void WriteToJson(const T& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			if constexpr (std::is_signed_v<T>)
				jsonValue.SetInt64(value);
			else
				jsonValue.SetUint64(value);
		}
	};

	template<>
	struct TypeTranslation<std::string, true> // So string technically matches out container specialization
	{
		static void WriteToJson(const std::string& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.SetString(value, allocator);
		}
	};

	template<>
	struct TypeTranslation<AgeAPI::Identifier, false>
	{
		static void WriteToJson(const AgeAPI::Identifier& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.SetString(value.GetFullNamespace(), allocator);
		}
	};

	template<>
	struct TypeTranslation<AgeAPI::SemanticVersion, false>
	{
		static void WriteToJson(const AgeAPI::SemanticVersion value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.SetString(std::format("{}.{}.{}", value.GetMajor(), value.GetMinor(), value.GetPatch()), allocator);
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


}