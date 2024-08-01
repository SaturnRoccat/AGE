#include "Transform.hpp"

namespace AgeData::BlockComponents
{
	AgeAPI::ErrorString Transformation::WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) const
	{
		auto& json = proxy.mWriteLoc;
		json["translation"] = mTranslation;
		json["rotation"] = mRotation;
		json["scale"] = mScale;
		return {};
	}
}