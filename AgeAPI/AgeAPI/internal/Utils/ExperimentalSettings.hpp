#pragma once
#include <AgeAPI/internal/Utils/BasicTypeDecls.hpp>
#include <AgeAPI/internal/Utils/SmallUtilities.hpp>
#include <array>


namespace AgeAPI
{
	enum class Experiments : u8
	{
		HolidayCreatorFeatures = 1 << 0,
		CustomBiomes = 1 << 1,
		UpcomingCreatorFeatures = 1 << 2,
		BetaAPI = 1 << 3,
		ExperimentalCameras = 1 << 4,
		ExperimentsALL = 0xFF
	};
	class ExperimentalSettings
	{
	public:
		ExperimentalSettings() = default;
		ExperimentalSettings(u8 flags) : mExperimentalFlags(flags) {}
		ExperimentalSettings(const std::array<Experiments, 5> flags)
		{
			for (Experiments flag : flags)
				mExperimentalFlags |= TO_UNDERLYING(flag);
		}
		ExperimentalSettings(const Experiments flags) : mExperimentalFlags(TO_UNDERLYING(flags)) {}

		bool IsFlagSet(u8 flag) const { return mExperimentalFlags & flag; }
		void SetFlag(u8 flag) { mExperimentalFlags |= flag; }
		void ClearFlag(u8 flag) { mExperimentalFlags &= ~flag; }
		u8 GetFlags() const { return mExperimentalFlags; }
	private:
		u8 mExperimentalFlags{ 0 };

	};

} // namespace AgeAPI
