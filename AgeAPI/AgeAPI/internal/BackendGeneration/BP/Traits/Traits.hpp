#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>

namespace AgeAPI::AddonFeatures
{
	class Block;
}

namespace AgeAPI::Backend::Bp
{

	class TraitBase
	{
	public:
		virtual ~TraitBase() = default;
		virtual ErrorString WriteToJson(JsonProxy proxy, NonOwningPtr<Addon> addon, NonOwningPtr<AddonFeatures::Block> blk) const = 0;
		TraitBase(const Identifier& id) : mTraitId(id) {}
		template<class Self>
		auto&& GetTraitId(this Self&& self)
		{
			return std::forward<Self>(self).mTraitId;
		}

	protected:
		Identifier mTraitId;

	};
	enum class EnabledStates : u8
	{
		Cardinal = 1,
		Facing = 2,
		VerticalHalf = 4,
		None = 0
	};
	class PlacementDirectionTrait : public TraitBase
	{
	public:
		PlacementDirectionTrait(
			EnabledStates enabledStates = {EnabledStates::None},
			u16 yRotation = 0
		) : TraitBase("minecraft:placement_direction"), mEnabledStates(enabledStates), mYRotation(yRotation) {}
		~PlacementDirectionTrait() override = default;
		ErrorString WriteToJson(JsonProxy proxy, NonOwningPtr<Addon> addon, NonOwningPtr<AddonFeatures::Block> blk) const override;

		bool HasCardinalDirection() const { return ToUnderlying(mEnabledStates) & ToUnderlying(EnabledStates::Cardinal); }
		bool HasFacingDirection() const { return ToUnderlying(mEnabledStates) & ToUnderlying(EnabledStates::Facing); }
	private:
		EnabledStates mEnabledStates{};
		u16 mYRotation{ 0 };
	};

	class PlacementPositionTrait : public TraitBase
	{
	public:
		PlacementPositionTrait(
			EnabledStates enabledStates = {EnabledStates::None}
		) : TraitBase("minecraft:placement_position"), mEnabledStates(enabledStates) {}
		~PlacementPositionTrait() override = default;
		ErrorString WriteToJson(JsonProxy proxy, NonOwningPtr<Addon> addon, NonOwningPtr<AddonFeatures::Block> blk) const override;
		bool HasFacingDirection() const { return ToUnderlying(mEnabledStates) & ToUnderlying(EnabledStates::Facing); }
		bool hasVerticalHalf() const { return ToUnderlying(mEnabledStates) & ToUnderlying(EnabledStates::VerticalHalf); }

	private:
		EnabledStates mEnabledStates{};
	};

}
