#pragma once
#include <uuid_v4.h>
#include <filesystem>

namespace AgeAPI
{
	static std::string GetUUIDString()
	{
		thread_local static UUIDv4::UUIDGenerator<std::mt19937_64> generator;
		return generator.getUUID().str();

	}

	static const std::string& GetCurrentWorkingDirectory()
	{
		thread_local static std::string cwd = std::filesystem::current_path().string();
		return cwd;
	}
}