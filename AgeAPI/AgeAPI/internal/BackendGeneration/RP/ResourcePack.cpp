#include "ResourcePack.hpp"
#include <fstream>


namespace AgeAPI::Backend::Rp
{
    void ResourcePack::buildResourcePack(const std::filesystem::path& outputDir, bool Manifest) const
    {
        std::filesystem::create_directories(outputDir);

        writeTerrainTextures(outputDir);
        writeBlockJson(outputDir);
        writeTexts(outputDir);
        if (!Manifest)
            return;

        // Write manifest
        auto ManifestDocumentExpected = mManifest.WriteToDocument();
        if (!ManifestDocumentExpected.has_value())
            throw std::runtime_error(ManifestDocumentExpected.error().GetAsString());
        rapidjson::Document& ManifestDocument = ManifestDocumentExpected.value();
        std::filesystem::path ManifestPath = outputDir / "manifest.json";
        std::ofstream ManifestFile(ManifestPath);
        if (!ManifestFile.is_open())
            throw std::runtime_error("Failed to open manifest file for writing");
        rapidjson::OStreamWrapper ManifestStream(ManifestFile);
        rapidjson::Writer<rapidjson::OStreamWrapper> ManifestWriter(ManifestStream);
        ManifestDocument.Accept(ManifestWriter);
        ManifestStream.Flush();
        ManifestFile.close();


    }

    void ResourcePack::writeTerrainTextures(const std::filesystem::path& outputBase) const
    {
        std::filesystem::path TexturesPath = outputBase / "textures";
        std::filesystem::create_directories(TexturesPath);

        auto TerrainTexturesDocumentExpected = mTerrainTextureManager.WriteToDocument();
        if (!TerrainTexturesDocumentExpected.has_value())
            throw std::runtime_error(TerrainTexturesDocumentExpected.error().GetAsString());
        rapidjson::Document& TerrainTexturesDocument = TerrainTexturesDocumentExpected.value();
        std::filesystem::path TerrainTexturesPath = TexturesPath / "terrain_texture.json";
        std::ofstream TerrainTexturesFile(TerrainTexturesPath);
        if (!TerrainTexturesFile.is_open())
            throw std::runtime_error("Failed to open terrain texture file for writing");
        rapidjson::OStreamWrapper TerrainTexturesStream(TerrainTexturesFile);
        rapidjson::Writer<rapidjson::OStreamWrapper> TerrainTexturesWriter(TerrainTexturesStream);
        TerrainTexturesDocument.Accept(TerrainTexturesWriter);
        TerrainTexturesStream.Flush();
        TerrainTexturesFile.close();
        mTerrainTextureManager.Write(outputBase);


    }

    void ResourcePack::writeBlockJson(const std::filesystem::path& outputBase) const
    {
        std::filesystem::path BlockJsonPath = outputBase / "blocks.json";
        std::ofstream BlockJsonFile(BlockJsonPath);
        if (!BlockJsonFile.is_open())
            throw std::runtime_error("Failed to open block json file for writing");
        rapidjson::OStreamWrapper BlockJsonStream(BlockJsonFile);
        rapidjson::Writer<rapidjson::OStreamWrapper> BlockJsonWriter(BlockJsonStream);
        auto BlockJsonDocumentExpected = mBlockJson.BuildBlockJsonDocument();
        if (!BlockJsonDocumentExpected.has_value())
            throw std::runtime_error(BlockJsonDocumentExpected.error().GetAsString());
        BlockJsonDocumentExpected.value().Accept(BlockJsonWriter);
        BlockJsonStream.Flush();
        BlockJsonFile.close();

    }

    void ResourcePack::writeTexts(const std::filesystem::path& outputBase) const
    {
        // Set up the paths
        std::filesystem::path TextsPath = outputBase / "texts";

        std::filesystem::create_directories(TextsPath);

        // Fonts

       /* for (const Font& font : mText.GetFonts())
        {
            std::filesystem::path fontPath;
            if (auto lang = font.GetLanguage())

            else
                fontPath = TextsPath / "font";

            std::filesystem::create_directories(fontPath);

            for ( const auto& [unicode, texture] : font.GetGlyphs())
            {
                std::filesystem::path texturePath = fontPath / std::format("{}_glyph.png", unicode);
                texture.FinalizeAndWrite(texturePath.string());
            }
        }*/


        if (mText.GetGlobalFont().has_value())
        {
            std::filesystem::path fontPath = TextsPath / "font";
            if (!std::filesystem::create_directories(fontPath))
            {
                throw std::runtime_error(std::format("Text: Failed to create {} directory", fontPath.string()));
            }
            // Write the glyphs to the disk
            for (const auto& [unicode, texture] : mText.GetGlobalFont().value().GetGlyphs())
            {
                texture.FinalizeAndWrite((fontPath / std::format("{}_glyph.png", unicode)).string());
            }
        }




        if (!mText.GetEntries().empty())
        { // Check file operations before even we start the generation
            std::filesystem::path languagesPath = TextsPath / "languages_vec.json";
            std::filesystem::path languageNamesPath = TextsPath / "language_names.json";

            std::ofstream languagesFile(languagesPath);
            std::ofstream languageNamesFile(languageNamesPath);

            // check if the files are open
            if (!languagesFile.is_open())
                throw std::runtime_error(std::format("Text: Failed to open {} file for writing", languagesPath.string()));

            if (!languageNamesFile.is_open())
                throw std::runtime_error(std::format("Text: Failed to open {} file for writing", languageNamesPath.string()));

            if (languagesFile.fail())
                throw std::runtime_error(std::format("Text: Failed to write to {} file", languagesPath.string()));

            if (languageNamesFile.fail())
                throw std::runtime_error(std::format("Text: Failed to write to {} file", languageNamesPath.string()));

            // Set up the json
            rapidjson::Document doc{};
            JsonProxy proxy{ doc, doc.GetAllocator() };           // TODO: global json allocator for the whole project

            // Write the languages_vec.json and language_names.json

            rapidjson::Value languages(rapidjson::kArrayType);
            rapidjson::Value language_names(rapidjson::kArrayType);

            // Loop through the entries_map and write the files to each language_ptr
            for (const auto& [key, value] : mText.GetEntries()) 
            { // Append the localization information
                // Microsoft requires CRLF for new lines and a tab before the comment_str
                value.GetLanguage()->mOutFile << std::format("{} = {}{}\r\n", key, value.GetLine(), value.GetComment().empty() ? "" : std::format("\t##{}",value.GetComment()));
            }


            for (const auto& language : mText.GetLanguages()) 
            {   // Check if there is any language_ptr data to write
                auto langFileContent = language->mOutFile.str();

                // if there is no language_ptr data, skip the language_ptr and continue to the next.
                if (langFileContent.empty()) continue;


                if (language->GetFont().has_value())
                {
                    std::filesystem::path fontPath = TextsPath / "font";
                    if (!std::filesystem::create_directories(fontPath))
                    {
                        throw std::runtime_error(std::format("Text: Failed to create {} directory", fontPath.string()));
                    }
                    // Write the glyphs to the disk
                    for (const auto& [unicode, texture] : language->GetFont().value().GetGlyphs())
                    {
                        texture.FinalizeAndWrite((fontPath / std::format("{:X}_glyph.png", unicode)).string());
                    }
                }




                // Make path and check the file system to see if the file is open
                std::filesystem::path langPath = TextsPath / std::format("{}.lang", language->GetCode());
                std::ofstream langFile(langPath);

                if (!langFile.is_open())
                    throw std::runtime_error(std::format("Text: Failed to open {} file for writing", langPath.string()));
                if (langFile.fail())
                    throw std::runtime_error(std::format("Text: Failed to write to {} file", langPath.string()));

                // Write to the language_ptr file
                langFile << langFileContent;

                // Append the language_ptr to the languages_vec.json file
                languages.GetArray().PushBack(rapidjson::StringRef(language->GetCode()), proxy.mAllocator);

                // Append the language_ptr to the language_names.json file
                rapidjson::Value lang_name_pair(rapidjson::kArrayType);

                // Get the language_ptr code and name and push it to the array of language_ptr names
                auto arr = lang_name_pair.GetArray();
                arr.PushBack(rapidjson::StringRef(language->GetCode()), proxy.mAllocator); // TODO: Make string ref type conversion
                arr.PushBack(rapidjson::StringRef(language->GetName()), proxy.mAllocator);
                language_names.GetArray().PushBack(lang_name_pair, proxy.mAllocator);

                // Write the language_ptr file and close it
                langFile << language->mOutFile.str();
                langFile.close();
            }

            // TODO: If the language_ptr json is empty, do not write the file

            // write the languages_vec.json file
            rapidjson::OStreamWrapper languagesStream(languagesFile);
            rapidjson::Writer languagesWriter(languagesStream);
            languages.Accept(languagesWriter);
            languagesStream.Flush();
            languagesFile.close();

            // write the language_names.json file
            rapidjson::OStreamWrapper languageNamesStream(languageNamesFile);
            rapidjson::Writer languageNamesWriter(languageNamesStream);
            language_names.Accept(languageNamesWriter);
            languageNamesStream.Flush();
            languageNamesFile.close();

        }

    }
}
