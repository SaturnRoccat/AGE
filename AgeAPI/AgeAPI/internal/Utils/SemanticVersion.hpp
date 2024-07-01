#pragma once
#include <AgeAPI/internal/Utils/BasicTypeDecls.hpp>
#include <array>
#include <vector>
#include <format>

namespace AgeAPI
{
	class SemanticVersion
	{
	public:
		SemanticVersion(u8 major, u8 minor, u8 patch) : mMajor(major), mMinor(minor), mPatch(patch) {}
		SemanticVersion(std::array<u8, 3> version) : mVersionArray(version) {}
		SemanticVersion(u32 version = 0) : mVersion(version) {}
		~SemanticVersion() = default;
		u8 GetMajor() const { return mMajor; }
		u8 GetMinor() const { return mMinor; }
		u8 GetPatch() const { return mPatch; }
		u32 GetVersion() const { return mVersion; }
		std::vector<u8> GetVersionVector() const { return { mMajor, mMinor, mPatch }; }
		std::array<u8, 3> GetVersionArray() const { return mVersionArray; }
		std::string GetString() const { return std::format("{}.{}.{}", mMajor, mMinor, mPatch); }
		void SetMajor(u8 major) { mMajor = major; }
		void SetMinor(u8 minor) { mMinor = minor; }
		void SetPatch(u8 patch) { mPatch = patch; }

		bool operator==(const SemanticVersion& other) const { return mVersion == other.mVersion; }
		bool operator!=(const SemanticVersion& other) const { return mVersion != other.mVersion; }
		bool operator<(const SemanticVersion& other) const { return mVersion < other.mVersion; }
		bool operator>(const SemanticVersion& other) const { return mVersion > other.mVersion; }
		bool operator<=(const SemanticVersion& other) const { return mVersion <= other.mVersion; }
		bool operator>=(const SemanticVersion& other) const { return mVersion >= other.mVersion; }
	private:
		union
		{
			struct
			{
				u8 mMajor;
				u8 mMinor;
				u8 mPatch;
			};
			std::array<u8, 3> mVersionArray;
			u32 mVersion{ 0 };
		};
	};

}