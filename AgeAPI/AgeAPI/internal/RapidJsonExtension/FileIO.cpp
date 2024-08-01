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

AgeAPI::ErrorString rapidjson::ForcePath(const std::filesystem::path& outputDir)
{
	if (!std::filesystem::exists(outputDir))
	{
		if (!std::filesystem::create_directory(outputDir))
			return std::format("Failed to create directory: {}", outputDir.string());
	}
	return "";
}
