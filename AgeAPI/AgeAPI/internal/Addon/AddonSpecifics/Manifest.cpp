#include "Manifest.hpp"

namespace AgeAPI
{
	void Metadata::WriteToJson(MetadataProxy& proxy) const
	{
		if (!this->mAuthors.empty())
			rapidjson::ValueWriteWithKey<std::vector<std::string>>::WriteToJsonValue(
			"authors",
			this->mAuthors,
			proxy.mMetadata,
			proxy.mAllocator
		);
		if (this->mLicense != "")
			rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue(
			"license",
			this->mLicense,
			proxy.mMetadata,
			proxy.mAllocator
		);
		if (this->mURL != "")
			rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue(
			"url",
			this->mURL,
			proxy.mMetadata,
			proxy.mAllocator
		);
	}
	void Module::WriteToJson(ModuleProxy& proxy) const
	{
		if (this->mDescription != "")
			rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue(
				"description",
				this->mDescription,
				proxy.mModules,
				proxy.mAllocator
			);
		if (this->mVersion.GetVersion() != 0)
			rapidjson::ValueWriteWithKey<std::array<u8, 3>>::WriteToJsonValue(
				"version",
				this->mVersion.GetVersionArray(),
				proxy.mModules,
				proxy.mAllocator
			);
		rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue(
			"uuid",
			this->mUUID,
			proxy.mModules,
			proxy.mAllocator
		);
		rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue(
			"type",
			this->mType,
			proxy.mModules,
			proxy.mAllocator
		);
	}
	void Dependency::WriteToJson(DependencyProxy& proxy) const
	{
		rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue(
			"uuid",
			this->mUUID,
			proxy.mDependencies,
			proxy.mAllocator
		);
		rapidjson::ValueWriteWithKey<std::array<u8, 3>>::WriteToJsonValue(
			"version",
			this->mVersion.GetVersionArray(),
			proxy.mDependencies,
			proxy.mAllocator
		);
	}
	ErrorString Manifest::WriteToValue(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const
	{
		value.SetObject();
		if (this->mFormatVersion != 2)
			return "Format version must be 2";
		rapidjson::ValueWriteWithKey<int>::WriteToJsonValue("format_version", this->mFormatVersion, value, allocator);

		rapidjson::Value header(rapidjson::kObjectType);
		rapidjson::ValueWriteWithKey<std::array<u8, 3>>::WriteToJsonValue("min_engine_version", this->mMinEngineVersion.GetVersionArray(), header, allocator);
		rapidjson::ValueWriteWithKey<std::array<u8, 3>>::WriteToJsonValue("version", this->mAddonVersion.GetVersionArray(), header, allocator);
		rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue("name", this->mName, header, allocator);
		rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue("description", this->mDescription, header, allocator);
		rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue("uuid", this->mUUID, header, allocator);
		value.AddMember("header", header, allocator);

		if (!this->mModules.empty())
		{
			rapidjson::Value modules(rapidjson::kArrayType);
			modules.Reserve(this->mModules.size(), allocator);
			for (const auto& module : this->mModules)
			{
				rapidjson::Value moduleValue(rapidjson::kObjectType);
				ModuleProxy moduleProxy{ moduleValue, allocator };
				module.WriteToJson(moduleProxy);
				modules.PushBack(moduleValue, allocator);
			}
			value.AddMember("modules", modules, allocator);
		}

		if (!this->mDependencies.empty())
		{
			rapidjson::Value dependencies(rapidjson::kArrayType);
			dependencies.Reserve(this->mDependencies.size(), allocator);
			for (const auto& dependency : this->mDependencies)
			{
				rapidjson::Value dependencyValue(rapidjson::kObjectType);
				DependencyProxy dependencyProxy{ dependencyValue, allocator };
				dependency.WriteToJson(dependencyProxy);
				dependencies.PushBack(dependencyValue, allocator);
			}
			value.AddMember("dependencies", dependencies, allocator);
		}

		MetadataProxy metadataProxy{ value, allocator };
		this->mMetadata.WriteToJson(metadataProxy);
		return "";
	}
}

struct test
{
	std::array<std::array<std::array<std::array<std::string, 16>, 16>, 16>, 24> blocks;
};