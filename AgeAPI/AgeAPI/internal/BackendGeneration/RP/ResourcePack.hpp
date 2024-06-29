#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/TerrainTextures.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/BlockJson.hpp>
#include <AgeAPI/internal/Addon/AddonSpecifics/Manifest.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Text/Text.hpp>

namespace AgeAPI::Backend::Rp
{
    class ResourcePack
    {
    private:
        Manifest mManifest{};
        TerrainTexture mTerrainTextureManager{};
        BlockJson mBlockJson{};
        Text mText{};
        friend class Addon;
    private:
        ResourcePack(const std::string& resource_pack_name, const std::string& atlasName = "atlas.terrain") : mTerrainTextureManager(atlasName, resource_pack_name) {}

        ResourcePack(const ResourcePack& other) = delete;
        ResourcePack(ResourcePack&& other) noexcept
            : mManifest(std::move(other.mManifest)), mTerrainTextureManager(std::move(other.mTerrainTextureManager)) {}

        void setManifest(Manifest& manifest) { mManifest = std::move(manifest); }
        void buildResourcePack(const std::filesystem::path& outputDir, bool Manifest) const;
        void writeTerrainTextures(const std::filesystem::path& outputBase) const;
        void writeBlockJson(const std::filesystem::path& outputBase) const;
        void writeTexts(const std::filesystem::path& outputBase) const;
    public:

        const Manifest& GetManifest() const { return mManifest; }
        Manifest& GetManifest() { return mManifest; }

        void AddBlockResource(BlockResource&& blockResource) { mTerrainTextureManager.AddTexture(std::move(blockResource)); }
        void AddBlockJson(const BlockJson& blockJson) { mBlockJson = blockJson; }
        void AddBlockJson(BlockJson&& blockJson) { mBlockJson = std::move(blockJson); }
        void AddNewBlockJson(const std::string& key, const BlockJsonEntry& blockJsonEntry) { mBlockJson.AddEntry(key, blockJsonEntry); }

        // should this return a reference to the language?
        void AddNewLanguage(const std::string& code, const std::string& name, const std::optional<Font>& font = std::nullopt)
        {
            if (mText.GetLanguage(code).has_value()) return;
            mText.AddLanguage(std::make_unique<Language>(code, name, NonOwningPtr{ &mText }, font));
        }

        std::optional<NonOwningPtr<Language>> GetLanguage(const std::string& code) { return mText.GetLanguage(code); }

        void AddText(const Text& text) { mText = text; }
        void AddText(Text&& text) { mText = std::move(text); }
        // TODO: This is slow...
        ErrorString AddNewLocalization(const std::string&& key, LocalizationEntry& value, const std::string& code)
        {
            auto lang = mText.GetLanguage(code);
            if (!lang.has_value()) return "Text: Not a valid language, try registering one first..."; // TODO: Make this better



            value.mLanguage = lang.value();
            mText.AddEntry(key, value);
            return ErrorString();
        }

    };
}