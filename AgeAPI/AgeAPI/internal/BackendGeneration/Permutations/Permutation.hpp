#pragma once 
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <memory>
#include <unordered_map>

namespace AgeAPI::AddonFeatures
{
	class Block;

}
namespace AgeAPI::Backend
{
	
	class Permutation
	{ 
	public:
		Permutation() = default;
		Permutation(const std::string& condition) : mCondition(condition) {}
		Permutation(const Permutation& other)
		{
			mCondition = other.mCondition;
			auto& otherComponents = const_cast<Permutation&>(other).mComponents;
			for (auto& [key, value] : otherComponents)
			{
				auto ptr = std::unique_ptr<Components::BlockComponentBase>(value->Clone());
				mComponents.emplace(key, std::move(ptr));
			}
		}
		Permutation(Permutation&& other) noexcept
		{
			mCondition = std::move(other.mCondition);
			mComponents = std::move(other.mComponents);
		}


		ErrorString WriteToJson(JsonProxy proxy, NonOwningPtr<Addon> addon, NonOwningPtr<AddonFeatures::Block> blk) const;



		const std::string& GetCondition() const { return mCondition; }
		const auto& GetComponents() const { return mComponents; }

		void SetCondition(std::string_view condition) { mCondition = condition; }

		ReferenceExpected<Permutation, ErrorString> AddComponent(std::unique_ptr<Components::BlockComponentBase> component);

	private:
		std::string mCondition;
		std::unordered_multimap<std::string, std::unique_ptr<Components::BlockComponentBase>> mComponents{};
		friend class AgeAPI::AddonFeatures::Block;
	};

}