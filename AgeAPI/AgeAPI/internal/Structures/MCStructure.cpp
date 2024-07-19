#include <AgeAPI/internal/Structures/MCStructure.hpp>
#include <filesystem>

namespace AgeAPI::Structure
{
	void MCStructure::Resize(const IVec3& newSize)
	{
		MCStructure newStructure(newSize, mStructureName);
		for (int x = 0; x < std::min(mSize.x, newSize.x); ++x)
			for (int y = 0; y < std::min(mSize.y, newSize.y); ++y)
				for (int z = 0; z < std::min(mSize.z, newSize.z); ++z)
					newStructure[{x, y, z}] = getBlockAt({x, y, z});
		this->mBlockIndices = std::move(newStructure.mBlockIndices);
		this->mSize = newSize;
		// Kinda troll method to resize but it works :3
	}
	void MCStructure::SetSpecialBlock(const IVec3& position, int specialIndex)
	{
		mBlockSpecialIndices[getBlockIndex(position)] = specialIndex;
	}
	void MCStructure::WriteToNBT(NBT::TagCompound& tag) const
	{
		tag["size"] = NBT::TagList(std::vector<NBT::TagInt>{ NBT::TagInt(mSize.x), NBT::TagInt(mSize.y), NBT::TagInt(mSize.z) });
		tag["structure_world_origin"] = NBT::TagList(
			std::vector<NBT::TagInt>{
			NBT::TagInt(mStructureWorldOrigin.x),
				NBT::TagInt(mStructureWorldOrigin.y),
				NBT::TagInt(mStructureWorldOrigin.z)
		});
		tag["format_version"] = NBT::TagInt(mFormatVersion);
		NBT::TagCompound structureTag{};

		mPallet.WriteToNBT(structureTag);

		NBT::TagList entitiesList{};

		NBT::TagList blockIndicesList{};
		blockIndicesList.SetType(NBT::TagListType{});

		NBT::TagList blockSpecialIndicesList{};
		std::vector<NBT::TagInt> blockSpecialIndicesVec{};
		blockSpecialIndicesVec = *(std::vector<NBT::TagInt>*)(&mBlockSpecialIndices);

		std::vector<NBT::TagInt> blockIndicesVec{};
		blockIndicesVec = *(std::vector<NBT::TagInt>*)(&mBlockIndices);
		// This looks unsafe but it should be totally fine since under the hood itll have the same ABI

		NBT::TagList blockIndicesListVecNBT{};
		NBT::TagList blockSpecialIndicesListVecNBT{};
		blockIndicesListVecNBT.AbsorbTagArray(blockIndicesVec);
		blockSpecialIndicesListVecNBT.AbsorbTagArray(blockSpecialIndicesVec);
		blockIndicesList.EmplaceBack<NBT::TagList>(std::move(blockIndicesListVecNBT));
		blockIndicesList.EmplaceBack<NBT::TagList>(std::move(blockSpecialIndicesListVecNBT));



		structureTag["block_indices"] = std::move(blockIndicesList);
		structureTag["entities"] = std::move(entitiesList);
		tag["structure"] = std::move(structureTag);
	}
	void MCStructure::WriteToFile(const std::string& path) const
	{
		NBT::TagCompound tag{};
		WriteToNBT(tag);
		NBT::NBTFile file{ std::move(tag)};
		std::filesystem::path p{path};
		if (p.has_extension())
			p.replace_extension();
		p /= mStructureName + ".mcstructure";

		file.WriteToFile(p.string());

	}
}
