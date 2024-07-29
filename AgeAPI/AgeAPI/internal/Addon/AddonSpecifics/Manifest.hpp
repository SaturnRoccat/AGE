#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/Error.hpp>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>

namespace AgeAPI
{
	using Capabilities = std::vector<std::string>;

	class Dependency
	{
	private:
		std::string mUUID{""};
		SemanticVersion mVersion{};
	public:
		Dependency() = default;
		Dependency(const std::string& uuid, const SemanticVersion& version) : mUUID(uuid), mVersion(version) {}

		std::string GetUUID() const { return mUUID; }
		SemanticVersion GetVersion() const { return mVersion; }

		void SetUUID(const std::string& uuid) { mUUID = uuid; }
		void SetVersion(const SemanticVersion& version) { mVersion = version; }

		void WriteToJson(JsonProxy& proxy) const;
	};

	class Module
	{
	private:
		std::string mDescription{""};
		std::string mUUID{""};
		std::string mType{""};
		SemanticVersion mVersion{};
	public:
		Module() = default;
		Module(const std::string& type, const std::string& description = "", const SemanticVersion& version = {1, 0, 0}, const std::string& uuid = GetUUIDString()) : mDescription(description), mUUID(uuid), mType(type), mVersion(version) {}

		std::string GetDescription() const { return mDescription; }
		std::string GetUUID() const { return mUUID; }
		std::string GetType() const { return mType; }
		SemanticVersion GetVersion() const { return mVersion; }

		void SetDescription(const std::string& description) { mDescription = description; }
		void SetUUID(const std::string& uuid) { mUUID = uuid; }
		void SetType(const std::string& type) { mType = type; }
		void SetVersion(const SemanticVersion& version) { mVersion = version; }

		void WriteToJson(JsonProxy& proxy) const;
	};

	class Metadata
	{
	private:
		std::vector<std::string> mAuthors{};
		std::string mLicense{""};
		std::string mURL{""};
	public:
		Metadata() = default;
		Metadata(const std::vector<std::string>& authors, const std::string& license, const std::string& url) : mAuthors(authors), mLicense(license), mURL(url) {}

		std::vector<std::string> GetAuthors() const { return mAuthors; }
		std::string GetLicense() const { return mLicense; }
		std::string GetURL() const { return mURL; }

		void SetAuthors(const std::vector<std::string>& authors) { mAuthors = authors; }
		void SetLicense(const std::string& license) { mLicense = license; }
		void SetURL(const std::string& url) { mURL = url; }

		void WriteToJson(JsonProxy& proxy) const;
	

	};


	class Manifest
	{
	private:
		int mFormatVersion{2};
		SemanticVersion mMinEngineVersion{};
		SemanticVersion mAddonVersion{};
		std::string mName{""};
		std::string mDescription{""};
		std::string mUUID{""};
		std::vector<Module> mModules{};
		std::vector<Dependency> mDependencies{};
		Metadata mMetadata{};
		Capabilities mCapabilities{};
	public:
		Manifest() = default;
		Manifest(
			const SemanticVersion& minEngineVersion,
			const SemanticVersion& addonVersion,
			const std::string& name,
			const std::string& description,
			const std::vector<Module>& modules,
			const std::vector<Dependency>& dependencies = {},
			const Metadata& metadata = {},
			const Capabilities& capabilities = {},
			const std::string& uuid = GetUUIDString()
		) :
			mMinEngineVersion(minEngineVersion),
			mAddonVersion(addonVersion),
			mName(name),
			mDescription(description),
			mUUID(uuid),
			mModules(modules),
			mDependencies(dependencies),
			mMetadata(metadata),
			mCapabilities(capabilities) {}

		void AddModule(const Module& module) { mModules.push_back(module); }
		void AddDependency(const Dependency& dependency) { mDependencies.push_back(dependency); }
		void AddCapability(const std::string& capability) { mCapabilities.push_back(capability); }

		Metadata& GetMetadata() { return mMetadata; }
		SemanticVersion GetFormatVersion() const { return mFormatVersion; }
		SemanticVersion GetMinEngineVersion() const { return mMinEngineVersion; }
		SemanticVersion GetAddonVersion() const { return mAddonVersion; }
		const std::string& GetName() const { return mName; }
		const std::string& GetDescription() const { return mDescription; }
		const std::string& GetUUID() const { return mUUID; }
		const std::vector<Module>& GetModules() const { return mModules; }
		const std::vector<Dependency>& GetDependencies() const { return mDependencies; }
		const Capabilities& GetCapabilities() const { return mCapabilities; }


		void SetFormatVersion(int formatVersion) { mFormatVersion = formatVersion; }
		void SetMinEngineVersion(const SemanticVersion& minEngineVersion) { mMinEngineVersion = minEngineVersion; }
		void SetAddonVersion(const SemanticVersion& addonVersion) { mAddonVersion = addonVersion; }
		void SetName(const std::string& name) { mName = name; }
		void SetDescription(const std::string& description) { mDescription = description; }
		void SetUUID(const std::string& uuid) { mUUID = uuid; }
		void SetMetadata(const Metadata& metadata) { mMetadata = metadata; }
		void SetModules(const std::vector<Module>& modules) { mModules = modules; }
		void SetDependencies(const std::vector<Dependency>& dependencies) { mDependencies = dependencies; }
		void SetCapabilities(const Capabilities& capabilities) { mCapabilities = capabilities; }

		Dependency GetAsDependency() const { return Dependency(mUUID, mAddonVersion); }
	
		ErrorString WriteToValue(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const;
		inline std::expected<rapidjson::Document, ErrorString> WriteToDocument() const
		{
			rapidjson::Document doc;
			auto val = WriteToValue(doc, doc.GetAllocator());
			if (val.ContainsError())
				return std::unexpected(val);
			return doc;
		}
		
	};
}