#pragma once
#include <AgeAPI/internal/Structures/MCBlock.hpp>

namespace AgeAPI::Structure
{
	class StructurePalette
	{
	public:
		StructurePalette() = default;

		int RegisterBlock(const MCBlock& block);
		int GetBlockReferenceIndex(const MCBlock& block) const;
		const MCBlock& GetBlockByIndex(int index) const;

		void WriteToNBT(NBT::TagCompound& tag) const;

	private:
		std::vector<std::pair<int, MCBlock>> mPallet{};
		std::unordered_map<MCBlock, int> mBlockToIndex{};

	};
}