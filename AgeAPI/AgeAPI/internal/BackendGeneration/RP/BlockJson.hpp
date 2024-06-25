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

		inline void WriteToJson(JsonProxy json) const
		{
			rapidjson::Value TextureObject;

			// Never realised how useful std::visit is until now
			std::visit(overloaded{
				[&](const std::string& singleTextureEntry)
				{
					rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue("textures", singleTextureEntry, json.mWriteLoc, json.mAllocator);
				},
				[&](const std::vector<std::pair<std::string, std::string>>& multiTextureEntry)
				{
					TextureObject.SetObject();
					for (const auto& [key, value] : multiTextureEntry)
						rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue(key, value, TextureObject, json.mAllocator);
					json.mWriteLoc.AddMember("textures", TextureObject, json.mAllocator);
				}
				}, mEntry);
		}


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

		ErrorString AddEntry(const std::string& key, const std::string& value)
		{
			if (mEntries.find(key) != mEntries.end())
				return ErrorString("Key already exists");
			mEntries[key] = BlockJsonEntry(value);
			return "";
		}

		ErrorString AddEntry(const std::string& key, const std::vector<std::pair<std::string, std::string>>& value)
		{
			if (mEntries.find(key) != mEntries.end())
				return ErrorString("Key already exists");
			mEntries[key] = BlockJsonEntry(value);
			return "";
		}

		ErrorString AddEntry(const std::string& key, const BlockJsonEntry& value)
		{
			if (mEntries.find(key) != mEntries.end())
				return ErrorString("Key already exists");
			mEntries[key] = value;
			return "";
		}

		ErrorString WriteToJson(JsonProxy json) const
		{
			rapidjson::ValueWriteWithKey<std::array<u8, 3>>::WriteToJsonValue("format_version", mFormatVersion.GetVersionArray(), json.mWriteLoc, json.mAllocator);
			for (const auto& [key, value] : mEntries)
			{
				rapidjson::Value textureObject(rapidjson::kObjectType);
				value.WriteToJson({ textureObject, json.mAllocator });
				rapidjson::Value keyVal(key, json.mAllocator);
				json.mWriteLoc.AddMember(keyVal, textureObject, json.mAllocator);
			}
			return "";
		}
		std::expected<rapidjson::Document, ErrorString> BuildBlockJsonDocument() const
		{
			auto doc = rapidjson::Document{};
			doc.SetObject();
			auto& allocator = doc.GetAllocator();
			auto err = WriteToJson({ doc, allocator });
			if (err.ContainsError())
				return std::unexpected(err);
			return doc;
		}

		ErrorString WriteToFile(const std::filesystem::path& BasePath) const
		{
			auto path = BasePath / "blocks.json";
			auto doc = BuildBlockJsonDocument();
			if (!doc)
				return doc.error();
			std::ofstream file(path);
			if (!file.is_open())
				return ErrorString("Failed to open file");
			rapidjson::OStreamWrapper osw(file);
			rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
			doc->Accept(writer);
			osw.Flush();
			file.close();
		}


	};
}