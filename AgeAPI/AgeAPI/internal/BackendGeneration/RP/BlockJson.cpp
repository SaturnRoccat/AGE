#include "BlockJson.hpp"
namespace AgeAPI::Backend::Rp
{
	ErrorString BlockJson::writeToFile(const std::filesystem::path& basePath)
	{
		return "Not implemented yet";
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
