#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/BehaviourComponentBase.hpp>

namespace AgeAPI::Backend::Bp
{
	class BlockBehaviour;

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
			bool isTransient = false) // If true, the component will not be written to the block json
			: BehaviourComponentBase(settings, version, identifier, canBeDoublePushed, isTransient) {}
		virtual ~BlockComponentBase() = default;
		
		virtual ErrorString WriteToJson(NonOwningPtr<Addon> addon, JsonProxy proxy, NonOwningPtr<Backend::Bp::BlockBehaviour> blk) const = 0;

		virtual ErrorString MergeDoublePush(NonOwningPtr<Addon> addon, NonOwningPtr<Backend::Bp::BlockBehaviour> blk, std::unique_ptr<BlockComponentBase>& other) { return ErrorString(); };

		virtual ErrorString MergeDoublePushShort(std::unique_ptr<BlockComponentBase>& other) { return ErrorString(); };

		virtual ErrorString OnComponentAdded(NonOwningPtr<Addon> addon, NonOwningPtr<Backend::Bp::BlockBehaviour> blk) { return""; };

	};
	
	template<typename T>
	concept BlockComponent = std::is_base_of_v<BlockComponentBase, T>;
}
