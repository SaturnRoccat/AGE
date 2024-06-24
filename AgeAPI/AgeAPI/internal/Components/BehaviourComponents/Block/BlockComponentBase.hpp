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
			const ExperimentalSettings& settings,
			const SemanticVersion& version,
			const Identifier& identifier,
			bool canBeDoublePushed = false,
			bool isTransient = false)
			: BehaviourComponentBase(settings, version, identifier, canBeDoublePushed, isTransient) {}
		virtual ~BlockComponentBase() = default;
		
		virtual ErrorString WriteToJson(std::unique_ptr<Addon>& addon, JsonProxy proxy, NonOwningPtr<Backend::Bp::BlockBehaviour> blk) const = 0;

		virtual ErrorString MergeDoublePush(std::unique_ptr<Addon>& addon, NonOwningPtr<Backend::Bp::BlockBehaviour> blk, std::unique_ptr<BlockComponentBase>& other) {};

		virtual ErrorString MergeDoublePushShort(std::unique_ptr<BlockComponentBase>& other) {};

        virtual void OnComponentAdded(std::unique_ptr<Addon>& addon, NonOwningPtr<Backend::Bp::BlockBehaviour> blk) {};

	};
	
	template<typename T>
	concept BlockComponent = std::is_base_of_v<BlockComponentBase, T>;
}
