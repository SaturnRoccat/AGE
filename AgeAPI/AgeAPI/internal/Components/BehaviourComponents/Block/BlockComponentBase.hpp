#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/BehaviourComponentBase.hpp>

namespace AgeAPI::AddonFeatures
{
	class Block;
}
namespace AgeAPI::Components
{
	class BlockComponentBase : public BehaviourComponentBase
	{
	public:
		BlockComponentBase() = default;
		BlockComponentBase(
			ExperimentalSettings settings,
			SemanticVersion version,
			const Identifier& identifier,
			bool canBeDoublePushed = false, // If true, the component can be pushed twice to the same block and will call MergeDoublePush/MergeDoublePushShort if is permutation
			bool isTransient = false, // If true, the component will not be written to the block json
			SemanticVersion maxVersion = {255, 255, 255}
		) 
			: BehaviourComponentBase(settings, version, identifier, canBeDoublePushed, isTransient, maxVersion) {}
		virtual ~BlockComponentBase() = default;
		
		virtual ErrorString WriteToJson(NonOwningPtr<Addon> addon, JsonProxy proxy, NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) const = 0;

		virtual ErrorString MergeDoublePush(
			NonOwningPtr<Addon> addon,
			NonOwningPtr<AgeAPI::AddonFeatures::Block> blk,
			std::unique_ptr<BlockComponentBase>& other) 
		{ 
			return ErrorString();
		};

		virtual ErrorString OnComponentAdded(NonOwningPtr<Addon> addon, NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) { return""; };

		virtual ErrorString OnComponentRemoved(NonOwningPtr<Addon> addon, NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) { return""; };

		virtual BlockComponentBase* Clone() const = 0;
	};
	
	template<typename T>
	concept BlockComponent = std::is_base_of_v<BlockComponentBase, T>;
}
