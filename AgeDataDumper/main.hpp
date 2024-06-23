#pragma once

#include <string>
#include <vector>
#include <map>
#include <variant>
#include <optional>
#include <memory>
#include <rapidjson/document.h>     

#include <any>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <print>
#include <unordered_map>
#include <variant>
#include <concepts>
#include <array>
#include <expected>
#include <memory>
#include <functional>
#include <optional>
#include <span>
#include<vector>
#include <string_view>

#include <string>
#include <vector>
#include <map>
#include <variant>
#include <optional>
#include <memory>
#include <rapidjson/document.h>

#ifndef RAPIDJSON_HAS_STDSTRING
#define RAPIDJSON_HAS_STDSTRING 1
#define RAPIDJSON_HAS_CXX11_RVALUE_REFS 1
#define RAPIDJSON_HAS_CXX11_NOEXCEPT 1
#define RAPIDJSON_HAS_CXX11_TYPETRAITS 1
#define RAPIDJSON_HAS_CXX11_RANGE_FOR 1
#define RAPIDJSON_HAS_CXX11_UNCAUGHT_EXCEPTION 1
#define RAPIDJSON_HAS_CXX11_USER_DEFINED_LITERALS 1
#define RAPIDJSON_HAS_CXX11_TEMPLATED_LITERALS 1
#define RAPIDJSON_HAS_CXX11_TYPENAME 1
#define RAPIDJSON_HAS_CXX11_EXPLICIT_CONVERSION 1
#endif

namespace JsonSchema
{



    class JsonSchema
    {
    public:
        JsonSchema() = default;
        ~JsonSchema() = default;

        void parse(const rapidjson::Value& value);

        const std::optional<std::string>& getId() const { return mId; }
        const std::optional<std::string>& getSchema() const { return mSchema; }
        const std::optional<std::string>& getRef() const { return mRef; }
        const std::optional<std::string>& getComment() const { return mComment; }
        const std::optional<std::string>& getTitle() const { return mTitle; }
        const std::optional<std::string>& getDescription() const { return mDescription; }
        const std::optional<std::variant<bool, std::string, double, std::vector<std::variant<bool, std::string, double>>>>& getDefault() const { return mDefault; }
        const std::optional<bool>& getReadOnly() const { return mReadOnly; }
        const std::optional<bool>& getWriteOnly() const { return mWriteOnly; }
        const std::optional<std::vector<std::variant<bool, std::string, double>>>& getExamples() const { return mExamples; }
        const std::optional<double>& getMultipleOf() const { return mMultipleOf; }
        const std::optional<double>& getMaximum() const { return mMaximum; }
        const std::optional<double>& getExclusiveMaximum() const { return mExclusiveMaximum; }
        const std::optional<double>& getMinimum() const { return mMinimum; }
        const std::optional<double>& getExclusiveMinimum() const { return mExclusiveMinimum; }
        const std::optional<size_t>& getMaxLength() const { return mMaxLength; }
        const std::optional<size_t>& getMinLength() const { return mMinLength; }
        const std::optional<std::string>& getPattern() const { return mPattern; }
        const std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>>& getItems() const { return mItems; }
        const std::optional<size_t>& getMaxItems() const { return mMaxItems; }
        const std::optional<size_t>& getMinItems() const { return mMinItems; }
        const std::optional<bool>& getUniqueItems() const { return mUniqueItems; }
        const std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>>& getContains() const { return mContains; }
        const std::optional<size_t>& getMaxProperties() const { return mMaxProperties; }
        const std::optional<size_t>& getMinProperties() const { return mMinProperties; }
        const std::optional<std::vector<std::string>>& getRequired() const { return mRequired; }
        const std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>>& getAdditionalProperties() const { return mAdditionalProperties; }
        const std::optional<std::map<std::string, std::shared_ptr<JsonSchema>>>& getDefinitions() const { return mDefinitions; }
        const std::optional<std::map<std::string, std::shared_ptr<JsonSchema>>>& getProperties() const { return mProperties; }
        const std::optional<std::map<std::string, std::shared_ptr<JsonSchema>>>& getPatternProperties() const { return mPatternProperties; }
        const std::optional<std::map<std::string, std::variant<std::shared_ptr<JsonSchema>, std::vector<std::string>>>>& getDependencies() const { return mDependencies; }
        const std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>>& getPropertyNames() const { return mPropertyNames; }
        const std::optional<std::variant<bool, std::string, double>>& getConst() const { return mConst; }
        const std::optional<std::vector<std::variant<bool, std::string, double>>>& getEnum() const { return mEnum; }
        const std::optional<std::variant<std::string, std::vector<std::string>>>& getType() const { return mType; }
        const std::optional<std::string>& getFormat() const { return mFormat; }
        const std::optional<std::string>& getContentMediaType() const { return mContentMediaType; }
        const std::optional<std::string>& getContentEncoding() const { return mContentEncoding; }
        const std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>>& getIf() const { return mIf; }
        const std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>>& getThen() const { return mThen; }
        const std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>>& getElse() const { return mElse; }
        const std::optional<std::vector<std::shared_ptr<JsonSchema>>>& getAllOf() const { return mAllOf; }
        const std::optional<std::vector<std::shared_ptr<JsonSchema>>>& getAnyOf() const { return mAnyOf; }
        const std::optional<std::vector<std::shared_ptr<JsonSchema>>>& getOneOf() const { return mOneOf; }
        const std::optional<std::shared_ptr<JsonSchema>>& getNot() const { return mNot; }

        // Setters
        void setId(const std::string& id) { mId = id; }
        void setSchema(const std::string& schema) { mSchema = schema; }
        void setRef(const std::string& ref) { mRef = ref; }
        void setComment(const std::string& comment) { mComment = comment; }
        void setTitle(const std::string& title) { mTitle = title; }
        void setDescription(const std::string& description) { mDescription = description; }
        void setDefault(const std::variant<bool, std::string, double, std::vector<std::variant<bool, std::string, double>>>& def) { mDefault = def; }
        void setReadOnly(bool readOnly) { mReadOnly = readOnly; }
        void setWriteOnly(bool writeOnly) { mWriteOnly = writeOnly; }
        void setExamples(const std::vector<std::variant<bool, std::string, double>>& examples) { mExamples = examples; }
        void setMultipleOf(double multipleOf) { mMultipleOf = multipleOf; }
        void setMaximum(double maximum) { mMaximum = maximum; }
        void setExclusiveMaximum(double exclusiveMaximum) { mExclusiveMaximum = exclusiveMaximum; }
        void setMinimum(double minimum) { mMinimum = minimum; }
        void setExclusiveMinimum(double exclusiveMinimum) { mExclusiveMinimum = exclusiveMinimum; }
        void setMaxLength(size_t maxLength) { mMaxLength = maxLength; }
        void setMinLength(size_t minLength) { mMinLength = minLength; }
        void setPattern(const std::string& pattern) { mPattern = pattern; }
        void setItems(const std::variant<std::shared_ptr<JsonSchema>, bool>& items) { mItems = items; }
        void setMaxItems(size_t maxItems) { mMaxItems = maxItems; }
        void setMinItems(size_t minItems) { mMinItems = minItems; }
        void setUniqueItems(bool uniqueItems) { mUniqueItems = uniqueItems; }
        void setContains(const std::variant<std::shared_ptr<JsonSchema>, bool>& contains) { mContains = contains; }
        void setMaxProperties(size_t maxProperties) { mMaxProperties = maxProperties; }
        void setMinProperties(size_t minProperties) { mMinProperties = minProperties; }
        void setRequired(const std::vector<std::string>& required) { mRequired = required; }
        void setAdditionalProperties(const std::variant<std::shared_ptr<JsonSchema>, bool>& additionalProperties) { mAdditionalProperties = additionalProperties; }
        void setDefinitions(const std::map<std::string, std::shared_ptr<JsonSchema>>& definitions) { mDefinitions = definitions; }
        void setProperties(const std::map<std::string, std::shared_ptr<JsonSchema>>& properties) { mProperties = properties; }
        void setPatternProperties(const std::map<std::string, std::shared_ptr<JsonSchema>>& patternProperties) { mPatternProperties = patternProperties; }
        void setDependencies(const std::map<std::string, std::variant<std::shared_ptr<JsonSchema>, std::vector<std::string>>>& dependencies) { mDependencies = dependencies; }
        void setPropertyNames(const std::variant<std::shared_ptr<JsonSchema>, bool>& propertyNames) { mPropertyNames = propertyNames; }
        void setConst(const std::variant<bool, std::string, double>& con) { mConst = con; }
        void setEnum(const std::vector<std::variant<bool, std::string, double>>& enu) { mEnum = enu; }
        void setType(const std::variant<std::string, std::vector<std::string>>& type) { mType = type; }
        void setFormat(const std::string& format) { mFormat = format; }
        void setContentMediaType(const std::string& contentMediaType) { mContentMediaType = contentMediaType; }
        void setContentEncoding(const std::string& contentEncoding) { mContentEncoding = contentEncoding; }
        void setIf(const std::variant<std::shared_ptr<JsonSchema>, bool>& ifVal) { mIf = ifVal; }
        void setThen(const std::variant<std::shared_ptr<JsonSchema>, bool>& thenVal) { mThen = thenVal; }
        void setElse(const std::variant<std::shared_ptr<JsonSchema>, bool>& elseVal) { mElse = elseVal; }
        void setAllOf(const std::vector<std::shared_ptr<JsonSchema>>& allOf) { mAllOf = allOf; }
        void setAnyOf(const std::vector<std::shared_ptr<JsonSchema>>& anyOf) { mAnyOf = anyOf; }
        void setOneOf(const std::vector<std::shared_ptr<JsonSchema>>& oneOf) { mOneOf = oneOf; }
        void setNot(const std::shared_ptr<JsonSchema>& notVal) { mNot = notVal; }


    private:
        template <typename T>
        std::optional<T> getOptional(const rapidjson::Value& value, const char* key) const;

        std::optional<std::string> mId{};
        std::optional<std::string> mSchema{};
        std::optional<std::string> mRef{};
        std::optional<std::string> mComment{};
        std::optional<std::string> mTitle{};
        std::optional<std::string> mDescription{};
        std::optional<std::variant<bool, std::string, double, std::vector<std::variant<bool, std::string, double>>>> mDefault{};
        std::optional<bool> mReadOnly{ false };
        std::optional<bool> mWriteOnly{ false };
        std::optional<std::vector<std::variant<bool, std::string, double>>> mExamples{};
        std::optional<double> mMultipleOf{};
        std::optional<double> mMaximum{};
        std::optional<double> mExclusiveMaximum{};
        std::optional<double> mMinimum{};
        std::optional<double> mExclusiveMinimum{};
        std::optional<size_t> mMaxLength{};
        std::optional<size_t> mMinLength{};
        std::optional<std::string> mPattern{};
        std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>> mItems{};
        std::optional<size_t> mMaxItems{};
        std::optional<size_t> mMinItems{};
        std::optional<bool> mUniqueItems{ false };
        std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>> mContains{};
        std::optional<size_t> mMaxProperties{};
        std::optional<size_t> mMinProperties{};
        std::optional<std::vector<std::string>> mRequired{};
        std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>> mAdditionalProperties{};
        std::optional<std::map<std::string, std::shared_ptr<JsonSchema>>> mDefinitions{};
        std::optional<std::map<std::string, std::shared_ptr<JsonSchema>>> mProperties{};
        std::optional<std::map<std::string, std::shared_ptr<JsonSchema>>> mPatternProperties{};
        std::optional<std::map<std::string, std::variant<std::shared_ptr<JsonSchema>, std::vector<std::string>>>> mDependencies{};
        std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>> mPropertyNames{};
        std::optional<std::variant<bool, std::string, double>> mConst{};
        std::optional<std::vector<std::variant<bool, std::string, double>>> mEnum{};
        std::optional<std::variant<std::string, std::vector<std::string>>> mType{};
        std::optional<std::string> mFormat{};
        std::optional<std::string> mContentMediaType{};
        std::optional<std::string> mContentEncoding{};
        std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>> mIf{};
        std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>> mThen{};
        std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>> mElse{};
        std::optional<std::vector<std::shared_ptr<JsonSchema>>> mAllOf{};
        std::optional<std::vector<std::shared_ptr<JsonSchema>>> mAnyOf{};
        std::optional<std::vector<std::shared_ptr<JsonSchema>>> mOneOf{};
        std::optional<std::shared_ptr<JsonSchema>> mNot{};
    };

    template <typename T>
    std::optional<T> JsonSchema::getOptional(const rapidjson::Value& value, const char* key) const
    {
        if (value.HasMember(key))
        {
            if constexpr (std::is_same_v<T, std::string>)
            {
                if (value[key].IsString())
                {
                    return value[key].GetString();
                }
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                if (value[key].IsBool())
                {
                    return value[key].GetBool();
                }
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                if (value[key].IsNumber())
                {
                    return value[key].GetDouble();
                }
            }
            else if constexpr (std::is_same_v<T, size_t>)
            {
                if (value[key].IsUint64())
                {
                    return static_cast<size_t>(value[key].GetUint64());
                }
            }
            else if constexpr (std::is_same_v<T, std::vector<std::string>>)
            {
                if (value[key].IsArray())
                {
                    std::vector<std::string> vec;
                    for (const auto& item : value[key].GetArray())
                    {
                        if (item.IsString())
                        {
                            vec.push_back(item.GetString());
                        }
                    }
                    return vec;
                }
            }
        }
        return std::nullopt;
    }

    void JsonSchema::parse(const rapidjson::Value& value)
    {
        mId = getOptional<std::string>(value, "$id");
        mSchema = getOptional<std::string>(value, "$schema");
        mRef = getOptional<std::string>(value, "$ref");
        mComment = getOptional<std::string>(value, "$comment");
        mTitle = getOptional<std::string>(value, "title");
        mDescription = getOptional<std::string>(value, "description");
        mReadOnly = getOptional<bool>(value, "readOnly");
        mWriteOnly = getOptional<bool>(value, "writeOnly");
        mExamples = getOptional<std::vector<std::variant<bool, std::string, double>>>(value, "examples");
        mMultipleOf = getOptional<double>(value, "multipleOf");
        mMaximum = getOptional<double>(value, "maximum");
        mExclusiveMaximum = getOptional<double>(value, "exclusiveMaximum");
        mMinimum = getOptional<double>(value, "minimum");
        mExclusiveMinimum = getOptional<double>(value, "exclusiveMinimum");
        mMaxLength = getOptional<size_t>(value, "maxLength");
        mMinLength = getOptional<size_t>(value, "minLength");
        mPattern = getOptional<std::string>(value, "pattern");
        mUniqueItems = getOptional<bool>(value, "uniqueItems");
        mMaxProperties = getOptional<size_t>(value, "maxProperties");
        mMinProperties = getOptional<size_t>(value, "minProperties");
        mRequired = getOptional<std::vector<std::string>>(value, "required");
        mFormat = getOptional<std::string>(value, "format");
        mContentMediaType = getOptional<std::string>(value, "contentMediaType");
        mContentEncoding = getOptional<std::string>(value, "contentEncoding");

        // For properties that require nested parsing or are complex types, handle accordingly
        if (value.HasMember("default"))
        {
            const auto& defaultValue = value["default"];
            if (defaultValue.IsBool())
            {
                mDefault = defaultValue.GetBool();
            }
            else if (defaultValue.IsString())
            {
                mDefault = defaultValue.GetString();
            }
            else if (defaultValue.IsNumber())
            {
                mDefault = defaultValue.GetDouble();
            }
            else if (defaultValue.IsArray())
            {
                std::vector<std::variant<bool, std::string, double>> vec;
                for (const auto& item : defaultValue.GetArray())
                {
                    if (item.IsBool())
                    {
                        vec.push_back(item.GetBool());
                    }
                    else if (item.IsString())
                    {
                        vec.push_back(item.GetString());
                    }
                    else if (item.IsNumber())
                    {
                        vec.push_back(item.GetDouble());
                    }
                }
                mDefault = vec;
            }
        }

        if (value.HasMember("items"))
        {
            const auto& itemsValue = value["items"];
            if (itemsValue.IsBool())
            {
                mItems = itemsValue.GetBool();
            }
            else if (itemsValue.IsObject())
            {
                auto schema = std::make_shared<JsonSchema>();
                schema->parse(itemsValue);
                mItems = schema;
            }
        }

        if (value.HasMember("contains"))
        {
            const auto& containsValue = value["contains"];
            if (containsValue.IsBool())
            {
                mContains = containsValue.GetBool();
            }
            else if (containsValue.IsObject())
            {
                auto schema = std::make_shared<JsonSchema>();
                schema->parse(containsValue);
                mContains = schema;
            }
        }

        if (value.HasMember("additionalProperties"))
        {
            const auto& additionalPropertiesValue = value["additionalProperties"];
            if (additionalPropertiesValue.IsBool())
            {
                mAdditionalProperties = additionalPropertiesValue.GetBool();
            }
            else if (additionalPropertiesValue.IsObject())
            {
                auto schema = std::make_shared<JsonSchema>();
                schema->parse(additionalPropertiesValue);
                mAdditionalProperties = schema;
            }
        }

        if (value.HasMember("definitions"))
        {
            std::map<std::string, std::shared_ptr<JsonSchema>> definitions;
            for (auto it = value["definitions"].MemberBegin(); it != value["definitions"].MemberEnd(); ++it)
            {
                auto schema = std::make_shared<JsonSchema>();
                schema->parse(it->value);
                definitions[it->name.GetString()] = schema;
            }
            mDefinitions = definitions;
        }

        if (value.HasMember("properties"))
        {
            std::map<std::string, std::shared_ptr<JsonSchema>> properties;
            for (auto it = value["properties"].MemberBegin(); it != value["properties"].MemberEnd(); ++it)
            {
                auto schema = std::make_shared<JsonSchema>();
                schema->parse(it->value);
                properties[it->name.GetString()] = schema;
            }
            mProperties = properties;
        }

        if (value.HasMember("patternProperties"))
        {
            std::map<std::string, std::shared_ptr<JsonSchema>> patternProperties;
            for (auto it = value["patternProperties"].MemberBegin(); it != value["patternProperties"].MemberEnd(); ++it)
            {
                auto schema = std::make_shared<JsonSchema>();
                schema->parse(it->value);
                patternProperties[it->name.GetString()] = schema;
            }
            mPatternProperties = patternProperties;
        }

        if (value.HasMember("dependencies"))
        {
            std::map<std::string, std::variant<std::shared_ptr<JsonSchema>, std::vector<std::string>>> dependencies;
            for (auto it = value["dependencies"].MemberBegin(); it != value["dependencies"].MemberEnd(); ++it)
            {
                if (it->value.IsArray())
                {
                    std::vector<std::string> depArray;
                    for (const auto& item : it->value.GetArray())
                    {
                        depArray.push_back(item.GetString());
                    }
                    dependencies[it->name.GetString()] = depArray;
                }
                else if (it->value.IsObject())
                {
                    auto schema = std::make_shared<JsonSchema>();
                    schema->parse(it->value);
                    dependencies[it->name.GetString()] = schema;
                }
            }
            mDependencies = dependencies;
        }

        if (value.HasMember("propertyNames"))
        {
            const auto& propertyNamesValue = value["propertyNames"];
            if (propertyNamesValue.IsBool())
            {
                mPropertyNames = propertyNamesValue.GetBool();
            }
            else if (propertyNamesValue.IsObject())
            {
                auto schema = std::make_shared<JsonSchema>();
                schema->parse(propertyNamesValue);
                mPropertyNames = schema;
            }
        }

        if (value.HasMember("const"))
        {
            const auto& constValue = value["const"];
            if (constValue.IsBool())
            {
                mConst = constValue.GetBool();
            }
            else if (constValue.IsString())
            {
                mConst = constValue.GetString();
            }
            else if (constValue.IsNumber())
            {
                mConst = constValue.GetDouble();
            }
        }

        if (value.HasMember("enum"))
        {
            std::vector<std::variant<bool, std::string, double>> enumValues;
            for (const auto& item : value["enum"].GetArray())
            {
                if (item.IsBool())
                {
                    enumValues.push_back(item.GetBool());
                }
                else if (item.IsString())
                {
                    enumValues.push_back(item.GetString());
                }
                else if (item.IsNumber())
                {
                    enumValues.push_back(item.GetDouble());
                }
            }
            mEnum = enumValues;
        }

        if (value.HasMember("type"))
        {
            const auto& typeValue = value["type"];
            if (typeValue.IsString())
            {
                mType = typeValue.GetString();
            }
            else if (typeValue.IsArray())
            {
                std::vector<std::string> typeArray;
                for (const auto& item : typeValue.GetArray())
                {
                    typeArray.push_back(item.GetString());
                }
                mType = typeArray;
            }
        }

        if (value.HasMember("if"))
        {
            const auto& ifValue = value["if"];
            if (ifValue.IsBool())
            {
                mIf = ifValue.GetBool();
            }
            else if (ifValue.IsObject())
            {
                auto schema = std::make_shared<JsonSchema>();
                schema->parse(ifValue);
                mIf = schema;
            }
        }

        if (value.HasMember("then"))
        {
            const auto& thenValue = value["then"];
            if (thenValue.IsBool())
            {
                mThen = thenValue.GetBool();
            }
            else if (thenValue.IsObject())
            {
                auto schema = std::make_shared<JsonSchema>();
                schema->parse(thenValue);
                mThen = schema;
            }
        }

        if (value.HasMember("else"))
        {
            const auto& elseValue = value["else"];
            if (elseValue.IsBool())
            {
                mElse = elseValue.GetBool();
            }
            else if (elseValue.IsObject())
            {
                auto schema = std::make_shared<JsonSchema>();
                schema->parse(elseValue);
                mElse = schema;
            }
        }

        if (value.HasMember("allOf"))
        {
            std::vector<std::shared_ptr<JsonSchema>> allOfArray;
            for (const auto& item : value["allOf"].GetArray())
            {
                auto schema = std::make_shared<JsonSchema>();
                schema->parse(item);
                allOfArray.push_back(schema);
            }
            mAllOf = allOfArray;
        }

        if (value.HasMember("anyOf"))
        {
            std::vector<std::shared_ptr<JsonSchema>> anyOfArray;
            for (const auto& item : value["anyOf"].GetArray())
            {
                auto schema = std::make_shared<JsonSchema>();
                schema->parse(item);
                anyOfArray.push_back(schema);
            }
            mAnyOf = anyOfArray;
        }

        if (value.HasMember("oneOf"))
        {
            std::vector<std::shared_ptr<JsonSchema>> oneOfArray;
            for (const auto& item : value["oneOf"].GetArray())
            {
                auto schema = std::make_shared<JsonSchema>();
                schema->parse(item);
                oneOfArray.push_back(schema);
            }
            mOneOf = oneOfArray;
        }

        if (value.HasMember("not"))
        {
            auto schema = std::make_shared<JsonSchema>();
            schema->parse(value["not"]);
            mNot = schema;
        }
    }



}
namespace SchemaDef {
    enum TypeDefError {
        InvalidMerge,
    };


    // FIXME: MAKE ALL OF THE THINGS BELOW USE std::move() instead of copying

    class DefinitionBase
    {
    public:
        virtual ~DefinitionBase() = default;

    private:
        virtual std::expected<DefinitionBase, TypeDefError> Merge(DefinitionBase other)
        {
            return std::unexpected(TypeDefError::InvalidMerge);
        };
    };


    class Definition : public DefinitionBase
    {
    // TODO: put a value using statement under     idk abt this
    public:
//        using 
        Definition() = default;
        Definition(std::string name, std::string documentation) : mName(name), mDocumentation(documentation) {}
        Definition(std::string name) : mName(name) {}

        inline std::string_view GetName() { return mName; }
        inline std::string_view GetDocumentation() { return mDocumentation; }
        //inline std::optional<DefinitionBase>& GetParent() { return mParent; }
        /// Add Documentation
        inline void AddDocumentation(std::string_view documentation) { mDocumentation += documentation; }
       /*virtual std::expected<DefinitionBase, TypeDefError> Merge(DefinitionBase& other) {
            return std::unexpected(TypeDefError::InvalidMerge);
        }*/
        //void SetOptional(bool optional) { mOptional = optional; }
        void SetName(std::string name) { mName = name; }
        //void AddParent(DefinitionBase& parent) { mParents.push_back(std::move(parent)); }

    private:


        std::string mName{};
        std::string mDocumentation{};
      
        //std::vector<DefinitionBase> mParents{};
    };

    class SubDefinitionList
    {
    public:
        SubDefinitionList(std::initializer_list<Definition> schemas) : mSchemas(schemas) {}
        ~SubDefinitionList() = default;
        void AddDef(Definition&& schema) { mSchemas.push_back(std::move(schema)); }
        void AddDef(const Definition& schema) { mSchemas.push_back(schema); }
        std::span<Definition> GetSchemas() { return mSchemas; }
    private:
        std::vector<Definition> mSchemas{};
    };

    class SubDefinition
    {
    public:
        explicit SubDefinition(Definition&& schema) : mDef(std::move(schema)) {}
        explicit SubDefinition(const Definition& schema) : mDef(schema) {}
        SubDefinition() = default;
        ~SubDefinition() = default;
        void SetSubDef(Definition& schema) { mDef = std::move(schema); }
        Definition& GetSubDef() { return mDef; }
    private:
        Definition mDef{};
    };


    class NamespacePath
    {
    public:
        NamespacePath() = default;
        ~NamespacePath() = default;

        void AddPath(std::string path) { mPath.push_back(path); }
        std::string GetPath(std::string& sep)
        {
            std::string fullPath;
            for (auto& path : mPath)
            {
                fullPath += path + sep;
            }
            return fullPath;
        }

    private:
        std::vector <std::string> mPath;

    };

    class Namespace : public Definition,public SubDefinitionList
    {
    public:
        Namespace() = default;
        Namespace(std::string name, std::initializer_list<Definition> schemas) :Definition(name), SubDefinitionList(schemas) {}
        ~Namespace() = default;
    };

    class Alias : public Definition, SubDefinition { public: Alias(std::string&& name, std::string&&, Definition&& schema) :Definition(name), SubDefinition(schema) {} };
    template <typename T=unsigned int>
    class Field : public Definition
    {
    public:
        Field(std::string&& name, std::string&& doc) :Definition(name, doc) {}
        Field(std::string&& name, std::string&& doc,  T&& value) :Definition(name, doc), mValue(value) {}
        T& GetValue() const { return mValue; }
        void SetValue(T&& value) { mValue = std::move(value); }
    private:
        T mValue;
    };

    template <typename T=unsigned int>
    class Enum : public Definition
    {
    public:
        Enum(std::string&& name, std::string&& doc) :Definition(name, doc) {}
        Enum(std::string&& name, std::string&& doc, std::initializer_list<Field<T>> fields) :Definition(name, doc), mFields(fields) {}
        ~Enum() = default;

        std::vector <Field<T>>& GetFields() const { return mFields; }
        void AddField(Field<T>&& field) { mFields.push_back(std::move(field)); }
    private:
        std::vector <Field<T>> mFields{};
    };

    using Numeric = std::variant<std::int8_t, std::int16_t, std::int32_t, std::int64_t, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t, float, double>;

    class Number : public Definition
    {
    public:
        Number() = default;
        Number(const Numeric min, const  Numeric max, const std::string&& name) : mMin(min), mMax(max) {}
        ~Number() = default;

        // Getters and Setters
        Numeric GetMin() const { return mMin; }
        Numeric GetMax() const { return mMax; }
        void SetMin(const Numeric min) { mMin = min; }
        void SetMax(const Numeric max) { mMax = max; }
    private:
        Numeric mMin = 0;
        Numeric mMax = 1;
    };


    class Map : public Definition
    {
    public:
        // CTOR
        Map(Definition&& key, Definition&& value) :Definition("Map", "A key value map"), mKey(std::move(key)), mValue(std::move(value)) {}
        // DTOR
        ~Map() = default;

        // Getters and Setters
        Definition& GetKey() { return mKey; }
        void SetKey(Definition&& key) { mKey = std::move(key); }

        Definition& GetValue() { return mValue; }
        void SetValue(Definition&& value) { mValue = std::move(value); }

    private:
        Definition mKey;
        Definition mValue;
    };
    const Definition STRING{ "string" };
    const Number U8{ 0, UINT8_MAX, "u8" };
    const Number I8{ INT8_MIN, INT8_MAX, "i8" };
    const Number U16{ 0, UINT16_MAX, "u16" };
    const Number I16{ INT16_MIN, INT16_MAX, "i16" };
    const Number U32{ 0, UINT32_MAX, "u32" };
    const Number I32{ INT32_MIN, INT32_MAX, "i32" };
    const Number F32{ FLT_MIN, FLT_MAX, "f32" };
    const Number U64{ 0, UINT64_MAX, "u64" };
    const Number I64{ INT64_MIN, INT64_MAX, "i64" };
    const Number F64{ DBL_MIN, DBL_MAX, "f64" };
    const Definition UNIT { "unit", "A type with only one value" };
    class Option : public Definition, public SubDefinition
    {
        public:
        Option(Definition&& tmplt) :Definition("Option", "An optional value"), SubDefinition(tmplt) {}
        ~Option() = default;
    };
    



    // const Number f128{LDBL_MIN, LDBL_MAX, "f128"};
// idk abt this 1
#define RANGE(min, max) Number(min, max,"Range")

    const Number BOOL{ 0, 1, "bool" };
    const Definition CHAR = Definition{ "char" };
    const Definition REG_EXP = Definition{ "regexp" };
    template <typename T=std::any>
    class Member :public Definition, public SubDefinition
    {

    public:
        Member(std::string&& name, std::string&& doc, Definition&& type) :Definition(name, doc), SubDefinition(type) {}

       inline void AddExample(T&& example)
        {
            mExamples.push_back(std::move(example));
        }
  
        T& GetDefault() { return mDefault; }
        void SetDefault(T&& def) { mDefault = std::move(def); }
    private:
        T mDefault;
        bool mOptional = false; 
        std::vector<T> mExamples;
    };

     


    class Object : public Definition, public SubDefinitionList
    {

    public:
        using T = std::any;
        Object(std::string name, std::string doc, std::initializer_list<Definition> schemas) :Definition(name, doc), SubDefinitionList(schemas) {}
        ~Object() = default;
        inline void AddField(Member<T>&& field) { mFields.push_back(std::move(field)); }

    private:
        std::vector <Member<T>> mFields{};
        std::vector<NamespacePath> mParents{};
    };

    class Tuple : public Definition, public SubDefinitionList
    {
    public:                                                                                          // TODO: Documentation
        Tuple(std::initializer_list<Definition> schemas) :Definition("Tuple", ""), SubDefinitionList(schemas) {};
        ~Tuple() = default;
    };

    class Array : public Definition, public SubDefinition
    {
    public:
        Array(Definition tmplt) :Definition("Array", "A list of elements"), SubDefinition(tmplt) {}
        ~Array() = default;
    };                                                                       
    class Set : public Definition, public SubDefinition
    {
    public:
        Set(Definition tmplt) :Definition("Set", "A set of unique elements"), SubDefinition(tmplt) {}
    };


    class Variant : public SubDefinitionList, public Definition
    {
    public:
        Variant(std:: initializer_list<Definition> schemas) :Definition("Variant", "A variant type that can hold many types"), SubDefinitionList(schemas) {}
        ~Variant() = default;
    };
}
