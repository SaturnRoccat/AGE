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
#include<vector>

size_t findLastOfString (std::string str, const std::string& find) {
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
		while(backoutCount)
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

using namespace rapidjson;

int main() {


	// open a file and read to string 
	std::ifstream fileData("C:/Users/Duckos/Downloads/packages_2/packages/minecraftBedrock/schemas.json");

	if (!fileData) {
		std::println("Error: Opening file failed");
		return 1;
	}


	std::ostringstream oss;
	oss << fileData.rdbuf();
	std::string json = oss.str();
	fileData.close();

	Document d;


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
	/*for (auto& itr = d.MemberBegin(); itr != d.MemberEnd(); ++itr) {
		
	}*/

	return 0;
}

