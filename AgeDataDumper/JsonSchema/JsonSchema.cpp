
#include "JsonSchema.hpp"
namespace JsonSchema {

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


