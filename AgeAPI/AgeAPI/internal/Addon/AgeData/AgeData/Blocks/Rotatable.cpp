#include "Rotatable.hpp"
#include "Transform.hpp"
#include <AgeAPI/internal/BackendGeneration/Permutations/Permutation.hpp>
#include <AgeAPI/internal/BackendGeneration/BP/Traits/Traits.hpp>
#include <AgeAPI/internal/Addon/AddonFeatures/Block.hpp>
using namespace AgeAPI::Backend::Bp;
using namespace AgeAPI::Backend;
namespace AgeData::BlockComponents
{
	AgeAPI::ErrorString Rotatable::OnComponentAdded(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk)
	{
		blk->AddTrait(
			PlacementDirectionTrait(
				EnabledStates::Facing
			)
		);
		std::array<Permutation, 4> permutations = {
			Permutation("q.block_state('minecraft:facing_direction') == 'north'").AddComponent(Transformation{}),
			Permutation("q.block_state('minecraft:facing_direction') == 'south'").AddComponent(Transformation{{0, 180, 0}}),
			Permutation("q.block_state('minecraft:facing_direction') == 'east'").AddComponent(Transformation{{0, 90, 0}}),
			Permutation("q.block_state('minecraft:facing_direction') == 'west'").AddComponent(Transformation{{0, 270, 0}}),
		};
		blk->AddPermutations(permutations);
		if (!mAllowRotation)
			return {};
		std::array<Permutation, 2> extraRotations = {
			Permutation("q.block_state('minecraft:facing_direction') == 'up'").AddComponent(Transformation{{90, 0, 0}}),
			Permutation("q.block_state('minecraft:facing_direction') == 'down'").AddComponent(Transformation{{270, 0, 0}})
		};
		blk->AddPermutations(extraRotations);
		return {};
	}
}