#include "BlockJson.hpp"
#include <AgeAPI/internal/RapidJsonExtension/FileIO.hpp>
#include <list>
namespace AgeAPI::Backend::Rp
{
	ErrorString BlockJson::writeToFile(const std::filesystem::path& basePath) const 
	{

		auto ToString = [](TextureSide side) -> std::string
			{
				switch (side)
				{
				case TextureSide::TOP:
					return "up";
				case TextureSide::BOTTOM:
					return "down";
				case TextureSide::FRONT:
					return "north";
				case TextureSide::RIGHT:
					return "east";
				case TextureSide::BACK:
					return "south";
				case TextureSide::LEFT:
					return "west";
				default:
					return "";
				}
			};
		rapidjson::Document document;
		document.SetObject();
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
		rapidjson::ValueWriteWithKey<std::array<u8, 3>>::WriteToJsonValue("format_version", mFormatVersion.GetVersionArray(), document, allocator);
		for (auto& [id, texture] : mBlockJsonStorage)
		{
			rapidjson::Value blockTexture(rapidjson::kObjectType);
			if (texture.mTextures.IsSmall())
			{
				blockTexture.AddMember("textures", texture.mTextures[0].second, allocator);

				rapidjson::Value Key(id.GetFullNamespace(), allocator);
				document.AddMember(Key, blockTexture, allocator);
				continue;
			}
			std::list<TextureSide> RemainingSides = {
				TextureSide::TOP,
				TextureSide::BOTTOM,
				TextureSide::FRONT,
				TextureSide::RIGHT,
				TextureSide::BACK,
				TextureSide::LEFT
			};
			std::string allTextureSide{};
			rapidjson::Value InternalTextures(rapidjson::kObjectType);
			for (auto& [side, textureName] : texture.mTextures)
			{
				if (side == TextureSide::ALL)
				{
					allTextureSide = textureName;
					continue;
				}
				RemainingSides.remove(side);
				rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue(ToString(side), textureName, InternalTextures, allocator);
			}
			if (RemainingSides.empty())
			{
				rapidjson::Value Key(id.GetFullNamespace(), allocator);
				blockTexture.AddMember("textures", InternalTextures, allocator);
				document.AddMember(Key, blockTexture, allocator);
				continue;
			}
			for (auto& side : RemainingSides)
			{
				rapidjson::Value Key(ToString(side), allocator);
				InternalTextures.AddMember(Key, allTextureSide, allocator);
			}
			rapidjson::Value Key(id.GetFullNamespace(), allocator);
			blockTexture.AddMember("textures", InternalTextures, allocator);
			document.AddMember(Key, blockTexture, allocator);
		}
		return rapidjson::WriteJsonFile(document, basePath / "blocks.json");
	}
	BlockJsonError BlockJson::AddBlock(const Identifier& blockID, const std::string& textureName, const std::string& soundID, bool override)
	{
		if (!override && mBlockJsonStorage.find(blockID) != mBlockJsonStorage.end())
			return BlockJsonError::ALREADY_EXISTS;

		mBlockJsonStorage[blockID] = BlockJsonStorageImpl({ {TextureSide::ALL, textureName} }, soundID);
		return BlockJsonError::NONE;
	}
	BlockJsonError BlockJson::AddBlock(const Identifier& blockID, const BlockJsonStorageImpl& blockJsonStorage, bool override)
	{
		if (!override && mBlockJsonStorage.find(blockID) != mBlockJsonStorage.end())
			return BlockJsonError::ALREADY_EXISTS;

		mBlockJsonStorage[blockID] = blockJsonStorage;
		return BlockJsonError::NONE;
	}
	BlockJsonError BlockJson::RemoveBlock(const Identifier& blockID)
	{
		if (mBlockJsonStorage.find(blockID) == mBlockJsonStorage.end())
			return BlockJsonError::NOT_FOUND;

		mBlockJsonStorage.erase(blockID);
		return BlockJsonError::NONE;
	}
}
