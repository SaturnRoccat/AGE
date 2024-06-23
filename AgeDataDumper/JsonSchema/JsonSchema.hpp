#pragma once             

#include <string>
#include <vector>
#include <map>
#include <variant>
#include <optional>
#include <memory>
#include <rapidjson/document.h>


namespace JsonSchema
{
    class JsonSchema
    {
    public:
        JsonSchema() = default;
        ~JsonSchema() = default;

        void parse(const rapidjson::Value& value);
        // Getters
        inline const std::optional<std::string>& getId() const { return mId; }
        inline const std::optional<std::string>& getSchema() const { return mSchema; }
        inline const std::optional<std::string>& getRef() const { return mRef; }
        inline const std::optional<std::string>& getComment() const { return mComment; }
        inline const std::optional<std::string>& getTitle() const { return mTitle; }
        inline const std::optional<std::string>& getDescription() const { return mDescription; }
        inline const std::optional<std::variant<bool, std::string, double, std::vector<std::variant<bool, std::string, double>>>>& getDefault() const { return mDefault; }
        inline const std::optional<bool>& getReadOnly() const { return mReadOnly; }
        inline const std::optional<bool>& getWriteOnly() const { return mWriteOnly; }
        inline const std::optional<std::vector<std::variant<bool, std::string, double>>>& getExamples() const { return mExamples; }
        inline const std::optional<double>& getMultipleOf() const { return mMultipleOf; }
        inline const std::optional<double>& getMaximum() const { return mMaximum; }
        inline const std::optional<double>& getExclusiveMaximum() const { return mExclusiveMaximum; }
        inline const std::optional<double>& getMinimum() const { return mMinimum; }
        inline const std::optional<double>& getExclusiveMinimum() const { return mExclusiveMinimum; }
        inline const std::optional<size_t>& getMaxLength() const { return mMaxLength; }
        inline const std::optional<size_t>& getMinLength() const { return mMinLength; }
        inline const std::optional<std::string>& getPattern() const { return mPattern; }
        inline const std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>>& getItems() const { return mItems; }
        inline const std::optional<size_t>& getMaxItems() const { return mMaxItems; }
        inline const std::optional<size_t>& getMinItems() const { return mMinItems; }
        inline const std::optional<bool>& getUniqueItems() const { return mUniqueItems; }
        inline const std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>>& getContains() const { return mContains; }
        inline const std::optional<size_t>& getMaxProperties() const { return mMaxProperties; }
        inline const std::optional<size_t>& getMinProperties() const { return mMinProperties; }
        inline const std::optional<std::vector<std::string>>& getRequired() const { return mRequired; }
        inline const std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>>& getAdditionalProperties() const { return mAdditionalProperties; }
        inline const std::optional<std::map<std::string, std::shared_ptr<JsonSchema>>>& getDefinitions() const { return mDefinitions; }
        inline const std::optional<std::map<std::string, std::shared_ptr<JsonSchema>>>& getProperties() const { return mProperties; }
        inline const std::optional<std::map<std::string, std::shared_ptr<JsonSchema>>>& getPatternProperties() const { return mPatternProperties; }
        inline const std::optional<std::map<std::string, std::variant<std::shared_ptr<JsonSchema>, std::vector<std::string>>>>& getDependencies() const { return mDependencies; }
        inline const std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>>& getPropertyNames() const { return mPropertyNames; }
        inline const std::optional<std::variant<bool, std::string, double>>& getConst() const { return mConst; }
        inline const std::optional<std::vector<std::variant<bool, std::string, double>>>& getEnum() const { return mEnum; }
        inline const std::optional<std::variant<std::string, std::vector<std::string>>>& getType() const { return mType; }
        inline const std::optional<std::string>& getFormat() const { return mFormat; }
        inline const std::optional<std::string>& getContentMediaType() const { return mContentMediaType; }
        inline const std::optional<std::string>& getContentEncoding() const { return mContentEncoding; }
        inline const std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>>& getIf() const { return mIf; }
        inline const std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>>& getThen() const { return mThen; }
        inline const std::optional<std::variant<std::shared_ptr<JsonSchema>, bool>>& getElse() const { return mElse; }
        inline const std::optional<std::vector<std::shared_ptr<JsonSchema>>>& getAllOf() const { return mAllOf; }
        inline const std::optional<std::vector<std::shared_ptr<JsonSchema>>>& getAnyOf() const { return mAnyOf; }
        inline const std::optional<std::vector<std::shared_ptr<JsonSchema>>>& getOneOf() const { return mOneOf; }
        inline const std::optional<std::shared_ptr<JsonSchema>>& getNot() const { return mNot; }

        // Setters
        inline void setId(const std::string& id) { mId = id; }
        inline void setSchema(const std::string& schema) { mSchema = schema; }
        inline void setRef(const std::string& ref) { mRef = ref; }
        inline void setComment(const std::string& comment) { mComment = comment; }
        inline void setTitle(const std::string& title) { mTitle = title; }
        inline void setDescription(const std::string& description) { mDescription = description; }
        inline void setDefault(const std::variant<bool, std::string, double, std::vector<std::variant<bool, std::string, double>>>& def) { mDefault = def; }
        inline void setReadOnly(bool readOnly) { mReadOnly = readOnly; }
        inline void setWriteOnly(bool writeOnly) { mWriteOnly = writeOnly; }
        inline void setExamples(const std::vector<std::variant<bool, std::string, double>>& examples) { mExamples = examples; }
        inline void setMultipleOf(double multipleOf) { mMultipleOf = multipleOf; }
        inline void setMaximum(double maximum) { mMaximum = maximum; }
        inline void setExclusiveMaximum(double exclusiveMaximum) { mExclusiveMaximum = exclusiveMaximum; }
        inline void setMinimum(double minimum) { mMinimum = minimum; }
        inline void setExclusiveMinimum(double exclusiveMinimum) { mExclusiveMinimum = exclusiveMinimum; }
        inline void setMaxLength(size_t maxLength) { mMaxLength = maxLength; }
        inline void setMinLength(size_t minLength) { mMinLength = minLength; }
        inline void setPattern(const std::string& pattern) { mPattern = pattern; }
        inline void setItems(const std::variant<std::shared_ptr<JsonSchema>, bool>& items) { mItems = items; }
        inline void setMaxItems(size_t maxItems) { mMaxItems = maxItems; }
        inline void setMinItems(size_t minItems) { mMinItems = minItems; }
        inline void setUniqueItems(bool uniqueItems) { mUniqueItems = uniqueItems; }
        inline void setContains(const std::variant<std::shared_ptr<JsonSchema>, bool>& contains) { mContains = contains; }
        inline void setMaxProperties(size_t maxProperties) { mMaxProperties = maxProperties; }
        inline void setMinProperties(size_t minProperties) { mMinProperties = minProperties; }
        inline void setRequired(const std::vector<std::string>& required) { mRequired = required; }
        inline void setAdditionalProperties(const std::variant<std::shared_ptr<JsonSchema>, bool>& additionalProperties) { mAdditionalProperties = additionalProperties; }
        inline void setDefinitions(const std::map<std::string, std::shared_ptr<JsonSchema>>& definitions) { mDefinitions = definitions; }
        inline void setProperties(const std::map<std::string, std::shared_ptr<JsonSchema>>& properties) { mProperties = properties; }
        inline void setPatternProperties(const std::map<std::string, std::shared_ptr<JsonSchema>>& patternProperties) { mPatternProperties = patternProperties; }
        inline void setDependencies(const std::map<std::string, std::variant<std::shared_ptr<JsonSchema>, std::vector<std::string>>>& dependencies) { mDependencies = dependencies; }
        inline void setPropertyNames(const std::variant<std::shared_ptr<JsonSchema>, bool>& propertyNames) { mPropertyNames = propertyNames; }
        inline void setConst(const std::variant<bool, std::string, double>& con) { mConst = con; }
        inline void setEnum(const std::vector<std::variant<bool, std::string, double>>& enu) { mEnum = enu; }
        inline void setType(const std::variant<std::string, std::vector<std::string>>& type) { mType = type; }
        inline void setFormat(const std::string& format) { mFormat = format; }
        inline void setContentMediaType(const std::string& contentMediaType) { mContentMediaType = contentMediaType; }
        inline void setContentEncoding(const std::string& contentEncoding) { mContentEncoding = contentEncoding; }
        inline void setIf(const std::variant<std::shared_ptr<JsonSchema>, bool>& ifVal) { mIf = ifVal; }
        inline void setThen(const std::variant<std::shared_ptr<JsonSchema>, bool>& thenVal) { mThen = thenVal; }
        inline void setElse(const std::variant<std::shared_ptr<JsonSchema>, bool>& elseVal) { mElse = elseVal; }
        inline void setAllOf(const std::vector<std::shared_ptr<JsonSchema>>& allOf) { mAllOf = allOf; }
        inline void setAnyOf(const std::vector<std::shared_ptr<JsonSchema>>& anyOf) { mAnyOf = anyOf; }
        inline void setOneOf(const std::vector<std::shared_ptr<JsonSchema>>& oneOf) { mOneOf = oneOf; }
        inline void setNot(const std::shared_ptr<JsonSchema>& notVal) { mNot = notVal; }

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


}