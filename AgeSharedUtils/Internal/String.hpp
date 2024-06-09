#pragma once
#include <string>


namespace AgeUtils
{
	/*
	* @brief Compare two strings
	* @param str1 First string
	* @param str2 Second string
	* @return True if strings are equal, false otherwise
	*/
	bool StringCompare(std::string_view str1, std::string_view str2);
}