#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>

namespace AgeData::BlockComponents
{
    using AgeAPI::u8;
    using AgeAPI::IVec3;
    using AgeAPI::FVec3;
    class Transformation : public AgeAPI::Components::BlockComponentBase
    {
    public:
        Transformation(
            IVec3 translation = {0, 0, 0},
            IVec3 rotation = {0, 0, 0},
            FVec3 scale = {1, 1, 1}
        ) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 60 },
            "minecraft:transformation"
        ), mTranslation(translation), mRotation(rotation), mScale(scale)
         {}
        
        void SetTranslation(IVec3 translation) { mTranslation = translation; }
        void SetRotation(IVec3 rotation) { mRotation = rotation; }
        void SetScale(FVec3 scale) { mScale = scale; }

        IVec3 GetTranslation() const { return mTranslation; }
        IVec3 GetRotation() const { return mRotation; }
        FVec3 GetScale() const { return mScale; }
        BlockComponentBase* Clone() const override {
            return new Transformation(*this);
        }

        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) const override;
    private:
        IVec3 mTranslation{};
        IVec3 mRotation{};
        FVec3 mScale{};
    };
}