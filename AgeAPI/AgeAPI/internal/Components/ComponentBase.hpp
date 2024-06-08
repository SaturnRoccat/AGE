#pragma once
#include <AgeAPI/internal/Types.hpp>

namespace AgeAPI
{
	class Addon;
}

namespace AgeAPI::Components
{
	class ComponentBase
	{
	public:
		ComponentBase() = default;
		ComponentBase(const ExperimentalSettings& settings, const SemanticVersion& version) : mExperimentalSettings(settings), mFormatVersion(version) {}

		ExperimentalSettings GetExperimentalSettings() const { return mExperimentalSettings; }
		SemanticVersion GetFormatVersion() const { return mFormatVersion; }

		void SetExperimentalSettings(const ExperimentalSettings& settings) { mExperimentalSettings = settings; }
		void SetFormatVersion(const SemanticVersion& version) { mFormatVersion = version; }
	private:
		ExperimentalSettings mExperimentalSettings;
		SemanticVersion mFormatVersion;
	};

}