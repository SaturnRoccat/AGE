#pragma once
#include <AgeAPI/internal/Types.hpp>

namespace AgeAPI::Components
{

	class ComponentBase 
	{
	public:
		ComponentBase() = default;
		ComponentBase(
			const ExperimentalSettings& settings,
			const SemanticVersion& version,
			const Identifier& componentID,
			bool canBeDoublePushed = false,
			bool isTransient = false,
			SemanticVersion maxVersion = {255, 255, 255}
		) :
			mExperimentalSettings(settings),
			mFormatVersion(version), 
			mComponentID(componentID), 
			mCanBeDoublePushed(canBeDoublePushed), 
			mIsTransient(isTransient), 
			mMaxVersion(maxVersion) {}



		bool CanBeDoublePushed() const { return mCanBeDoublePushed; }
		bool IsTransient() const { return mIsTransient; }
		bool IsVersionCompatible(const SemanticVersion& version) const { return version >= mFormatVersion && version <= mMaxVersion; }
		const ExperimentalSettings& GetExperimentalSettings() const { return mExperimentalSettings; }
		SemanticVersion GetMaxVersion() const { return mMaxVersion; } // faster to copy then ref cuz its smaller than a ptr
		SemanticVersion GetFormatVersion() const { return mFormatVersion; }
		const Identifier& GetComponentID() const { return mComponentID; }

		void SetExperimentalSettings(const ExperimentalSettings& settings) { mExperimentalSettings = settings; }
		void SetFormatVersion(const SemanticVersion& version) { mFormatVersion = version; }
        void SetIdentifier(const Identifier& id) { mComponentID = id; }
		void SetCanBeDoublePushed(bool canBeDoublePushed) { mCanBeDoublePushed = canBeDoublePushed; }
		void SetIsTransient(bool isTransient) { mIsTransient = isTransient; }
		void SetMaxVersion(const SemanticVersion& version) { mMaxVersion = version; }

	protected:
		ExperimentalSettings mExperimentalSettings;
		bool mCanBeDoublePushed = false;	// This allows components to be pushed twice meaning if you did 
		bool mIsTransient = false;			// This means that the component is not saved to the file
		SemanticVersion mFormatVersion;
		SemanticVersion mMaxVersion = { 255, 255, 255 };
		Identifier mComponentID;
	};

	template<typename T, typename U>
	concept Component = std::is_base_of_v<ComponentBase, U>;

}