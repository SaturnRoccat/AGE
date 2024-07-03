#pragma once
#include <AgeAPI/internal/Structures/MCPallet.hpp>

namespace AgeAPI::Structure
{
	int StructurePalette::RegisterBlock(const MCBlock& block)
	{
		auto it = mBlockToIndex.find(block);
		if (it != mBlockToIndex.end())
		{
			return it->second;
		}
		else
		{
			int index = mPallet.size();
			mPallet.push_back({ index, block });
			mBlockToIndex[block] = index;
			return index;
		}
	}
	int StructurePalette::GetBlockReferenceIndex(const MCBlock& block) const
	{
		auto it = mBlockToIndex.find(block);
		if (it != mBlockToIndex.end())
		{
			return it->second;
		}
		else
		{
			return -1;
		}

	}
	const MCBlock& StructurePalette::GetBlockByIndex(int index) const
	{
		return mPallet[index].second;
	}
	void StructurePalette::WriteToNBT(NBT::TagCompound& tag) const
	{
		NBT::TagCompound palletTag{};
		NBT::TagCompound defaultTag{};
		NBT::TagCompound blockPositionDataTag{};
		NBT::TagList palletList{};
		std::vector<NBT::TagCompound> palletListVec{};
		palletListVec.resize(mPallet.size());
		for (const auto& [index, block] : mPallet)
		{
			NBT::TagCompound blockTag{};
			block.WriteToNBT(blockTag);
			palletListVec[index] = std::move(blockTag);
		}
		palletList.AbsorbTagArray(palletListVec);
		defaultTag["block_palette"] = std::move(palletList);
		defaultTag["block_position_data"] = std::move(blockPositionDataTag);
		palletTag["default"] = std::move(defaultTag);
		tag["palette"] = std::move(palletTag);
	}
}