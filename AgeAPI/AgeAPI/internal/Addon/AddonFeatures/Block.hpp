#pragma once
#include <AgeAPI/internal/BackendGeneration/BP/Block/BlockBehaviour.hpp>
#include <AgeAPI/internal/BackendGeneration/BP/BehaviourPack.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/ResourcePack.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Block/BlockResource.hpp>

namespace AgeAPI::AddonFeatures
{
	class IBlock
	{
	public:
		IBlock() = default;
		virtual ~IBlock() = default;
		virtual ErrorString AddComponent(NonOwningPtr<Addon> addon, std::unique_ptr<Components::BlockComponentBase> component) = 0;
		virtual void AddState(std::unique_ptr<Backend::AState> state) = 0;
		virtual void AddPermutation(Backend::Permutation&& permutation) = 0;
		virtual void SetResource(Backend::Rp::BlockResource&& resource) = 0;
		virtual Backend::Rp::BlockResource& GetResource() = 0;
		virtual void WriteToPacks(Backend::Bp::BehaviourPack& behaviourPack, Backend::Rp::ResourcePack& resourcePack) = 0;
		virtual void SetIdentifier(const Identifier& identifier) = 0;
		virtual void SetShouldUseBlockJson(bool shouldUseBlockJson) = 0;
		virtual void SetCategory(const std::string& category) = 0;
	};

	class Block : public IBlock
	{
	private:
		Backend::Bp::BlockBehaviour mBlockBehaviour;
		Backend::Rp::BlockResource mBlockResource;
		bool mShouldUseBlockJson{ true };
	public:
		Block(const Identifier& identifier, SemanticVersion formatVersion) : mBlockBehaviour(identifier, formatVersion) {}
		Block(
			const Identifier& identifier,
			SemanticVersion formatVersion,
			const std::string& textureShortName,
			const std::vector<Backend::Rp::BlockResourceElement>& resources
		) : mBlockBehaviour(identifier, formatVersion), mBlockResource(textureShortName, resources) {}

		ErrorString AddComponent(NonOwningPtr<Addon> addon, std::unique_ptr<Components::BlockComponentBase> component) override {
			return mBlockBehaviour.AddBlockComponent(addon, component); 
		}

		Block& AddComponentChain(NonOwningPtr<Addon> addon, std::unique_ptr<Components::BlockComponentBase> component) {
			auto err = AddComponent(addon, std::move(component));
			if (err.ContainsError())
				throw std::runtime_error(std::format("Failed to add component: {}", err.GetAsString()));
			return *this;
		}

		void AddState(std::unique_ptr<Backend::AState> state) override { mBlockBehaviour.AddState(std::move(state)); }
		void AddPermutation(Backend::Permutation&& permutation) override { mBlockBehaviour.AddPermutation(std::move(permutation)); }
		void SetResource(Backend::Rp::BlockResource&& resource) override { mBlockResource = std::move(resource); }
		void SetIdentifier(const Identifier& identifier) override { mBlockBehaviour.SetIdentifier(identifier); }
		Backend::Rp::BlockResource& GetResource() override { return mBlockResource; }
		void SetShouldUseBlockJson(bool shouldUseBlockJson) override { mShouldUseBlockJson = shouldUseBlockJson; }
		void SetCategory(const std::string& category) override { mBlockBehaviour.SetCategory(category); }

		void WriteToPacks(Backend::Bp::BehaviourPack& behaviourPack, Backend::Rp::ResourcePack& resourcePack) override
		{
			behaviourPack.AddBlockBehaviour(std::move(mBlockBehaviour));
			resourcePack.AddNewBlockJson(mBlockBehaviour.GetIdentifier().GetFullNamespace(), mBlockResource.GetTextureShortName());
			resourcePack.AddBlockResource(std::move(mBlockResource));
		}
	};
}