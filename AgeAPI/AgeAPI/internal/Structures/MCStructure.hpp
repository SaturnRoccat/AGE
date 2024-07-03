#pragma once
#include <AgeAPI/internal/Structures/MCPallet.hpp>

namespace AgeAPI::Structure
{
	class MCStructure
	{
	public:
		MCStructure() = default;
		MCStructure(const IVec3& size, const std::string& structureName) : 
			mSize(size),
			mStructureName(structureName),
			mBlockIndices(size.x * size.y * size.z),
			mBlockSpecialIndices(size.x * size.y * size.z, -1) {}

		void Resize(const IVec3& newSize);
		void SetSpecialBlock(const IVec3& position, int specialIndex);

		int& operator[](const IVec3& position)
		{
			return getBlockAt(position);
		}

		void WriteToNBT(NBT::TagCompound& tag) const;
		void WriteToFile(const std::string& path) const;

		int RegisterBlock(const MCBlock& block)
		{
			return mPallet.RegisterBlock(block);
		}
		int GetBlockReferenceIndex(const MCBlock& block) const
		{
			return mPallet.GetBlockReferenceIndex(block);
		}
		const MCBlock& GetBlockByIndex(int index) const
		{
			return mPallet.GetBlockByIndex(index);
		}
	
		IVec3 GetSize() const { return mSize; }

		auto begin() { return mBlockIndices.begin(); }
		auto end() { return mBlockIndices.end(); }

	private:
		inline int getBlockIndex(const IVec3& position) const 
		{
			// Bedrock uses ZYX order
			return position.z * mSize.x * mSize.y + position.y * mSize.x + position.x;
		}
		inline int& getBlockAt(const IVec3& position)
		{
			return mBlockIndices[getBlockIndex(position)];
		}
	private:
		constexpr static int mFormatVersion{1};
		StructurePalette mPallet{};
		IVec3 mSize{ 5, 5, 5 };
		IVec3 mStructureWorldOrigin{ 0, 0, 0 };
		std::string mStructureName{"goobah"};

		std::vector<int> mBlockIndices{};
		std::vector<int> mBlockSpecialIndices{};

		// std::vector<MCEntity> mEntities{}; // FIXME: This needs to be a real object

			
	};

}