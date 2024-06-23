#pragma once
#include "./JsonSchema/JsonSchema.hpp"
#include "TDL/TDL.hpp"

#include <string>
#include <vector>
#include <map>
#include <variant>
#include <optional>
#include <memory>
#include <rapidjson/document.h>     

#include <any>
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
#include <span>
#include<vector>
#include <string_view>
// TODO: Move the defines to the xmake.lua
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

