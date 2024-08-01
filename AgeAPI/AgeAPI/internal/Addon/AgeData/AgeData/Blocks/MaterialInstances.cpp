#include "MaterialInstances.hpp"
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>
namespace AgeData::BlockComponents
{
    AgeAPI::ErrorString MaterialInstances::WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) const
    {
        auto& [json, alloc] = proxy;
        json.SetObject();
        using namespace AgeAPI::Backend::Rp;
        auto& materials = mInstanceData.mMaterials;
        auto WriteMaterialToJson = [&](AgeAPI::JsonProxy jProxy, const MaterialInstance::MaterialInstanceElement& element)
            {
                auto& [val, allocator] = jProxy;
                val.SetObject();
                if (mInstanceData.mRenderMethod != RenderMethod::opaque)
                    val.AddMember("render_method", mInstanceData.GetRenderMethodString(), allocator);
                if (!element.mFaceDimming)
                    val.AddMember("face_dimming", false, allocator);
                if (element.mAmbientOcclusion)
                    val.AddMember("ambient_occlusion", true, allocator);
                val.AddMember("texture", element.mTextureName, allocator);

            };
        for (auto& side : materials)
        {
            rapidjson::Value sideJson(rapidjson::kObjectType);
            WriteMaterialToJson({ sideJson, alloc }, side);
            rapidjson::ValueWriteWithKey<rapidjson::Value>::WriteToJsonValue(GetTextureSideAsString(side.mSide), sideJson, json, alloc);
        }
        return "";

    }
}
