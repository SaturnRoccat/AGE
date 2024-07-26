#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>


namespace AgeData::BlockComponents
{
   class CollisionBox : public AgeAPI::Components::BlockComponentBase
   {
    public:
        CollisionBox(const AgeAPI::IVec3& origin, const AgeAPI::IVec3& size) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 50 },
            "minecraft:collision_box"
        ), mValue({ origin, size }) {}

        CollisionBox(bool value) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 50 },
            "minecraft:collision_box"
        ), mValue(value) {}

        void SetCollisionBox(const AgeAPI::IVec3& origin, const AgeAPI::IVec3& size) { mValue = { origin, size }; }
        void SetCollisionBox(bool value) { mValue = value; }
        AgeAPI::IVec3 GetCollisionBoxOrigin() const { return mValue.GetData().GetOrigin(); }
        AgeAPI::IVec3 GetCollisionBoxSize() const { return mValue.GetData().GetSize(); }
        bool GetCollisionBoxBool() const { return mValue; }
        bool IsData() const { return !mValue.IsToggle(); }
        BlockComponentBase* Clone() const override { return new CollisionBox(*this); }


        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) const override;
   private:
        ToggleOrData<BlockBounds, true> mValue;
   };

    
}
