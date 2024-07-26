#pragma once
#include <AgeData/BlockComponents.hpp>
#include <AgeAPI/internal/Addon/AddonFeatures/Block.hpp>

namespace AgeData
{
	/*using namespace AgeAPI;
	class Block : public AgeAPI::AddonFeatures::IBlock
	{
	public:
		using Bhp = Backend::Bp::BlockBehaviour;
		using Rhp = Backend::Rp::BlockResource;

	private:
		Backend::Bp::BlockBehaviour mBehaviourHandler;
		Backend::Rp::BlockResource mResourceHandler;
	public:
		Block(const Identifier& identifier, SemanticVersion formatVersion = { 1,21, 0 }) : mBehaviourHandler(identifier, formatVersion), mResourceHandler(), IBlock() {}
		virtual ~Block() = default;

		auto& GetBehaviourHandler() { return mBehaviourHandler; }
		const auto& GetBehaviourHandler() const { return mBehaviourHandler; }
		auto& GetResourceHandler() { return mResourceHandler; }
		const auto& GetResourceHandler() const { return mResourceHandler; }

		ErrorString AddBlockComponent(NonOwningPtr<Addon> addon, std::unique_ptr<Components::BlockComponentBase>& component) override
		{
			return mBehaviourHandler.AddBlockComponent(addon, std::move(component));
		}
		Block& AddBlockComponent(NonOwningPtr<Addon> addon, std::unique_ptr<Components::BlockComponentBase>& component, ErrorString& err)
		{
			err = AddBlockComponent(addon, component);
			return *this;
		}
		void AddPermutation(Backend::Permutation&& permutation) override
		{
			mBehaviourHandler.AddPermutation(std::move(permutation));
		}
		void AddPermutation(const Backend::Permutation& permutation) override
		{
			auto copy = permutation;
			AddPermutation(std::move(copy));
		}
		Block& AddPermutation(Backend::Permutation&& permutation, ErrorString& err)
		{
			AddPermutation(std::move(permutation));
			return *this;
		}
		Block& AddPermutation(const Backend::Permutation& permutation, ErrorString& err)
		{
			auto copy = permutation;
			AddPermutation(std::move(copy));
			return *this;
		}

		void SetIdentifier(const Identifier& identifier) override
		{
			mBehaviourHandler.SetIdentifier(identifier);
			mResourceHandler.SetIdentifier(identifier);
		}
		Block& SetIdentifier(const Identifier& identifier, ErrorString& err)
		{
			SetIdentifier(identifier);
			return *this;
		}
		void SetCategory(const MenuCategory& category) override
		{
			mBehaviourHandler.SetCategory(category);
		}
		Block& SetCategory(const MenuCategory& category, ErrorString& err)
		{
			SetCategory(category);
			return *this;
		}

		void AddTexture(Backend::Rp::TextureSide side, const Backend::Rp::BlockResourceElement& element) override
		{
			mResourceHandler.AddTexture(side, element);
		}
		Block& AddTexture(Backend::Rp::TextureSide side, const Backend::Rp::BlockResourceElement& element, ErrorString& err)
		{
			AddTexture(side, element);
			return *this;
		}

		bool HasGeo() const { return mResourceHandler.HasGeo(); }
		bool HasAllSideTexture() const { return mResourceHandler.HoldsSingleTexture(); }
		const Rhp::GeoType::value_type& GetGeo() const { return mResourceHandler.GetGeo(); }
		const Rhp::TextureStore& GetTextures() const { return mResourceHandler.GetTextures(); }
		const Backend::Rp::BlockResourceElement& GetOverallTexture() const { return mResourceHandler.GetOverallTexture(); }
		const Rhp::MultiTextureStore& GetMultiTextures() const { return mResourceHandler.GetMultiTextures(); }
		void BindToPacks(Backend::Bp::BehaviourPack& bp, Backend::Rp::ResourcePack& rp) override
		{
			bp.AddBlockBehaviour(mBehaviourHandler);
			rp.BindBlockResource(mResourceHandler);
		}

		void SetGeo(NonOwningPtr<Addon> addon, const Backend::Rp::Geometry& geo, const Backend::Rp::MaterialInstance& mat = {}) override;

	};*/

}