#include <AgeData/Block.hpp>


namespace AgeData
{
	void Block::SetGeo(NonOwningPtr<Addon> addon, const Backend::Rp::Geometry& geo, const Backend::Rp::MaterialInstance& mat)
	{
		auto& textures = mResourceHandler.GetMultiTextures();
		mResourceHandler.SetGeo(geo);
		BlockComponents::Geometry geoComp(geo.GetGeoName());
		mBehaviourHandler.AddBlockComponentInternal(addon, std::move(
			PC<Components::BlockComponentBase, BlockComponents::Geometry>(
					std::move(geoComp)
			)
		));
		if (mat.mMaterials.size() == 0) // They default ctord the func so we populate based on the existing textures
		{
			Backend::Rp::MaterialInstance newMat(mat.mRenderMethod);
			for (auto& [side, tex] : textures)
				newMat.mMaterials.push_back({ tex.mTextureAlias, side });
			mBehaviourHandler.AddBlockComponentInternal(addon, std::move(
				PC<Components::BlockComponentBase, BlockComponents::MaterialInstances>(
					std::move(newMat)
				)
			));
		}
		else
		{
			mBehaviourHandler.AddBlockComponentInternal(addon, std::move(
				PC<Components::BlockComponentBase, BlockComponents::MaterialInstances>(
					mat
				)
			));
		}
	}
}