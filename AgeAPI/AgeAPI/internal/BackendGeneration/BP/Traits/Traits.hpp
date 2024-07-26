#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>

namespace AgeAPI::AddonFeatures
{
	class Block;
}

namespace AgeAPI::Backend::Bp
{
	class TraitBase
	{
	public:
		virtual ~TraitBase() = default;
		virtual ErrorString WriteToJson(JsonProxy proxy, NonOwningPtr<Addon> addon, NonOwningPtr<AddonFeatures::Block> blk) const = 0;

		template<class Self>
		auto&& GetTraitId(this Self&& self)
		{
			return std::forward<Self>(self).mTraitId;
		}

		ReferenceExpected<TraitBase, ErrorString> AddComponent(std::unique_ptr<Components::BlockComponentBase> component, bool override = false, NonOwningPtr<Addon> addon = nullptr);
		template<Components::BlockComponent Component>
		ReferenceExpected<TraitBase, ErrorString> AddComponent(const Component& component, bool override = false, NonOwningPtr<Addon> addon = nullptr)
		{
			return AddComponent(std::make_unique<Component>(component), override, addon);
		}
		ReferenceExpected<TraitBase, ErrorString> RemoveComponent(const std::string& componentName, NonOwningPtr<Addon> addon = nullptr);
		template<typename Container> requires std::ranges::range<Container>
		ReferenceExpected<TraitBase, ErrorString> AddComponentsCopy(
			const Container& components,
			bool override = false,
			NonOwningPtr<Addon> addon = nullptr)
		{
			for (auto& component : components)
			{
				auto ptr = std::unique_ptr<Components::BlockComponentBase>(component->Clone());
				auto err = AddComponent(std::move(ptr), override, addon);
				if (!err)
					return err;
			}
			return *this;
		}
		template<typename Container> requires std::ranges::range<Container>
		ReferenceExpected<TraitBase, ErrorString> AddComponents(
			Container&& components,
			bool override = false,
			NonOwningPtr<Addon> addon = nullptr)
		{
			for (auto& component : components)
			{
				auto err = AddComponent(std::move(component), override, addon);
				if (!err)
					return err;
			}
			return *this;
		}

	private:
		Identifier mTraitId;
		absl::flat_hash_map<std::string, std::unique_ptr<Components::BlockComponentBase>> mComponents{};

	};

}

/*
* Ref
* ReferenceExpected<Block, ErrorString> AddComponent(std::unique_ptr<Components::BlockComponentBase> component, bool override = false, NonOwningPtr<Addon> addon = nullptr);
		template<Components::BlockComponent Component>
		ReferenceExpected<Block, ErrorString> AddComponent(const Component& component, bool override = false, NonOwningPtr<Addon> addon = nullptr)
		{
			return AddComponent(std::make_unique<Component>(component), override, addon);
		}
		ReferenceExpected<Block, ErrorString> RemoveComponent(const std::string& componentName, NonOwningPtr<Addon> addon = nullptr);
		template<typename Container> requires std::ranges::range<Container>
		ReferenceExpected<Block, ErrorString> AddComponentsCopy(
			const Container& components,
			bool override = false,
			NonOwningPtr<Addon> addon = nullptr)
		{
			for (auto& component : components)
			{
				auto ptr = std::unique_ptr<Components::BlockComponentBase>(component->Clone());
				auto err = AddComponent(std::move(ptr), override, addon);
				if (!err)
					return err;
			}
			return *this;
		}
		template<typename Container> requires std::ranges::range<Container>
		ReferenceExpected<Block, ErrorString> AddComponents(
			Container&& components,
			bool override = false,
			NonOwningPtr<Addon> addon = nullptr)
		{
			for (auto& component : components)
			{
				auto err = AddComponent(std::move(component), override, addon);
				if (!err)
					return err;
			}
			return *this;
		}
*/