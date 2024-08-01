#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Textures/Texture.hpp>
#include <filesystem>

namespace AgeAPI::Backend::Rp
{
	
	enum class TextureError
	{
		NONE,
		TEXTURE_ALREADY_EXISTS,
		TEXTURE_DOES_NOT_EXIST
	};

	const ErrorString& GetTextureErrorString(TextureError error);

	template<typename T>
	class TextureRegistry
	{
	private:
		absl::flat_hash_map<std::string, T> mTextures{};
	public:
		TextureRegistry() = default; 
		TextureRegistry(const TextureRegistry& other) = delete;	
		TextureRegistry(TextureRegistry&& other) noexcept
			: mTextures(std::move(other.mTextures)) {}
		TextureRegistry& operator=(const TextureRegistry& other) = delete;
		TextureRegistry& operator=(TextureRegistry&& other) noexcept
		{
			mTextures = std::move(other.mTextures);
			return *this;
		}
		TextureError AddTexture(const std::string& name, T&& texture, bool override = false)
		{
			if (!override && mTextures.find(name) != mTextures.end())
				return TextureError::TEXTURE_ALREADY_EXISTS;
			mTextures.emplace(name, std::move(texture));
			return TextureError::NONE;
		}

		TextureError RemoveTexture(const std::string& name)
		{
			if (mTextures.find(name) == mTextures.end())
				return TextureError::TEXTURE_DOES_NOT_EXIST;
			mTextures.erase(name);
			return TextureError::NONE;
		}

		const T& GetTexture(const std::string& name) const
		{
			return mTextures.at(name);
		}
		T& GetTexture(const std::string& name)
		{
			return mTextures.at(name);
		}

		auto begin() { return mTextures.begin(); }
		auto end() { return mTextures.end(); }
		const auto begin() const { return mTextures.begin(); }
		const auto end() const { return mTextures.end(); }

	};


}