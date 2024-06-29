#pragma once
#include <format>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <AgeAPI/internal/Types.hpp>

#include "AgeAPI/internal/BackendGeneration/RP/Textures/Texture.hpp"

#ifndef AGE_SORT_LOCALIZATION_ENTERY_KEYS
#define AGE_SORT_LOCALIZATION_ENTERY_KEYS 1
#endif 

namespace AgeAPI::Backend::Rp
{


    class Font
    {
    public:
        Font() = default;
        ~Font() = default;
        Font(Font&& other) noexcept = default;
        Font(const Font& other) = default;
        Font& operator=(const Font& other) = default;
        Font& operator=(Font&& other) noexcept = default;

        const std::unordered_map<std::uint8_t, Texture>& GetGlyphs() const { return mGlyphs; }
        void AddGlyph(std::uint8_t unicode_prefix, const Texture& texture) { mGlyphs[unicode_prefix] = texture; }

    private:
        std::unordered_map<std::uint8_t, Texture> mGlyphs{}; // TODO: Convert this into a texture atlas
    };


    class Text
    {
    public:

        class Language : std::pair<std::string, std::string>
        {
        public:
            using LocalizationString = std::string;

            /// An entry in the localization file
            class LocalizationEntry : public std::pair<std::string, std::string> {
            public:
                friend class Language;
                friend class ResourcePack;
                using line_str = std::string;
                using comment_str = std::string;
                using pair = std::pair<line_str, comment_str>;
                using language_ptr = NonOwningPtr<Text::Language>;
                // Constructor
                LocalizationEntry(const std::string& line, const std::string& comment) : pair(line, comment) {}

                // Default Constructor and Destructor
                LocalizationEntry() = default;
                ~LocalizationEntry() = default;
                // Copy Constructor and Move Constructor
                LocalizationEntry(const LocalizationEntry& other) = default;
                LocalizationEntry(LocalizationEntry&& other) noexcept = default;
                // Copy Assignment and Move Assignment
                LocalizationEntry& operator=(const LocalizationEntry& other) = default;
                LocalizationEntry& operator=(LocalizationEntry&& other) noexcept = default;

                // Getters
                line_str GetLine() const { return first; }
                comment_str GetComment() const { return second; }

                // Setters 
                void SetLine(const std::string& line) { first = line; }
                void SetComment(const std::string& comment) { second = comment; }

                language_ptr GetLanguage() const { return mLanguage; }
            private:
                void SetLanguage(language_ptr language) { mLanguage = language; }
                language_ptr mLanguage = nullptr;
            };



            // Useful when writing the language_ptr files
            friend class ResourcePack;


            Language() = default; // Default Constructor
            ~Language() = default; // Destructor
            Language(const Language& other) = default; // Copy Constructor
            Language(Language&& other) noexcept = default; // Move Constructor
            Language& operator=(const Language& other) = default; // Copy Assignment
            Language& operator=(Language&& other) noexcept = default; // Move Assignment

            inline const std::string& GetName() const { return first; }
            inline const std::string& GetCode() const { return second; }
            inline std::optional<Font>& GetFont() { return mFont; }

            void SetFont(Font&& font) { mFont.emplace(std::move(font)); }
            void SetFont(const Font& font) { mFont.emplace(font); }
            // TODO: When language_ptr is already defined do nothing
            void AddEntry(const LocalizationString& key, LocalizationEntry& value)
            {
                // Set the language_ptr of the value to this as it is now valid 
                value.mLanguage = NonOwningPtr(this);
                this->mText->AddEntry(key, value);
            }



            Language(const std::string& code, const std::string& name, NonOwningPtr<Text> text,const std::optional<Font>& font) : std::pair<std::string, std::string>(name, code), mText(text),mFont(font) {}
        private:
            // This Constructor is private as it is only used by the ResourcePack class when adding a new language_ptr
            std::optional<Font> mFont = std::nullopt;
            NonOwningPtr<Text> mText = nullptr;
            mutable std::stringstream mOutFile{}; // This needs to be mutable as it is used in a const context within ResourcePack.cpp
        };


        using entries_map = std::unordered_multimap<Language::LocalizationString, Language::LocalizationEntry>;
        using language_un_ptr = std::unique_ptr<Language>;
        using languages_vec = std::vector<language_un_ptr>;
        using global_font_opt = std::optional<Font>;



        Text() = default;
        ~Text() = default;

        Text(Text&& other) noexcept = default;
        Text(const Text& other) = default;
        Text& operator=(const Text& other) = default;
        Text& operator=(Text&& other) noexcept = default;


        void AddLanguage(std::unique_ptr<Language> language) { mLanguages.emplace_back(std::move(language)); }

        const languages_vec& GetLanguages() const { return mLanguages; }
        const entries_map& GetEntries() const { return mEntries; }
        const global_font_opt& GetGlobalFont() const { return mGlobalFont; }
        /// Iters through all the languages and returns the language_ptr with the matching code
        std::optional <NonOwningPtr<Language>> GetLanguage(const std::string& code) const
        {
            for (auto& language : mLanguages)
            {
                if (language.get()->GetCode() == code) return std::make_optional(NonOwningPtr(&*language.get()));
            }
            return std::nullopt;
        }
        // Setters
        void SetGlobalFont(const Font&& font) { mGlobalFont.emplace(font); }

        void AddEntry(const Language::LocalizationString& key, const Language::LocalizationEntry& val)
        {
            // TODO: Do validation here when it's added to the map rather than when its generated
            mEntries.emplace(std::make_pair(key, val));
        }

    private:
        entries_map mEntries{};
        languages_vec mLanguages{};
        global_font_opt  mGlobalFont = std::nullopt;
    };


    using Language = Text::Language;
    using LocalizationString = Text::Language::LocalizationString;
    using LocalizationEntry = Text::Language::LocalizationEntry;







    /* class LocalizationEntry : public std::pair< LocalizationString, LocalizationEntry>
     {
         LocalizationEntry(LocalizationString& key, LocalizationEntry& value) : std::pair< LocalizationString, LocalizationEntry>(key, value) {}
         LocalizationEntry(LocalizationEntry&& other) noexcept = default;
         LocalizationEntry(const LocalizationEntry& other) = default;
         ~LocalizationEntry() = default;

     };*/



     /// Formatting codes
     /// https://minecraft.wiki/w/Formatting_codes
    enum class FormattingCode : char
    {
        Black = '0',            // §0
        Navy = '1',             // §1
        Green = '2',        // §2
        DarkCyan = '3',         // §3
        Red = '4',          // §4
        Magenta = '5',          // §5
        Orange = '6',             // §6
        // They are different colors: https://www.janepack.net/blog/grey-or-gray/27/2/2014
        Grey = '7',             // §7
        Gray = '8',         // §8
        Blue = '9',             // §9
        Lime = 'a',            // §a
        Aqua = 'b',             // §b
        Salmon = 'c',              // §c
        Pink = 'd',             // §d
        Yellow = 'e',           // §e
        White = 'f',            // §f
        Minecoin = 'g',     // §g
        Quartz = 'h',        // §h
        Iron = 'i',            // §i
        Coal = 'j',            // §j
        Redstone = 'm',      // §m
        Copper = 'n',           // §n
        Gold = 'p',            // §p
        Emerald = 'q',          // §q
        Diamond = 's',         // §s
        Lapis = 't',             // §t
        Amethyst = 'u',         // §u
        Bold = 'l',             // §l
        Italic = 'o',           // §o
        /// Obfuscated
        MagicalText = 'k',       // §k
        Reset = 'r'             // §r
    };






    enum class LanguageSubstitutions
    {
        /// %s





    };







}

//template <>
//struct std::formatter<AgeAPI::Backend::Rp::FormattingCode> : std::formatter<std::string> {
//    auto format(AgeAPI::Backend::Rp::FormattingCode p, std::format_context& ctx) const {
//        return formatter<std::string>::format(
//            std::format("\u00A7{}", p), ctx);
//    }
//};