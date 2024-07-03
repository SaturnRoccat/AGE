#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/NBT/NBTFile.hpp>

namespace AgeAPI::Structure
{


	class MCBlock
	{
	private:
		std::string mIdentifier{};
		int mVersion{ 0x1150003 };

		// FIXME: This needs to have a states object
	public:
		MCBlock() = default;
		MCBlock(const std::string& identifier, int Version = {0x1150003}) : mIdentifier(identifier), mVersion(Version) {}

		const std::string& GetIdentifier() const { return mIdentifier; }
		void SetIdentifier(const std::string& identifier) { mIdentifier = identifier; }
		int GetVersion() const { return mVersion; }

		void WriteToNBT(NBT::TagCompound& tag) const
		{
			tag["name"] = NBT::TagString(mIdentifier);
			tag["version"] = NBT::TagInt(mVersion);
			tag["states"] = NBT::TagCompound{}; // FIXME: This needs to be a real object
		}


		bool operator==(const MCBlock& other) const
		{
			return mIdentifier == other.mIdentifier;
		}

		bool operator!=(const MCBlock& other) const
		{
			return !(*this == other);
		}

	};

}

namespace std
{
	template<>
	struct hash<AgeAPI::Structure::MCBlock>
	{
		size_t operator()(const AgeAPI::Structure::MCBlock& block) const
		{
			return hash<string>()(block.GetIdentifier());
		}
	};

}