#include <AgeAPI/internal/BackendGeneration/RP/Textures/TextureRegistry.hpp>

namespace AgeAPI::Backend::Rp
{
	const ErrorString& GetTextureErrorString(TextureError error)
	{
		static const ErrorString TEXTURE_ALREADY_EXISTS{ "Texture already exists" };
		static const ErrorString TEXTURE_DOES_NOT_EXIST{ "Texture does not exist" };
		static const ErrorString NONE{ "" };

		switch (error)
		{
		case TextureError::TEXTURE_ALREADY_EXISTS:
			return TEXTURE_ALREADY_EXISTS;
		case TextureError::TEXTURE_DOES_NOT_EXIST:
			return TEXTURE_DOES_NOT_EXIST;
		default:
			return NONE;
		}
	}
}