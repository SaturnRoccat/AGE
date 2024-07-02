#pragma once
#include <AgeAPI/internal/NBT/Tags.hpp>
#include <concepts>


namespace AgeAPI::NBT
{
	enum class CompressionType
	{
		None,
		GZip,
		ZLib
	};

	class NBTFile
	{
	private:
		TagCompound mRootTag{};
		std::string mRootName{};
		CompressionType mCompressionType{ CompressionType::None};
	public:
		NBTFile() {};
		NBTFile(const std::string& path);
		NBTFile(std::vector<u8>& stream);


		void SetRootName(const std::string& name) { mRootName = name; }
		const std::string& GetRootName() const { return mRootName; }

		TagCompound& GetRootTag() { return mRootTag; }
		const TagCompound& GetRootTag() const { return mRootTag; }

		CompressionType GetCompressionType() const { return mCompressionType; }
		CompressionType SetCompressionType(CompressionType type) { return mCompressionType = type; }

	

		template<typename T>
		T& GetTag(const TagString& name)
		{
			return mRootTag.GetTag<T>(name);
		}


		auto& operator[](const TagString& name) { return mRootTag[name]; }



		void Print() const { mRootTag.PrintTag(); }
	private:
		void readFromFile(const std::string& path);

		void readFromBuffer(std::vector<u8>& stream);
		void detectAndHandleCompression(std::vector<u8>&);

		// TODO: Implement these
		void readGZip(std::vector<u8>&);
		void readZLib(std::vector<u8>&);
	};

}