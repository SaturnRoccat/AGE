#include "Manifest.hpp"

namespace AgeAPI
{
	void Metadata::WriteToJson(MetadataProxy& proxy) const
	{
		rapidjson::ValueWriteWithKey<std::vector<std::string>>::WriteToJsonValue(
			"authors",
			this->mAuthors,
			proxy.mMetadata,
			proxy.mAllocator
		);
		rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue(
			"license",
			this->mLicense,
			proxy.mMetadata,
			proxy.mAllocator
		);
		rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue(
			"url",
			this->mURL,
			proxy.mMetadata,
			proxy.mAllocator
		);
	}
	void Module::WriteToJson(ModuleProxy& proxy) const
	{
		rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue(
			"description",
			this->mDescription,
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
		rapidjson::ValueWriteWithKey<std::array<u8, 3>>::WriteToJsonValue(
			"version",
			this->mVersion.GetVersionArray(),
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
}