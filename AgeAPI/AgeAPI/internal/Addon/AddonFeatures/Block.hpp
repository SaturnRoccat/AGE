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
		virtual ErrorString AddBlockComponent(NonOwningPtr<Addon> addon, std::unique_ptr<Components::BlockComponentBase>& component) = 0;
		virtual void AddPermutation(Backend::Permutation&& permutation) = 0;
		virtual void AddPermutation(const Backend::Permutation& permutation) {
			auto copy = permutation;
			AddPermutation(std::move(copy));
		}
		virtual void SetIdentifier(const Identifier& identifier) = 0;
		virtual void SetCategory(const MenuCategory& category) = 0;
		virtual void AddTexture(Backend::Rp::TextureSide side, const Backend::Rp::BlockResourceElement& element) = 0;
		virtual void BindToPacks(Backend::Bp::BehaviourPack& bp, Backend::Rp::ResourcePack& rp) = 0;
		virtual void SetGeo(NonOwningPtr<Addon> addon, const Backend::Rp::Geometry& geo, const Backend::Rp::MaterialInstance& mat = {}) = 0;
		/// virtual void SetGeo(const Backend::Rp::Geometry& geo, const Backend::Rp::BlockResourceElement& texture) = 0;


	};

	
}