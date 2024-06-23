#pragma once
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <any>
#include <string>
#include <print>
#include <variant>
#include <concepts>
#include <array>
#include <expected>


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
