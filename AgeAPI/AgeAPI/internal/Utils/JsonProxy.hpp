#pragma once
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>
#include <tuple>

namespace AgeAPI
{
	struct JsonProxy
	{
		JsonProxy(rapidjson::Value& writeLoc, rapidjson::Document::AllocatorType& allocator) : mWriteLoc(writeLoc), mAllocator(allocator) {}
		rapidjson::Value& mWriteLoc;
		rapidjson::Document::AllocatorType& mAllocator;
		JsonProxy Derive(rapidjson::Value& value) const { return JsonProxy(value, mAllocator); }
		operator std::pair<rapidjson::Value&, rapidjson::Document::AllocatorType&>() { return { mWriteLoc, mAllocator }; }
	};
}