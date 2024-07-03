#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>
#include <variant>

namespace AgeData::BlockComponents
{
    // TODO: Switch to a custom geometry string that has validation
    // TODO: Add support for referencing other geometry files
    class MaterialInstances : public AgeAPI::Components::BlockComponentBase
    {
    public:
        struct Instance
        {
            std::string mTexture{};
            enum class RenderMethod
            {
                opaque,
                doubleSided,
                blend,
                alphaTest
            };
            RenderMethod mRenderMethod{ RenderMethod::opaque };
            const std::string& GetRenderMethodString() const
            {
                switch (mRenderMethod)
                {
                case RenderMethod::opaque:
                    return "opaque";
                case RenderMethod::doubleSided:
                    return "double_sided";
                case RenderMethod::blend:
                    return "blend";
                case RenderMethod::alphaTest:
                    return "alpha_test";
                default:
                    return "opaque";
                }
            }
            bool mFaceDimming{ true };
            bool mAmbientOcclusion{ true };
            std::string mInstanceName{ "*" };
            Instance() = default;
            Instance(
                const std::string& texture,
                const std::string& instanceName = "*",
                RenderMethod renderMethod = RenderMethod::opaque,
                bool faceDimming = true,
                bool ambientOcclusion = true
            ) : mTexture(texture), mInstanceName(instanceName), mRenderMethod(renderMethod), mFaceDimming(faceDimming), mAmbientOcclusion(ambientOcclusion) {}
        };

        struct MaterialInstancesWithData
        {
            struct InstanceReference
            {
                std::string mReference{};
                std::string mInstanceName{"*"};
                InstanceReference(const std::string& reference) : mReference(reference) {}
                InstanceReference() = default;
                InstanceReference(const Instance& reference) : mReference(reference.mInstanceName) {}
            };
            std::vector<
                std::variant<
                    Instance,
                    InstanceReference
            >> mInstances{};

        };
    public:
        MaterialInstances(const Instance& geometry = {}) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 50 },
            "minecraft:material_instances"
        ), mValue(geometry) {}

        MaterialInstances(const MaterialInstancesWithData& geometry) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 50 },
            "minecraft:material_instances"
        ), mValue(geometry) {}

        void AddInstance(const Instance& instance)
        {
            mValue.mInstances.push_back(instance);
        }

        void AddInstanceReference(const std::string& reference)
        {
            mValue.mInstances.push_back(MaterialInstancesWithData::InstanceReference{reference});
        }

        void AddInstanceReference(const Instance& reference)
        {
            mValue.mInstances.push_back(MaterialInstancesWithData::InstanceReference{reference});
        }
        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const override;
    private:
        MaterialInstancesWithData mValue{};
    };
}