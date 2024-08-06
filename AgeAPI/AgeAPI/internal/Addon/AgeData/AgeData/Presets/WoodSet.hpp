#include <AgeAPI/internal/Types.hpp>
#include <optional>
#include <AgeAPI/internal/Addon/AddonFeatures/Block.hpp>
#include <AgeData/BlockComponents.hpp>

namespace AgeData::PreSets::WoodSet
{
	struct SetConfig
	{
		struct LogConfig
		{
			AgeAPI::Backend::Rp::BlockResourceElement mEndTexture;
			AgeAPI::Backend::Rp::BlockResourceElement mSideTexture;
		};
		using StrippedConfig = LogConfig;

		struct PlankConfig
		{
			AgeAPI::Backend::Rp::BlockResourceElement mTexture;
		};
		using FlammableData = AgeData::BlockComponents::Flammable::FlammableData;
		FlammableData mFlammableData{ 2, 5 };
		float mSecondsToDestroy{ 1.f };
		std::optional<LogConfig> mLogConfig{std::nullopt};
		std::optional<StrippedConfig> mStrippedConfig{ std::nullopt };
		std::optional<PlankConfig> mPlankConfig{ std::nullopt };
		std::string mBaseIdentifier{};

		constexpr SetConfig(
			const std::string& baseIdentifer,
			const FlammableData& mFlammableData = { 2,5 },
			float mSecondsToDestroy = 1.f,
			std::optional<PlankConfig>&& mPlankConfig = std::nullopt,
			std::optional<LogConfig>&& mLogConfig = std::nullopt,
			std::optional<StrippedConfig>&& mStrippedConfig = std::nullopt
		) : mFlammableData(mFlammableData), mSecondsToDestroy(mSecondsToDestroy),
			mLogConfig(std::move(mLogConfig)),
			mStrippedConfig(std::move(mStrippedConfig)),
			mPlankConfig(std::move(mPlankConfig)),
			mBaseIdentifier(baseIdentifer)
		{}
	};

	struct WoodenSet
	{
		std::optional<AgeAPI::AddonFeatures::Block> mPlanks{ std::nullopt };
		std::optional<AgeAPI::AddonFeatures::Block> mLog{ std::nullopt };
		std::optional<AgeAPI::AddonFeatures::Block> mStrippedLog{ std::nullopt };

		void Register(AgeAPI::NonOwningPtr<AgeAPI::Addon> = nullptr);
	};
	WoodenSet CreateWoodSet(SetConfig&& config, AgeAPI::NonOwningPtr<AgeAPI::Addon> addon = nullptr);

	template<typename a1>
		requires std::is_constructible_v<SetConfig, a1>
	WoodenSet CreateWoodSet(
		a1&& config,
		AgeAPI::NonOwningPtr<AgeAPI::Addon> addon = nullptr
	)
	{
		return CreateWoodSet(std::forward<a1>(config), addon);
	}
}