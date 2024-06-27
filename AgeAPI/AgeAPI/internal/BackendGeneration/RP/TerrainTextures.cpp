#include "TerrainTextures.hpp"
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>



namespace AgeAPI::Backend::Rp
{
	ErrorString TerrainTexture::WriteToJson(JsonProxy proxy) const
	{
		rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue("resource_pack_name", mResourcePackName, proxy.mWriteLoc, proxy.mAllocator);
		rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue("texture_name", mAtlasName, proxy.mWriteLoc, proxy.mAllocator);
		rapidjson::ValueWriteWithKey<u8>::WriteToJsonValue("num_mip_levels", mMipMapLevels, proxy.mWriteLoc, proxy.mAllocator);
		rapidjson::ValueWriteWithKey<u8>::WriteToJsonValue("padding", mPadding, proxy.mWriteLoc, proxy.mAllocator);
		rapidjson::Value textureData(rapidjson::kObjectType);
		JsonProxy textureDataProxy{ textureData, proxy.mAllocator };
		for (auto& [name, texture] : mTextureData)
			texture.WriteToJson(textureDataProxy);
		proxy.mWriteLoc.AddMember("texture_data", textureData, proxy.mAllocator);
		return ErrorString();
	}
}