#include "Traits.hpp"
#include <AgeAPI/internal/Addon/Addon.hpp>
namespace AgeAPI::Backend::Bp
{
	ErrorString PlacementDirectionTrait::WriteToJson(JsonProxy proxy, NonOwningPtr<Addon> addon) const
	{
		auto& [json, alloc] = proxy;
		rapidjson::Value enabledStatesArray(rapidjson::kArrayType);
		if (HasCardinalDirection())
			enabledStatesArray.PushBack("minecraft:cardinal_direction", alloc);
		if (HasFacingDirection())
			enabledStatesArray.PushBack("minecraft:facing_direction", alloc);
		json.AddMember("enabled_states", enabledStatesArray, alloc);
		json.AddMember("y_rotation_offset", mYRotation, alloc);
		return {};
	}
	ErrorString PlacementPositionTrait::WriteToJson(JsonProxy proxy, NonOwningPtr<Addon> addon) const
	{
		auto& [json, alloc] = proxy;
		rapidjson::Value enabledStatesArray(rapidjson::kArrayType);
		if (HasFacingDirection())
			enabledStatesArray.PushBack("minecraft:block_face", alloc);
		if (hasVerticalHalf())
			enabledStatesArray.PushBack("minecraft:vertical_half", alloc);
		json.AddMember("enabled_states", enabledStatesArray, alloc);
		return {};
	}
}
