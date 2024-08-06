#include "WoodSet.hpp"
#include <AgeAPI/internal/Addon/Addon.hpp>
namespace AgeData::PreSets::WoodSet
{
    using namespace AgeAPI::AddonFeatures;
    using namespace AgeAPI;
    using namespace AgeData::BlockComponents;

    static Block CreatePlank(SetConfig& plankConfig, NonOwningPtr<Addon> addon)
    {
        return Block::MakeBlock(
            std::format("{}_planks", plankConfig.mBaseIdentifier),
            { 1,21,2 },
            MenuCategory("nature", ItemGroup::itemGroup_name_planks)
        )
            .value()
            .AddComponent(Flammable{ plankConfig.mFlammableData }, true, addon)
            .AddComponent(DestructibleByMining{ plankConfig.mSecondsToDestroy }, false, addon)
            .AddComponent(Tag{ "minecraft:wood_tier_destructible" }, false, addon)
            .AddComponent(Tag{ "minecraft:is_axe_item_destructible" }, false, addon)
            .AddComponent(Tag{ "wood" }, false, addon)
            .SetTexture(std::move(plankConfig.mPlankConfig.value().mTexture));
    }

    static Block CreateLog(SetConfig& config, NonOwningPtr<Addon> addon)
    {
        return Block::MakeBlock(
            std::format("{}_log", config.mBaseIdentifier),
            { 1, 21, 2 },
            MenuCategory("nature", ItemGroup::itemGroup_name_log)
        )
            .value()
            .AddComponent(Flammable{ config.mFlammableData }, true, addon)
            .AddComponent(DestructibleByMining{ config.mSecondsToDestroy }, false, addon)
            .AddComponent(Tag{ "minecraft:wood_tier_destructible" }, false, addon)
            .AddComponent(Tag{ "minecraft:is_axe_item_destructible" }, false, addon)
            .AddComponent(Tag{ "wood" }, false, addon)
            .AddComponent(Tag{ "log" }, false, addon)
            .AddComponent(
                LogRotation{},
                false,
                addon
            )
            .SetTexture(
                std::move(config.mLogConfig.value().mEndTexture.mTexture.value()),
                config.mLogConfig.value().mEndTexture.mTextureAlias,
                Backend::Rp::TextureSide::up,
                config.mLogConfig.value().mEndTexture.mPathFromBase
            )
            .SetTexture(
                config.mLogConfig.value().mEndTexture.mTextureAlias,
                Backend::Rp::TextureSide::down,
                config.mLogConfig.value().mEndTexture.mPathFromBase
            )
            .SetTexture(std::move(config.mLogConfig.value().mSideTexture));
    }

    static Block CreateLogStripped(SetConfig& config, NonOwningPtr<Addon> addon)
    {
        return Block::MakeBlock(
            std::format("{}_log_stripped", config.mBaseIdentifier),
            { 1, 21, 2 },
            MenuCategory("nature", ItemGroup::itemGroup_name_log)
        )
            .value()
            .AddComponent(Flammable{ config.mFlammableData }, true, addon)
            .AddComponent(DestructibleByMining{ config.mSecondsToDestroy }, false, addon)
            .AddComponent(Tag{ "minecraft:wood_tier_destructible" }, false, addon)
            .AddComponent(Tag{ "minecraft:is_axe_item_destructible" }, false, addon)
            .AddComponent(Tag{ "wood" }, false, addon)
            .AddComponent(Tag{ "log" }, false, addon)
            .AddComponent(
                LogRotation{},
                false,
                addon
            )
            .SetTexture(
                std::move(config.mStrippedConfig.value().mEndTexture.mTexture.value()),
                config.mStrippedConfig.value().mEndTexture.mTextureAlias,
                Backend::Rp::TextureSide::up,
                config.mStrippedConfig.value().mEndTexture.mPathFromBase
            )
            .SetTexture(
                config.mStrippedConfig.value().mEndTexture.mTextureAlias,
                Backend::Rp::TextureSide::down,
                config.mStrippedConfig.value().mEndTexture.mPathFromBase
            )
            .SetTexture(std::move(config.mStrippedConfig.value().mSideTexture));
    }

    WoodenSet CreateWoodSet(SetConfig&& config, NonOwningPtr<AgeAPI::Addon> addon)
    {
        if (!addon)
            addon = Addon::GetStaticInstance();
        WoodenSet set{};
        if (config.mPlankConfig.has_value())
            set.mPlanks = CreatePlank(config, addon);
        if (config.mLogConfig.has_value())
            set.mLog = CreateLog(config, addon);
        if (config.mStrippedConfig.has_value())
            set.mStrippedLog = CreateLogStripped(config, addon);
        return set;

    }
    void WoodenSet::Register(NonOwningPtr<Addon> addon)
    {
        if (!addon)
            addon = Addon::GetStaticInstance();
        if (mPlanks.has_value())
            addon->AddBlock(mPlanks.value());
        if (mLog.has_value())
            addon->AddBlock(mLog.value());
        if (mStrippedLog.has_value())
            addon->AddBlock(mStrippedLog.value());
    }
}
