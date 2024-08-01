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
		Permutation& operator=(const Permutation& other)
		{
			mCondition = other.mCondition;
			auto& otherComponents = const_cast<Permutation&>(other).mComponents;
			for (auto& [key, value] : otherComponents)
			{
				auto ptr = std::unique_ptr<Components::BlockComponentBase>(value->Clone());
				mComponents.emplace(key, std::move(ptr));
			}
			return *this;
		}
		Permutation& operator=(Permutation&& other) noexcept
		{
			mCondition = std::move(other.mCondition);
			mComponents = std::move(other.mComponents);
			return *this;
		}


		ErrorString WriteToJson(JsonProxy proxy, NonOwningPtr<Addon> addon, NonOwningPtr<AddonFeatures::Block> blk);

		Permutation Finalize()
		{
			Permutation perm;
			std::swap(perm.mCondition, mCondition);
			std::swap(perm.mComponents, mComponents);
			return perm;
		}
			
		const std::string& GetCondition() const { return mCondition; }
		const auto& GetComponents() const { return mComponents; }

		void SetCondition(std::string_view condition) { mCondition = condition; }

		Permutation&& AddComponent(std::unique_ptr<Components::BlockComponentBase> component, bool override = false, NonOwningPtr<Addon> addon = nullptr);
		template<Components::BlockComponent Component>
		Permutation&& AddComponent(Component&& component, bool override = false, NonOwningPtr<Addon> addon = nullptr)
		{
			return AddComponent(std::make_unique<Component>(std::forward<Component>(component)), override, addon);
		}
		template<typename Container> requires std::ranges::range<Container>
		Permutation&& AddComponents(
			const Container& components,
			bool override = false,
			NonOwningPtr<Addon> addon = nullptr)
		{
			for (auto& component : components)
			{
				auto ptr = std::unique_ptr<Components::BlockComponentBase>(component->Clone());
				auto err = AddComponent(std::move(ptr), override, addon);
				if (!err)
					throw std::runtime_error(err.GetAsString());
			}
			return std::move(*this);
		}
		template<typename Container> requires std::ranges::range<Container>
		Permutation&& AddComponents(
			Container&& components,
			bool override = false,
			NonOwningPtr<Addon> addon = nullptr)
		{
			for (auto& component : components)
				AddComponent(std::move(component), override, addon);
			return std::move(*this);
		}

	private:
		std::string mCondition;
		absl::flat_hash_map<std::string, std::unique_ptr<Components::BlockComponentBase>> mComponents{};
		friend class AgeAPI::AddonFeatures::Block;
	};

}