#include <AgeAPI/internal/NBT/NBTFile.hpp>
#include <fstream>
#include <zlib.h>
namespace AgeAPI::NBT
{
	NBTFile::NBTFile(const std::string& path)
	{
		readFromFile(path);
	}
	NBTFile::NBTFile(std::vector<u8>& stream)
	{
		readFromBuffer(stream);
	}
	void NBTFile::WriteToFile(const std::string& path)
	{
		// Make parent directories if they don't exist
		std::filesystem::create_directories(std::filesystem::path(path).parent_path());

		std::ofstream file(path);
		if (!file.is_open())
			throw std::runtime_error("Failed to open file");

		BinaryStream<u8> stream;
		if (!mHasBedrockHeader)
		{
			stream.Write<u8>(10); // NBT file type
			stream.WriteBuff(std::span<u8>{(u8*)this->mRootName.data(), mRootName.size()});
		}
		else
		{
			stream.Write({ (u8*)mBedrockHeader.data(), sizeof(mBedrockHeader)});
		}
		this->mRootTag.WriteTag(stream);
	}
	void NBTFile::readFromFile(const std::string& path)
	{
		std::ifstream file(path, std::ios::binary);
		if (!file.is_open())
			throw std::runtime_error("Failed to open file");

		file.seekg(0, std::ios::end);
		size_t size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<u8> buffer(size);
		file.read(reinterpret_cast<char*>(buffer.data()), size);
		file.close();
		readFromBuffer(buffer);
	}

	void NBTFile::readFromBuffer(std::vector<u8>& buffer)
	{
		detectAndHandleCompression(buffer);

		BinaryStream<u8> stream(buffer);
		auto tagType = stream.Read<u8>();
		if (tagType < 9)
			throw std::runtime_error("Invalid NBT file");
		else if (tagType == 9)
			throw std::runtime_error("Unimplemented NBT file type");
		else if (tagType > 10)
			throw std::runtime_error("Invalid NBT file");

		auto RootName = TagString().ReadTag(stream);
		mRootName = std::move(RootName.GetValue());

		this->mRootTag = TagCompound().ReadTag(stream);
		if (this->mRootTag.size() == 0 && buffer.size() > 11)
		{
			stream.GoTo(11); // Skip the bedrock header and read the first index this should only happen on level.dat from bedrock?
			this->mRootTag = TagCompound().ReadTag(stream);
			if (mRootTag.size() != 0)
				mHasBedrockHeader = true;
		}

	}
	void NBTFile::detectAndHandleCompression(std::vector<u8>& buffer)
	{
		//Chec for the zip and gzip headers and decompress if needed

		if (buffer.size() > 2)
		{
			if (buffer[0] == 0x1F && buffer[1] == 0x8B)
			{
				mCompressionType = CompressionType::GZip;
				readGZip(buffer);
			}
			else if (buffer[0] == 0x78 && buffer[1] == 0x9C)
			{
				mCompressionType = CompressionType::ZLib;
				readZLib(buffer);
			}
		}
	}
	void NBTFile::readGZip(std::vector<u8>& buffer)
	{
		std::vector<u8> outBuffer;
		outBuffer.resize(1024 * 1024 * 5);
		z_stream stream;
		stream.zalloc = Z_NULL;
		stream.zfree = Z_NULL;
		stream.opaque = Z_NULL;
		stream.avail_in = buffer.size();
		stream.next_in = buffer.data();
		stream.avail_out = outBuffer.size();
		stream.next_out = outBuffer.data();

		auto ret = inflateInit2(&stream, 16 + MAX_WBITS);
		if (ret != Z_OK)
			throw std::runtime_error("Failed to initialize zlib");

		ret = inflate(&stream, Z_FINISH);
		if (ret != Z_STREAM_END)
			throw std::runtime_error("Failed to decompress zlib");

		ret = inflateEnd(&stream);
		if (ret != Z_OK)
			throw std::runtime_error("Failed to end zlib");

		buffer = outBuffer;
	}
	void NBTFile::readZLib(std::vector<u8>& buffer)
	{
		std::vector<u8> outBuffer;
		outBuffer.resize(1024 * 1024 * 5);
		z_stream stream;
		stream.zalloc = Z_NULL;
		stream.zfree = Z_NULL;
		stream.opaque = Z_NULL;
		stream.avail_in = buffer.size();
		stream.next_in = buffer.data();
		stream.avail_out = outBuffer.size();
		stream.next_out = outBuffer.data();

		auto ret = inflateInit(&stream);
		if (ret != Z_OK)
			throw std::runtime_error("Failed to initialize zlib");

		ret = inflate(&stream, Z_FINISH);
		if (ret != Z_STREAM_END)
			throw std::runtime_error("Failed to decompress zlib");

		ret = inflateEnd(&stream);
		if (ret != Z_OK)
			throw std::runtime_error("Failed to end zlib");

		buffer = outBuffer;
	}
}