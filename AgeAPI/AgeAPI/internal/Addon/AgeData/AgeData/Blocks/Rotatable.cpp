#include "Rotatable.hpp"
#include "Transform.hpp"
#include <AgeAPI/internal/BackendGeneration/Permutations/Permutation.hpp>
#include <AgeAPI/internal/BackendGeneration/BP/Traits/Traits.hpp>
#include <AgeAPI/internal/Addon/AddonFeatures/Block.hpp>
#include "LightEmission.hpp"
using namespace AgeAPI::Backend::Bp;
using namespace AgeAPI::Backend;
namespace AgeData::BlockComponents
{
	AgeAPI::ErrorString Rotatable::OnComponentAdded(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk)
	{
		blk->AddTrait(
			PlacementDirectionTrait(
				EnabledStates::Cardinal
			)
		);
		std::array<Permutation, 4> permutations = {
			Permutation("q.block_state('minecraft:cardinal_direction') == 'north'").AddComponent(Transformation{{0, 180, 0}}),
			Permutation("q.block_state('minecraft:cardinal_direction') == 'south'").AddComponent(Transformation{{0, 0, 0}}),
			Permutation("q.block_state('minecraft:cardinal_direction') == 'east'").AddComponent(Transformation{{0, 90, 0}}),
			Permutation("q.block_state('minecraft:cardinal_direction') == 'west'").AddComponent(Transformation{{0, 270, 0}}),
		};
		blk->AddPermutations(permutations);
		if (!mAllowRotation)
			return {};
		blk->AddTrait(
			PlacementDirectionTrait{
				EnabledStates::Facing
			}
		);

		std::array<std::pair<std::string, IVec3>, 2> topDir = 
		{ 
			std::make_pair("up", IVec3(90, 0, 0)),
			std::make_pair("down", IVec3(270, 0, 0)) 
		};
		std::array<std::pair<std::string, IVec3>, 4> sideDir = 
		{ 
			std::make_pair("north", IVec3(0, 180, 0)),
			std::make_pair("south", IVec3(0, 0, 0)),
			std::make_pair("east", IVec3(0, 270, 0)),
			std::make_pair("west", IVec3(0, 90, 0))
		};
		for (auto& [dir, rot] : topDir)
		{
			for (auto& [side, rot2] : sideDir)
			{
				Permutation p(
					std::format(
						"q.block_state('minecraft:cardinal_direction') == '{}' && q.block_state('minecraft:facing_direction') == '{}'", side, dir
					)
				);
				p.AddComponent(Transformation{ rot + rot2 });
				blk->AddPermutation(p);
			}
		}
		return {};
	}
}