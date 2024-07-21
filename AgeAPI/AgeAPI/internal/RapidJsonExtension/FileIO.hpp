#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>


namespace rapidjson
{
	AgeAPI::ErrorString WriteJsonFile(const rapidjson::Value& value, const std::filesystem::path& path);


}