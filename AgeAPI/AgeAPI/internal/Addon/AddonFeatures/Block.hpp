#pragma once
#include <AgeAPI/internal/BackendGeneration/BP/Block/BlockBehaviour.hpp>
#include <AgeAPI/internal/BackendGeneration/BP/BehaviourPack.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/ResourcePack.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Block/BlockResource.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Models/Models.hpp>

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
		virtual void WriteToPacks(Backend::Bp::BehaviourPack& behaviourPack, Backend::Rp::ResourcePack& resourcePack) = 0;
		virtual void SetIdentifier(const Identifier& identifier) = 0;
		virtual void SetCategory(const std::string& category) = 0;
	};

	class Block : public IBlock
	{
	private:
	public:
	};
}