#ifndef AGE_DATA_BLOCK_COMPONENTS
#define AGE_DATA_BLOCK_COMPONENTS
#include <AgeData/Blocks/CollisionBox.hpp>
#include <AgeData/Blocks/CraftingTable.hpp>
#include <AgeData/Blocks/DestructibleByExplosion.hpp>
#include <AgeData/Blocks/DestructibleByMining.hpp>
#include <AgeData/Blocks/DisplayName.hpp>
#include <AgeData/Blocks/Flammable.hpp>
#include <AgeData/Blocks/Friction.hpp>
#include <AgeData/Blocks/Geometry.hpp>
#include <AgeData/Blocks/LightDampening.hpp>
#include <AgeData/Blocks/LightEmission.hpp>
#include <AgeData/Blocks/MapColor.hpp>
#include <AgeData/Blocks/Loot.hpp>
#include <memory>
#include <AgeData/Blocks/MaterialInstances.hpp>

namespace AgeData::BlockComponents
{
	using BlockComponentStore = std::unique_ptr<AgeAPI::Components::BlockComponentBase>;
	
	template<typename T>
	std::unique_ptr<AgeAPI::Components::BlockComponentBase> CreateBlockComponent(T&& t)
	{
		T* ptr = new T(std::forward<T>(t));
		return BlockComponentStore((AgeAPI::Components::BlockComponentBase*)ptr);
	}

	template<typename... Ts>
	std::vector<BlockComponentStore> MakeComponentStore(Ts&&... ts)
	{
		std::vector<BlockComponentStore> store;
		store.reserve(sizeof...(Ts)); // Reserve the size of the parameter pack
		(store.emplace_back(std::move(CreateBlockComponent(std::forward<Ts>(ts)))), ...); // Fold expression to push all the components into the store
		return store;
	}
}

#endif // !AGE_DATA_BLOCK_COMPONENTS
