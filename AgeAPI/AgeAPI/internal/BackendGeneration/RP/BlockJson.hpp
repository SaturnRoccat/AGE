#pragma once
#include <variant>

#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>
#include <fstream>
namespace AgeAPI::Backend::Rp
{
	class BlockJsonEntry
	{
	public:
		BlockJsonEntry() = default;
		BlockJsonEntry(const std::string& singleTextureEntry) : mEntry(singleTextureEntry) {}
		BlockJsonEntry(const std::vector<std::pair<std::string, std::string>>& multiTextureEntry) : mEntry(multiTextureEntry) {}

		inline void WriteToJson(JsonProxy json) const;


	private:
		std::variant<std::string, std::vector<std::pair<std::string, std::string>>> mEntry;

	};

	class BlockJson
	{
	private:
		SemanticVersion mFormatVersion{ 1, 1, 0 };
		std::unordered_map<std::string, BlockJsonEntry> mEntries{};
	public:
		BlockJson() = default;

		ErrorString AddEntry(const std::string& key, const std::string& value);

		ErrorString AddEntry(const std::string& key, const std::vector<std::pair<std::string, std::string>>& value);

		ErrorString AddEntry(const std::string& key, const BlockJsonEntry& value);

		ErrorString WriteToJson(JsonProxy json) const;
		std::expected<rapidjson::Document, ErrorString> BuildBlockJsonDocument() const;

		ErrorString WriteToFile(const std::filesystem::path& BasePath) const;


	};
}