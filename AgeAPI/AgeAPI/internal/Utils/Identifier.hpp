#pragma once
#include <string>
#include <format>
#include <AgeAPI/internal/Utils/BasicTypeDecls.hpp>
#include <concepts>

namespace AgeAPI
{
    
	class Identifier
	{
	public:
		Identifier() = default;
		Identifier(const std::string& fullNamespace) : mFullNamespace(fullNamespace) { mColonPos = mFullNamespace.find(':'); }
		Identifier(const std::string& name, const std::string& namespace_) : mFullNamespace(namespace_ + ":" + name) { mColonPos = mFullNamespace.find(':'); }
		Identifier(const char* fullNamespace) : mFullNamespace(fullNamespace) { mColonPos = mFullNamespace.find(':'); }
		Identifier(const char* name, const char* namespace_) : mFullNamespace(std::format("{}:{}", name, namespace_)) { mColonPos = mFullNamespace.find(':'); }
		Identifier(const Identifier& other) : mFullNamespace(other.mFullNamespace), mColonPos(other.mColonPos) {}

		template<typename T> requires std::is_constructible_v<std::string, T>
		Identifier(T&& ident) : mFullNamespace(std::forward<T>(ident)) { mColonPos = mFullNamespace.find(':'); }

		const std::string_view GetName() const { return mFullNamespace.substr(mColonPos + 1); }
		const std::string_view GetNamespace() const { return mFullNamespace.substr(0, mColonPos); }
		const std::string& GetFullNamespace() const { return mFullNamespace; }
		std::string GetFullNamespaceFile() const {
			std::string str = mFullNamespace;
			for (char& c : str)
			{
				if (c == ':')
					c = '_';
			}
			return str;
		}

		void SetName(const std::string& name) { mFullNamespace = std::format("{}:{}", GetNamespace(), name); }
		void SetNamespace(const std::string& namespace_) { mFullNamespace = std::format("{}:{}", namespace_, GetName()); }
		void SetFullNamespace(const std::string& fullNamespace)
		{
			mFullNamespace = fullNamespace;
			mColonPos = mFullNamespace.find(':');
		}

		bool operator==(const Identifier& other) const { return mFullNamespace == other.mFullNamespace; }
		bool operator!=(const Identifier& other) const { return mFullNamespace != other.mFullNamespace; }

		bool Validate() const
		{
			for (char c : mFullNamespace)
			{
				if (c >= 0x80 && isupper(c))
					return false;
			}
			if (mFullNamespace.find(' ') != std::string::npos)
				return false;
			return true;

		}


	private:
		std::string mFullNamespace{};
		u32 mColonPos{ 0 };
	};
}


namespace rapidjson
{
	template<>
	struct TypeTranslation<AgeAPI::Identifier, false>
	{
		static void WriteToJson(const AgeAPI::Identifier& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.SetString(value.GetFullNamespace(), allocator);
		}

		static AgeAPI::Identifier ReadFromJson(const rapidjson::Value& jsonValue)
		{
			return AgeAPI::Identifier(jsonValue.GetString());
		}
	};
}


namespace std
{

	template<>
	struct hash<AgeAPI::Identifier>
	{
		std::size_t operator()(const AgeAPI::Identifier& id) const
		{
			return std::hash<std::string>{}(id.GetFullNamespace());
		}
	};
}