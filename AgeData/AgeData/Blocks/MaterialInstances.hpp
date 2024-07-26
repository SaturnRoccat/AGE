#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Block/BlockResource.hpp>
#include <AgeData/Details/Types.hpp>
#include <variant>

namespace AgeData::BlockComponents
{
    // TODO: Switch to a custom geometry string that has validation
    // TODO: Add support for referencing other geometry files
    class MaterialInstances : public AgeAPI::Components::BlockComponentBase
    {
    public:
    
    public:
        MaterialInstances(
            const AgeAPI::Backend::Rp::MaterialInstance& instanceData
        ) : mInstanceData(instanceData), AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 50 },
            "minecraft:material_instances"
        )
        {}
        MaterialInstances(
            AgeAPI::Backend::Rp::MaterialInstance&& instanceData
        ) : mInstanceData(std::move(instanceData)), AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 50 },
            "minecraft:material_instances"
        )
        {}
        void SetMaterialInstance(AgeAPI::Backend::Rp::MaterialInstance& instanceData)
		{
			mInstanceData = instanceData;
		}
        AgeAPI::Backend::Rp::MaterialInstance& GetMaterialInstance() { return mInstanceData; }
        const AgeAPI::Backend::Rp::MaterialInstance& GetMaterialInstance() const { return mInstanceData; }
        BlockComponentBase* Clone() const override {
            return new MaterialInstances(mInstanceData);
        }

       
        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) const override;
    private:
        AgeAPI::Backend::Rp::MaterialInstance mInstanceData;
    };
}