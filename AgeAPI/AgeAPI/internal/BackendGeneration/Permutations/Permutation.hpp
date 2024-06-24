#pragma once 
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <memory>
#include <unordered_map>

namespace AgeAPI::Backend
{
	namespace Bp
	{
		class BlockBehaviour;
	}
	
	class Permutation
	{ 
	public:
		Permutation() = default;
		Permutation(std::string_view condition) : mCondition(condition) {}
		Permutation(const Permutation& other) = delete;
		Permutation(Permutation&& other) noexcept
		{
			mCondition = std::move(other.mCondition);
			mComponents = std::move(other.mComponents);
		}


		ErrorString WriteToJson(JsonProxy proxy, NonOwningPtr<Addon> addon, NonOwningPtr<Backend::Bp::BlockBehaviour> blk) const;



		const std::string& GetCondition() const { return mCondition; }
		const auto& GetComponents() const { return mComponents; }

		void SetCondition(std::string_view condition) { mCondition = condition; }

		ErrorString AddComponent(std::unique_ptr<Components::BlockComponentBase>& component) 
		{
			auto it = mComponents.find(component->GetComponentID().GetFullNamespace());

			if (it != mComponents.end() && !component->CanBeDoublePushed())
				return ErrorString("Component already exists");
			else if (component->CanBeDoublePushed() && it != mComponents.end())
				return it->second->MergeDoublePushShort(component);
			else
				mComponents[component->GetComponentID().GetFullNamespace()] = std::move(component);
			return "";
		}
	private:
		std::string mCondition;
		std::unordered_map<std::string, std::unique_ptr<Components::BlockComponentBase>> mComponents{};
	};

}