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
        if (materials.size() == 1)
        {
            auto& side = materials[0];
            rapidjson::Value AllSide(rapidjson::kObjectType);
            if (side.mSide != TextureSide::all)
                std::println("Warning: MaterialInstances has only one material, but it's side is not TextureSide::all");
                //TODO: Switch over to a global logger for errors and warnings
            WriteMaterialToJson({ AllSide, alloc }, side);
            json.AddMember("*", AllSide, alloc);
            return "";
        }
       
        std::list<TextureSide> unlabledSides = {
            TextureSide::up,
            TextureSide::down,
            TextureSide::north,
            TextureSide::east,
            TextureSide::south,
            TextureSide::west
		};
        AgeAPI::NonOwningPtr<const MaterialInstance::MaterialInstanceElement> element = nullptr;
        for (auto& side : materials)
        {
            if (side.mSide == TextureSide::all)
            {
                element = &side;
                continue;
            }
            rapidjson::Value sideJson(rapidjson::kObjectType);
            WriteMaterialToJson({ sideJson, alloc }, side);
            rapidjson::ValueWriteWithKey<rapidjson::Value>::WriteToJsonValue(GetTextureSideAsString(side.mSide), sideJson, json, alloc);
            unlabledSides.remove(side.mSide);
        }
        if (!element && !unlabledSides.empty())
            return "MaterialInstances has no material for TextureSide::all and hasnt supplied enough materials for the other sides";

        for (auto& side : unlabledSides)
		{
			rapidjson::Value sideJson(rapidjson::kObjectType);
			WriteMaterialToJson({ sideJson, alloc }, *element);
			rapidjson::ValueWriteWithKey<rapidjson::Value>::WriteToJsonValue(GetTextureSideAsString(side), sideJson, json, alloc);
		}
        return "";

    }
}
