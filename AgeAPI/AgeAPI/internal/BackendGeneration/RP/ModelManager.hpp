#pragma once

#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Models/Models.hpp>

namespace AgeAPI::Backend::Rp
{
	enum class ModelError
	{
		NONE,
		MODEL_ALREADY_EXISTS,
		MODEL_DOES_NOT_EXIST
	};


	class ModelManager
	{
	private:
		std::unordered_map<std::string, Geometry> mModels{};
		friend class ResourcePack;
	private:
		ModelManager() = default;
		ModelManager(const ModelManager& other) = delete;
		ModelManager(ModelManager&& other) noexcept = default;
	public:
		ModelManager& operator=(const ModelManager& other) = delete;
		ModelManager& operator=(ModelManager&& other) noexcept = default;

		ModelError AddModel(const std::string& name, const Geometry& model, bool overwrite = false)
		{
			if (!overwrite && mModels.find(name) != mModels.end())
				return ModelError::MODEL_ALREADY_EXISTS;
			mModels[name] = model;
			return ModelError::NONE;
		}

		ModelError RemoveModel(const std::string& name)
		{
			if (mModels.find(name) == mModels.end())
				return ModelError::MODEL_DOES_NOT_EXIST;
			mModels.erase(name);
			return ModelError::NONE;
		}

		const Geometry& GetModel(const std::string& name) const
		{
			return mModels.at(name);
		}

		Geometry& GetModel(const std::string& name)
		{
			return mModels.at(name);
		}
	};

}