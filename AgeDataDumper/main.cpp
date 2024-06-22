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

#include <Windows.h>

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
#include<vector>
//#include "patterns.hpp"


size_t findLastOfString(std::string str, const std::string& find) {
    size_t last = 0;
    size_t pos = 0;
    while ((pos = str.find(find, pos)) != std::string::npos) {
        last = pos;
        pos++;
    }
    return last;
}


class ReferenceCache
{
public:
    static std::string MakeFullPath(std::string path, std::string reference)
    {
        path = path.substr(0, path.find_last_of('/'));

        uint32_t backoutCount = 0;
        while (reference.find("../") == 0) {
            reference = reference.substr(3);
            backoutCount++;
        }

        while (backoutCount)
        {
            path = path.substr(0, path.find_last_of('/'));
            backoutCount--;
        }
        if (reference.starts_with("./"))
            reference = reference.substr(2);
        return path + "/" + reference;
    }
private:
};

int main() {


    // open a file and read to string 
    std::ifstream fileData("./bridge/packages/minecraftBedrock/schemas.json");

    if (!fileData) {
        std::println("Error: Opening file failed");
        return 1;
    }


    std::ostringstream oss;
    oss << fileData.rdbuf();
    std::string json = oss.str();
    fileData.close();

    rapidjson::Document d;


    d.Parse(json.c_str());

    if (d.HasParseError()) {
        std::println("Error: Parsing JSON failed");
        return 1;
    }

    if (!d.IsObject()) {
        std::println("Error: JSON is not an object");
        return 1;
    }

    // Iterate over the object
    for (rapidjson::Value::MemberIterator key_val = d.MemberBegin(); key_val != d.MemberEnd(); ++key_val) {
        std::string_view path{ key_val->name.GetString(), key_val->name.GetStringLength() };

        std::println("path: {}", path);

        rapidjson::Value& key_val_val = key_val->value;

        if (!key_val_val.IsObject())
        {
            //  auto obj = key_val_val.GetObject();



            std::println("not obj");
        };
    }
    return 0;
}

class Namespace
{
    std::vector <std::string> mPath;
};

enum TypeDefError {
    InvalidMerge,
};



class TypeDefBase
{
    virtual std::expected<TypeDefBase, TypeDefError> Merge(TypeDefBase other)
    {
        return std::unexpected(TypeDefError::InvalidMerge);
    };
};

class TypeStructure : public TypeDefBase
{
public:
    TypeStructure() = default;
    TypeStructure(std::string name, std::string documentation) : mName(name), mDocumentation(documentation) {}
    TypeStructure(std::string name) : mName(name) {}

    std::string_view GetName() { return mName; }
    std::string_view GetDocumentation() { return mDocumentation; }

    /// Add Documentation
    void AddDocumentation(std::string_view documentation) { mDocumentation += documentation; }

    virtual std::expected<TypeDefBase, TypeDefError> Merge(TypeDefBase other) {
        return std::unexpected(TypeDefError::InvalidMerge);
    }


private:
    std::string mName{};
    std::string mDocumentation{};

};

class EnumField :public TypeStructure
{
public:
    EnumField() = default;
    EnumField(std::string name, std::string documentation, int value) : TypeStructure(name, documentation), mValue(value) {}
    ~EnumField() = default;

    // Getters and Setters
    int GetValue() { return mValue; }
    void SetValue(int value) { mValue = value; }
private:
    int mValue;

};

class TypeEnum : public TypeStructure
{
public:
    TypeEnum() = default;
    ~TypeEnum() = default;

    std::vector <EnumField>& GetFields() { return mFields; }
    void AddField(EnumField field) { mFields.push_back(field); }
    void AddField(std::string name, std::string documentation, int value) { mFields.push_back(EnumField(name, documentation, value)); }

private:
    std::vector <EnumField> mFields;
};
template<typename Default>
class ObjectField : public TypeStructure
{
public:


private:

    bool mRequired;



};
// signed, min, max and instance the 
class Number : public TypeDefBase
{
public:
    Number() = default;
    Number(int min, int max, std::string name) : mMin(min), mMax(max) {}
    ~Number() = default;

    // Getters and Setters
    int GetMin() { return mMin; }
    void SetMin(int min) { mMin = min; }
    int GetMax() { return mMax; }
    void SetMax(int max) { mMax = max; }
private:
    int mMin;
    int mMax;
};

class TypeObject : public TypeStructure
{
};



template <typename Default>
class Property : public TypeDefBase
{

public:
    // Getters and Setters
    Default& GetDefault() { return mDefault; }
    void SetDefault(Default default) { mDefault = default; }

private:
    Default mDefault;



};






