#pragma once
#include <variant>

#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/TerrainTextures.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Block/BlockResource.hpp>
#include <fstream>
namespace AgeAPI::Backend::Rp
{
	struct BlockJsonStorageImpl
	{
		SmallVector<std::pair<TextureSide, std::string>, 1> mTextures{};

		std::string mSoundID{""};
		BlockJsonStorageImpl() = default;
		BlockJsonStorageImpl(const SmallVector<std::pair<TextureSide, std::string>, 1>& textureName, const std::string& soundID) : mTextures(textureName), mSoundID(soundID) {}
	};

	enum class BlockJsonError
	{
		NONE,
		ALREADY_EXISTS,
		NOT_FOUND
	};

	class BlockJson
	{
	private:
		SemanticVersion mFormatVersion{ 1, 1, 0 };
		std::unordered_map<Identifier, BlockJsonStorageImpl> mBlockJsonStorage;
		friend class ResourcePack;
	private:
		BlockJson() = default;

		ErrorString writeToFile(const std::filesystem::path& basePath) const;
	public:
		BlockJsonError AddBlock(const Identifier& blockID, const std::string& textureName = "", const std::string& soundID = "", bool override = false);
		BlockJsonError AddBlock(const Identifier& blockID, const BlockJsonStorageImpl& blockJsonStorage, bool override = false);
		BlockJsonError RemoveBlock(const Identifier& blockID);

		// WARNING: This function probably should not be used by the user
		void SetFormatVersion(const SemanticVersion& version) { mFormatVersion = version; } 
	};
}