#include "BlockResource.hpp"
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>

namespace AgeAPI::Backend::Rp
{
	void BlockResourceElement::Write(const std::filesystem::path& base) const
	{
		std::filesystem::path outputPath = base / mOutputPathFromBase;
		std::filesystem::create_directories(outputPath.parent_path());
		if (!outputPath.has_extension())
			outputPath.replace_extension(".png");
		mBlockTexture.FinalizeAndWrite(outputPath.string());
	}

	ErrorString BlockResource::WriteToJson(JsonProxy proxy) const
	{
		rapidjson::Value TextureObject(rapidjson::kObjectType);
		if (this->mTextures.size() == 1)
			rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue("textures", this->mTextures[0].GetOutputPathFromBase(), TextureObject, proxy.mAllocator);
		else
			rapidjson::ValueWriteWithKey<std::vector<BlockResourceElement>>::WriteToJsonValue("textures", this->mTextures, TextureObject, proxy.mAllocator);
		rapidjson::Value key(this->mTextureShortName, proxy.mAllocator);
		proxy.mWriteLoc.AddMember(key, TextureObject, proxy.mAllocator);
		return ErrorString();
	}
}
