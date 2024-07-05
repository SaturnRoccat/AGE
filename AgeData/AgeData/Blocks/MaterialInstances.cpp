#include "MaterialInstances.hpp"
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>
namespace AgeData::BlockComponents
{
    inline AgeAPI::ErrorString MaterialInstances::WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const
    {
        auto& [json, alloc] = proxy;
        json.SetObject();

        for (auto& textureInstance : this->mValue.mInstances)
        {
            std::visit(
                AgeAPI::overloaded{
                    [&](const MaterialInstancesWithData::InstanceReference& ref)
                    {
                        rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue(ref.mInstanceName, ref.mReference, json, alloc);
                    },
                    [&](const Instance& instance)
                    {
                        rapidjson::Value textureInstanceJson;
                        textureInstanceJson.SetObject();
                        rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue("texture", instance.mTexture, textureInstanceJson, alloc);
                        if (instance.mRenderMethod != Instance::RenderMethod::opaque)
							rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue("render_method", instance.GetRenderMethodString(), textureInstanceJson, alloc);
                        if (!instance.mFaceDimming)
                            textureInstanceJson.AddMember("face_dimming", false, alloc);
                        if (!instance.mAmbientOcclusion)
                            textureInstanceJson.AddMember("ambient_occlusion", false, alloc);
                        rapidjson::Value key(instance.mInstanceName, alloc);
                        json.AddMember(key, textureInstanceJson, alloc);
                    }
                
                }, textureInstance);


        }
    }
}
