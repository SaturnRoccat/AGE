#include "FileIO.hpp"
#include <fstream>

AgeAPI::ErrorString rapidjson::WriteJsonFile(const rapidjson::Value& value, const std::filesystem::path& path)
{
	std::filesystem::create_directories(path.parent_path());

	std::ofstream file(path);
	if (!file.is_open())
		return "Failed to open file for writing";
	rapidjson::OStreamWrapper stream(file);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(stream);
	value.Accept(writer);
	stream.Flush();
	file.close();
	return "";
}
