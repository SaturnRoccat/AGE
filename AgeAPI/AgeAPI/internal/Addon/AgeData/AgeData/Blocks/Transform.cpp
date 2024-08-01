#include "Transform.hpp"

namespace AgeData::BlockComponents
{
	AgeAPI::ErrorString Transformation::WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) const
	{
		auto& json = proxy.mWriteLoc;
		rapidjson::ValueWriteWithKey<std::array<int, 3>>::WriteToJsonValue("rotation", mRotation.ToArray(), json, proxy.mAllocator);
		rapidjson::ValueWriteWithKey<std::array<int, 3>>::WriteToJsonValue("translation", mTranslation.ToArray(), json, proxy.mAllocator);
		if (mScale != FVec3{0, 0, 0})
			rapidjson::ValueWriteWithKey<std::array<float, 3>>::WriteToJsonValue("scale", mScale.ToArray(), json, proxy.mAllocator);
		return {};
	}
}