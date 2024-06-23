#include "Texture.hpp"
#include <fstream>
#include <filesystem>
#include <endianness.h>

namespace AgeAPI::Backend::Rp
{

	TextureLayer::TextureLayer(const std::string& path, bool lazyLoad) : mIsLazyLoaded(lazyLoad)
	{
		if (lazyLoad)
			mPath = path;
		else
			readToMemory(path);

	}
	void TextureLayer::Write(const std::string& path)
	{
		if (mIsLazyLoaded) // If lazy loaded we can just copy the file with no read to memory
		{
			std::filesystem::path sourcePath = mPath;
			std::filesystem::path destPath = path;

			try // TODO: Change to Error as value
			{
				std::filesystem::copy_file(sourcePath, destPath, std::filesystem::copy_options::overwrite_existing);
			}
			catch (std::exception& e)
			{
				throw std::runtime_error("Failed to copy file: " + std::string(e.what()));
			}
			return;
		}

		std::vector<std::unique_ptr<u8[]>> fileData;
		switch (mBitDepth)
		{
		case 8:
			fileData = std::move(this->writeNBit<u8>());
			break;
		case 16:
			fileData = std::move(this->writeNBit<u16>());
			break;
		}

		FILE* file = fopen(path.c_str(), "wb");
		if (!file)
			throw std::runtime_error("Failed to open file: " + path);

		png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		if (!png)
			throw std::runtime_error("Failed to create png write struct");

		png_infop info = png_create_info_struct(png);
		if (!info)
			throw std::runtime_error("Failed to create png info struct");

		if (setjmp(png_jmpbuf(png)))
			throw std::runtime_error("Failed to set jump buffer");

		png_init_io(png, file);
		png_set_IHDR(
			png,
			info,
			mSize.x,
			mSize.y,
			mBitDepth,
			mColorType,
			mInterlacing,
			mCompressionType,
			mFilterType
		);

		png_write_info(png, info);
		png_set_swap(png);

		png_write_image(png, (u8**)fileData.data());
		png_write_end(png, nullptr);

		png_destroy_write_struct(&png, &info);
		fclose(file);
	}
	void TextureLayer::Resize(IVec2 newSize)
	{
		handleLazyWrite();
		TextureLayer newSelf(newSize, mBitDepth, {0.f, 0.f, 0.f, 0.f}, mColorType, mInterlacing, mFilterType, mCompressionType);
		auto newY = std::min(mSize.y, newSize.y);
		auto newX = std::min(mSize.x, newSize.x);
		for (int y = 0; y < newY; y++)
			for (int x = 0; x < newX; x++)
				newSelf[{x, y}] = (*this)[{x, y}];
		*this = std::move(newSelf);
	}
	void TextureLayer::readToMemory(const std::string& path)
	{
		FILE* file = fopen(path.c_str(), "rb"); // Dislike using C functions but tbh i dont wanna deal with figuring out how to get lib png to work with them
		if (!file)
			throw std::runtime_error("Failed to open file: " + path);

		png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		if (!png)
			throw std::runtime_error("Failed to create png read struct");

		png_infop info = png_create_info_struct(png);
		if (!info)
			throw std::runtime_error("Failed to create png info struct");

		if (setjmp(png_jmpbuf(png)))
			throw std::runtime_error("Failed to set jump buffer");

		png_init_io(png, file);
		png_read_info(png, info);

		mSize.x = png_get_image_width(png, info);
		mSize.y = png_get_image_height(png, info);
		mColorType = png_get_color_type(png, info);
		mBitDepth = png_get_bit_depth(png, info);
		mInterlacing = png_get_interlace_type(png, info);
		mFilterType = png_get_filter_type(png, info);
		mCompressionType = png_get_compression_type(png, info);
		if (mBitDepth > 16)
			throw std::runtime_error("Bit depth is too high");
		mData.resize(mSize.x * mSize.y * sizeof(float)); // 4 bytes per pixel


		png_set_swap(png);

		if (mColorType == PNG_COLOR_TYPE_PALETTE)
			png_set_palette_to_rgb(png);

		if (mColorType == PNG_COLOR_TYPE_GRAY && mBitDepth < 8)
			png_set_expand_gray_1_2_4_to_8(png);

		if (png_get_valid(png, info, PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(png);

		if (mColorType == PNG_COLOR_TYPE_RGB || mColorType == PNG_COLOR_TYPE_GRAY || mColorType == PNG_COLOR_TYPE_PALETTE)
			png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

		if (mColorType == PNG_COLOR_TYPE_GRAY || mColorType == PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_to_rgb(png);

		//png_set_swap(png);
		png_read_update_info(png, info);

		std::vector<std::unique_ptr<u8[]>> data(mSize.y);
		for (int i = 0; i < mSize.y; i++)
			data[i] = std::unique_ptr<u8[]>(new u8[png_get_rowbytes(png, info)]);

		png_read_image(png, (u8**)data.data()); // A little hacky but it works :3

		switch (mBitDepth)
		{
		case 8:
			this->readNBit<u8>(data);
			break;
		case 16:
			// Demon shit lmao
			this->readNBit<u16>(*reinterpret_cast<std::vector<std::unique_ptr<u16[]>>*>(&data));
			break;
		}

		png_destroy_read_struct(&png, &info, nullptr);


	}
}