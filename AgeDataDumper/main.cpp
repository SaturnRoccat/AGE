

#include "main.hpp"
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





    SchemaDef::Namespace root{"root",{}};                                                                                                                                                                                                                                
    SchemaDef::Namespace stdl {
        "std",{
            // u8 through u64
            SchemaDef::U8, SchemaDef::U16, SchemaDef::U32, SchemaDef::I64,
            // i8 through i64
            SchemaDef::I8, SchemaDef::I16, SchemaDef::I32, SchemaDef::I64,
            // f32 and f64
            SchemaDef::F32, SchemaDef::F64,
            // bool
            SchemaDef::BOOL,
            // STRING
            SchemaDef::STRING,
            SchemaDef::REG_EXP,
        }};

    root.AddDef(stdl);

    SchemaDef::Namespace Bedrock{"Bedrock",{}};
    SchemaDef::Namespace Minecraft{"Minecraft",{Bedrock}};

    // Iterate over the object
    for (rapidjson::Value::MemberIterator val = d.MemberBegin(); val != d.MemberEnd(); ++val) {
        std::string_view path{ val->name.GetString(), val->name.GetStringLength() };

        // Json Schema Parsing
        rapidjson::Value& value = val->value;
        JsonSchema::JsonSchema json_schema{};
        json_schema.parse(value);
        // Definition Def 
        //SchemaDef::Object schema{"null"};
        std::println("path: {}", path);

        // get if 
        if (auto title = json_schema.getTitle())
        {

            std::println("title: {}", title.value());
        }
        // get the type
        if (auto type = json_schema.getType(); type) {
            if (auto type_str = std::get_if<std::string>(&type.value())) {
                std::cout << "type: " << *type_str << std::endl;
            }
        }
        
        if (path.contains("file:///data/packages/minecraftBedrock/schema/project/experimentalGameplay/"))
        {


        }



    }
    return 0;
}



