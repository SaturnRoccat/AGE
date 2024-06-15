#pragma once 
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <memory>

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


		ErrorString WriteToJson(JsonProxy& proxy, std::unique_ptr<Addon>& addon, NonOwningPtr<Backend::Bp::BlockBehaviour> blk) const;



		const std::string& GetCondition() const { return mCondition; }
		const std::vector<std::unique_ptr<Components::BlockComponentBase>>& GetComponents() const { return mComponents; }

		void SetCondition(std::string_view condition) { mCondition = condition; }

		void AddComponent(std::unique_ptr<Components::BlockComponentBase> component) { mComponents.emplace_back(std::move(component)); }
	private:
		std::string mCondition;
		std::vector<std::unique_ptr<Components::BlockComponentBase>> mComponents{};
	};

}