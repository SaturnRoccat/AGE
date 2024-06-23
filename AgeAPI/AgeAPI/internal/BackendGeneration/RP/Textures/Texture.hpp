#pragma once
#include <png.h>
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/Math/BresenhamLine.hpp>
#include <algorithm>

namespace AgeAPI::Backend::Rp
{
	struct Color
	{
		float r, g, b, a;

		Color operator*(float factor) const { return { r * factor, g * factor, b * factor, a * factor }; }
		Color operator*(const Color& other) const { return { r * other.r, g * other.g, b * other.b, a * other.a }; }
		Color operator+(const Color& other) const { return { r + other.r, g + other.g, b + other.b, a + other.a }; }
		Color operator-(const Color& other) const { return { r - other.r, g - other.g, b - other.b, a - other.a }; }
		Color operator/(float factor) const { return { r / factor, g / factor, b / factor, a / factor }; }
		Color operator/(const Color& other) const { return { r / other.r, g / other.g, b / other.b, a / other.a }; }
		Color operator-() const { return { -r, -g, -b, -a }; }
		Color& operator*=(float factor) { r *= factor; g *= factor; b *= factor; a *= factor; return *this; }
		Color& operator*=(const Color& other) { r *= other.r; g *= other.g; b *= other.b; a *= other.a; return *this; }
		Color& operator+=(const Color& other) { r += other.r; g += other.g; b += other.b; a += other.a; return *this; }
		Color& operator-=(const Color& other) { r -= other.r; g -= other.g; b -= other.b; a -= other.a; return *this; }
		Color& operator/=(float factor) { r /= factor; g /= factor; b /= factor; a /= factor; return *this; }
		Color& operator/=(const Color& other) { r /= other.r; g /= other.g; b /= other.b; a /= other.a; return *this; }
		
	};

	class TextureLayer
	{
	public:
		TextureLayer(const std::string& path, bool lazyLoad = true); // By default we only load into memory on write and if we never write we just copy from the disk 
		TextureLayer(u8 colorType, u8 bitDepth, u8 interlacing, u8 filterType, u8 compressionType, const std::vector<Color>& data, IVec2 size) : mColorType(colorType), mBitDepth(bitDepth), mInterlacing(interlacing), mFilterType(filterType), mCompressionType(compressionType), mData(data), mSize(size) {}
		TextureLayer(
			IVec2 size,
			u8 bitDepth = 8,
			Color fillColor = { 0.f, 0.f, 0.f, 0.f },
			u8 colorType = PNG_COLOR_TYPE_RGBA,
			u8 interlacing = PNG_INTERLACE_NONE,
			u8 filterType = PNG_FILTER_TYPE_BASE,
			u8 compressionType = PNG_COMPRESSION_TYPE_BASE
		) : mSize(size), mBitDepth(bitDepth), mColorType(colorType), mInterlacing(interlacing), mFilterType(filterType), mCompressionType(compressionType), mData(size.x* size.y)
		{
			Fill(fillColor);
		}

		TextureLayer() = default;

		void Write(const std::string& path);

		Color& operator[](IVec2 pos) { return mData[pos.x + pos.y * mSize.x]; }
		Color& operator[](i32 index) { return mData[index]; }

		const Color& operator[](IVec2 pos) const { return mData[pos.x + pos.y * mSize.x]; }
		const Color& operator[](i32 index) const { return mData[index]; }

		Color& At(IVec2 pos) { return mData[pos.x + pos.y * mSize.x]; }
		Color& At(i32 index) { return mData[index]; }

		const Color& At(IVec2 pos) const { return mData[pos.x + pos.y * mSize.x]; }
		const Color& At(i32 index) const { return mData[index]; }

		void Fill(Color color) { handleLazyWrite();  std::fill(mData.begin(), mData.end(), color); }
		void FillRow(i32 row, Color color) { handleLazyWrite(); std::fill(mData.begin() + row * mSize.x, mData.begin() + (row + 1) * mSize.x, color); }
		void FillColumn(i32 column, Color color) { handleLazyWrite(); for (i32 y = 0; y < mSize.y; y++) At({ column, y }) = color; }
		
		void Multiply(Color color) 
		{ 
			handleLazyWrite();
			std::transform(mData.begin(), mData.end(), mData.begin(), [&color](Color c) { return c * color; }); 
		}
		void MultiplyRow(i32 row, Color color) {
			handleLazyWrite();
			std::transform(
				mData.begin() + row * mSize.x,
				mData.begin() + (row + 1) * mSize.x, mData.begin() + row * mSize.x,
				[&color](Color c) { return c * color;});
		}
		void MultiplyColumn(i32 column, Color color) {
			handleLazyWrite();
			for (i32 y = 0; y < mSize.y; y++)
			{
				At({ column, y }) *= color;
			}
		}
		void Multiply(const TextureLayer& other) {
			handleLazyWrite();
			for (i32 i = 0; i < mData.size(); i++)
				mData[i]*= other[i];
		}
		void Multiply(const TextureLayer& other, float factor) {
			handleLazyWrite();
			for (i32 i = 0; i < mData.size(); i++)
				mData[i]*= other[i] * factor;
		}
		void DrawLine(IVec2 begin, IVec2 end, Color color, int thickness = 1)
		{
			handleLazyWrite();
			BresenHamLine<IVec2>(begin, end, [this, &color, thickness](IVec2 pos) {
				if (thickness == 1)
					SafeSet(pos, color);
				else
					for (int y = -thickness / 2; y < thickness / 2; y++)
						for (int x = -thickness / 2; x < thickness / 2; x++)
							SafeSet(pos + IVec2{ x, y }, color);
			});
		}
		void DrawLineUnsafe(IVec2 begin, IVec2 end, Color color, int thickness = 1)
		{
			handleLazyWrite();
			BresenHamLine<IVec2>(begin, end, [this, &color, thickness](IVec2 pos) {
				if (thickness == 1)
					UnsafeSet(pos, color);
				else
					for (int y = -thickness / 2; y < thickness / 2; y++)
						for (int x = -thickness / 2; x < thickness / 2; x++)
							UnsafeSet(pos + IVec2{ x, y }, color);
				});
		}
		void DrawCircleOutline(IVec2 center, i32 radius, Color col, i32 thickness = 1)
		{
			handleLazyWrite();
			for (i32 x = -radius; x < radius; x++)
				for (i32 y = -radius; y < radius; y++)
				{
					int distSq = x * x + y * y;
					if (distSq < radius * radius && distSq >= (radius - thickness) * (radius - thickness))
						SafeSet(center + IVec2{ x, y }, col);
				}
		}
		void DrawCircleOutlineUnsafe(IVec2 center, i32 radius, Color col, i32 thickness = 1)
		{
			handleLazyWrite();
			for (i32 x = -radius; x < radius; x++)
				for (i32 y = -radius; y < radius; y++)
				{
					int distSq = x * x + y * y;
					if (distSq < radius * radius && distSq >= (radius - thickness) * (radius - thickness))
						UnsafeSet(center + IVec2{ x, y }, col);
				}
		}
		void DrawCircle(IVec2 center, i32 rad, Color col)
		{
			handleLazyWrite();
			for (i32 x = -rad; x < rad; x++)
				for (i32 y = -rad; y < rad; y++)
					if (x * x + y * y < rad * rad)
						SafeSet(center + IVec2{ x, y }, col);
		}
		void DrawCircleUnsafe(IVec2 center, i32 rad, Color col)
		{
			handleLazyWrite();
			for (i32 x = -rad; x < rad; x++)
				for (i32 y = -rad; y < rad; y++)
					if (x * x + y * y < rad * rad)
						UnsafeSet(center + IVec2{ x, y }, col);
		}
		void DrawRectangleOutline(IVec2 pos, IVec2 size, Color col, i32 thickness = 1)
		{
			handleLazyWrite();
			for (i32 x = 0; x < size.x; x++)
				for (i32 y = 0; y < size.y; y++)
					if (x < thickness || y < thickness || x >= size.x - thickness || y >= size.y - thickness)
						SafeSet(pos + IVec2{ x, y }, col);
		}
		void DrawRectangleOutlineUnsafe(IVec2 pos, IVec2 size, Color col, i32 thickness = 1)
		{
			handleLazyWrite();
			for (i32 x = 0; x < size.x; x++)
				for (i32 y = 0; y < size.y; y++)
					if (x < thickness || y < thickness || x >= size.x - thickness || y >= size.y - thickness)
						UnsafeSet(pos + IVec2{ x, y }, col);
		}
		void DrawRectangle(IVec2 pos, IVec2 size, Color col)
		{
			handleLazyWrite();
			for (i32 x = 0; x < size.x; x++)
				for (i32 y = 0; y < size.y; y++)
					SafeSet(pos + IVec2{ x, y }, col);
		}
		void DrawRectangleUnsafe(IVec2 pos, IVec2 size, Color col)
		{
			handleLazyWrite();
			for (i32 x = 0; x < size.x; x++)
				for (i32 y = 0; y < size.y; y++)
					UnsafeSet(pos + IVec2{ x, y }, col);
		}

		IVec2 GetSize() const { return mSize; }
		i32 GetWidth() const { return mSize.x; }
		i32 GetHeight() const { return mSize.y; }
		u8 GetBitDepth() const { return mBitDepth; }
		u8 GetColorType() const { return mColorType; }
		u8 GetInterlacing() const { return mInterlacing; }
		u8 GetFilterType() const { return mFilterType; }
		u8 GetCompressionType() const { return mCompressionType; }

		void Resize(IVec2 newSize);
		void Resize(i32 newWidth, i32 newHeight) { Resize(IVec2{ newWidth, newHeight }); }

		void SetColorType(u8 colorType) { mColorType = colorType; }
		void SetBitDepth(u8 bitDepth) { mBitDepth = bitDepth; }
		void SetInterlacing(u8 interlacing) { mInterlacing = interlacing; }
		void SetFilterType(u8 filterType) { mFilterType = filterType; }
		void SetCompressionType(u8 compressionType) { mCompressionType = compressionType; }
		void SafeSet(IVec2 pos, Color color) 
		{
			BoundingBox<IVec2> b = BoundingBox<IVec2>{ {0, 0}, mSize - 1 };
			if (!b.Contains(pos)) [[unlikely]]
				return;

			auto& col = At(pos);
			col = color;
		}
		void UnsafeSet(IVec2 pos, Color color) { At(pos) = color; }
	private:
		void readToMemory(const std::string& path);
		void handleLazyWrite() 
		{
			if (mIsLazyLoaded) [[unlikely]]
			{
				mIsLazyLoaded = false;
				readToMemory(mPath);
			}
		}
		template<std::integral T>
		void readNBit(const std::vector<std::unique_ptr<T[]>>& pixelData)
		{
			mData.resize(mSize.x * mSize.y);
			for (i32 y = 0; y < mSize.y; y++)
			{
				const T* pixelRow = reinterpret_cast<const T*>(pixelData[y].get());
				for (i32 x = 0; x < mSize.x; x++)
				{
					auto* colorDataOfRow = pixelRow + x * 4; 
					// Assuming RGBA format which can be upheld due to libpng conversion
					Color& color = mData[x + y * mSize.x];
					color.r = static_cast<float>(colorDataOfRow[0]) / std::numeric_limits<T>::max();
					color.g = static_cast<float>(colorDataOfRow[1]) / std::numeric_limits<T>::max();
					color.b = static_cast<float>(colorDataOfRow[2]) / std::numeric_limits<T>::max();
					color.a = static_cast<float>(colorDataOfRow[3]) / std::numeric_limits<T>::max();
				}
			}
		}

		template<std::integral T>
		std::vector<std::unique_ptr<u8[]>> writeNBit()
		{
			auto BaseWrite = [&]<size_t colorCount>(){
				std::vector<std::unique_ptr<T[]>> pixelData;
				pixelData.resize(mSize.y);
				for (i32 y = 0; y < mSize.y; y++)
				{
					void* data = new T[mSize.x * colorCount];
#ifndef NDEBUG
					memset(data, 0xAB, mSize.x * colorCount);
#endif
					pixelData[y] = std::unique_ptr<T[]>((T*)data);
					for (i32 x = 0; x < mSize.x; x++)
					{
						std::unique_ptr<T[]>& pixelRow = pixelData[y];
						std::array<T, colorCount>* colorDataOfRow = ((std::array<T, colorCount>*)pixelRow.get()) + x; // RGBA format this needs to be updated lol
						std::array<float, colorCount>* colorData = (std::array<float, colorCount>*) & mData[x + y * mSize.x];
						std::transform(
							colorData->begin(),
							colorData->end(),
							colorDataOfRow->begin(),
							[](float val)
							{ return (T)(val * (float)std::numeric_limits<T>::max()); }
						);



					}
				}
				return std::move(*(std::vector<std::unique_ptr<u8[]>>*) & pixelData);
			};
			switch (this->mColorType)
			{
			case PNG_COLOR_TYPE_RGBA:
				return BaseWrite.operator()<4>();
			case PNG_COLOR_TYPE_RGB:
				return BaseWrite.operator()<3>();
			case PNG_COLOR_TYPE_GRAY:
				return BaseWrite.operator()<1>();
			case PNG_COLOR_TYPE_GRAY_ALPHA:
				return BaseWrite.operator()<2>();
			default:
				throw std::runtime_error("Unsupported color type");
			}
				
		}
	private:
		bool mIsLazyLoaded = false;
		std::string mPath = "";
		u8 mColorType{};
		u8 mBitDepth{};
		u8 mInterlacing{};
		u8 mFilterType{};
		u8 mCompressionType{};
		std::vector<Color> mData{};
		IVec2 mSize{ 0, 0 };


	};

	

	class Texture
	{
	private:
		constexpr static u8 mFilterType = PNG_FILTER_TYPE_BASE;
		constexpr static u8 mCompressionType = PNG_COMPRESSION_TYPE_DEFAULT;
		constexpr static u8 mInterlacing = PNG_INTERLACE_NONE;

		std::vector<TextureLayer> mLayers{ 4 };
		IVec2 mSize;
		u8 mBitDepth;
		u8 mColorType;



	};

}