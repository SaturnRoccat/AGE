#pragma once
#include <typeinfo>
#include <iostream>
#include <functional>
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
#include <charconv>
#include <string>
namespace rapidjson
{

	template<typename T, typename = void>
	struct InformationIndex :  std::integral_constant<int, 0> {};

	template<typename T>
	struct InformationIndex<T, std::void_t<
		decltype(std::declval<T>().size()),
		decltype(std::declval<T>().begin()),
		decltype(std::declval<T>().end()),
		typename T::value_type,
		typename T::size_type>> : std::integral_constant<int, 1> {};

	template<typename T, int = InformationIndex<T>::value>
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
	struct TypeTranslation<Value, InformationIndex<Value>::value>
	{
		static void WriteToJson(const Value& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.CopyFrom(value, allocator);
		}

		static Value ReadFromJson(const rapidjson::Value& jsonValue)
		{
			throw std::runtime_error("Cannot read Value from json");
			return {};
		}
	};

	template<>
	struct TypeTranslation<bool, 0>
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
	struct TypeTranslation<T, 0>
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

	template<std::floating_point T>
	struct TypeTranslation<T, 0>
	{
		static void WriteToJson(const T& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.SetDouble(value);
		}

		static T ReadFromJson(const rapidjson::Value& jsonValue)
		{
			return static_cast<T>(jsonValue.GetDouble());
		}
	};

	template<>
	struct TypeTranslation<std::string, 1> // So string technically matches out container specialization
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
	

	template<typename T>
	struct TypeTranslation<T, 1>
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