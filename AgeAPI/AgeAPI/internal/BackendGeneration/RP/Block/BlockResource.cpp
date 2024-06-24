#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Textures/Texture.hpp>

namespace AgeAPI::Backend::Rp
{
	class BlockResource
	{
	public:
	private:
		std::string mOutputPathFromBase;
		std::string mTexureShortName;
		Texture mBlockTexture;

	};
}