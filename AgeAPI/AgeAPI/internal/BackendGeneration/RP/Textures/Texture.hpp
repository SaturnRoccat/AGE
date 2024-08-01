#pragma once
#include <png.h>
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/Math/BresenhamLine.hpp>
#include <algorithm>
#include <print>
#include <span>
namespace AgeAPI::Backend::Rp
{

	class TextureLayer
	{
	public:
		TextureLayer(const std::string& path, bool lazyLoad = true); // By default we only load into memory on write and if we never write we just copy from the disk 
		TextureLayer(u8 colorType, u8 bitDepth, u8 interlacing, u8 filterType, u8 compressionType, const std::vector<Color>& data, I16Vec2 size) : mColorType(colorType), mBitDepth(bitDepth), mInterlacing(interlacing), mFilterType(filterType), mCompressionType(compressionType), mData(data), mSize(size) {}
		TextureLayer(
			I16Vec2 size,
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
		TextureLayer(const TextureLayer& bottom, const TextureLayer& Top, float alpha);


		TextureLayer() = default;

		TextureLayer& Write(const std::filesystem::path& path);

		Color& operator[](I16Vec2 pos) { handleLazyWrite(); return mData[pos.x + pos.y * mSize.x]; }
		Color& operator[](i16 index) { handleLazyWrite(); return mData[index]; }

		const Color& operator[](I16Vec2 pos) const { return mData[pos.x + pos.y * mSize.x]; }
		const Color& operator[](i16 index) const { return mData[index]; }

		Color& At(I16Vec2 pos) { handleLazyWrite(); return mData[pos.x + pos.y * mSize.x]; }
		Color& At(i16 index) { handleLazyWrite(); return mData[index]; }

		const Color& At(I16Vec2 pos) const { return mData[pos.x + pos.y * mSize.x]; }
		const Color& At(i16 index) const { return mData[index]; }

		TextureLayer& Fill(Color color) { handleLazyWrite();  std::fill(mData.begin(), mData.end(), color); return *this;}
		TextureLayer& FillRow(i16 row, Color color) { handleLazyWrite(); std::fill(mData.begin() + row * mSize.x, mData.begin() + (row + 1) * mSize.x, color); return *this;}
		TextureLayer& FillColumn(i16 column, Color color) { handleLazyWrite(); for (i16 y = 0; y < mSize.y; y++) At({ column, y }) = color; return *this;}
		TextureLayer& FillGradientTopDown(Color start, Color end) {
			handleLazyWrite();
			for (i16 y = 0; y < mSize.y; y++)
				FillRow(y, start.Lerp(end, (float)y / (float)mSize.y));
			return *this;
		}
		TextureLayer& FillGradientLeftRight(Color start, Color end) {
			handleLazyWrite();
			for (i16 x = 0; x < mSize.x; x++)
				FillColumn(x, start.Lerp(end, (float)x / (float)mSize.x));
			return *this;
		}
		// FIXME: Does not function corectly
		TextureLayer& FillGradientAngle(Color start, Color end, float angle)
		{ 
			handleLazyWrite();
			for (i16 y = 0; y < mSize.y; y++)
				for (i16 x = 0; x < mSize.x; x++)
					At({ x, y }) = start.Lerp(end, (float)(x * std::cos(angle) + y * std::sin(angle)) / (float)mSize.x /*handle the fact that rotation makes the distance longer*/);
			return *this;
		}
		TextureLayer& FillGradientRow(i16 row, Color start, Color end) { handleLazyWrite(); for (i16 x = 0; x < mSize.x; x++) At({ x, row }) = start.Lerp(end, (float)x / (float)mSize.x); return *this;
		}
		TextureLayer& FillGradientColumn(i16 column, Color start, Color end) { handleLazyWrite(); for (i16 y = 0; y < mSize.y; y++) At({ column, y }) = start.Lerp(end, (float)y / (float)mSize.y); return *this; }
		
		TextureLayer& GrayScale() { handleLazyWrite(); std::transform(mData.begin(), mData.end(), mData.begin(), [](Color c) { return c.GrayScale(); }); return *this; }

		TextureLayer& Roate90()
		{
			handleLazyWrite();
			for (i16 y = 0; y < mSize.y; y++)
			{
				auto row = std::span<Color>(&mData[y * mSize.x], mSize.x);
				std::reverse(row.begin(), row.end());
			}
			return *this;
		}
			
		TextureLayer& Multiply(Color color)
		{ 
			handleLazyWrite();
			std::transform(mData.begin(), mData.end(), mData.begin(), [&color](Color c) { return c * color; }); 
			return *this;
		}
		TextureLayer& Multiply(const TextureLayer& other) {
			handleLazyWrite();
			for (i16 i = 0; i < mData.size(); i++)
				mData[i]*= other[i];
			return *this;
		}
		TextureLayer& Multiply(const TextureLayer& other, float factor) {
			handleLazyWrite();
			for (i16 i = 0; i < mData.size(); i++)
				mData[i]*= other[i] * factor;
			return *this;
		}
		TextureLayer& Multiply(Color start, Color end) {
			handleLazyWrite();
			for (i16 i = 0; i < mData.size(); i++)
				mData[i] *= start.Lerp(end, (float)i / (float)mData.size());
			return *this;
		}
		TextureLayer& MultiplyRow(i16 row, Color color) {
			handleLazyWrite();
			std::transform(
				mData.begin() + row * mSize.x,
				mData.begin() + (row + 1) * mSize.x, mData.begin() + row * mSize.x,
				[&color](Color c) { return c * color;});
			return *this;
		}
		TextureLayer& MultiplyColumn(i16 column, Color color) {
			handleLazyWrite();
			for (i16 y = 0; y < mSize.y; y++)
				At({ column, y }) *= color;
			return *this;
		}
		TextureLayer& DrawLine(I16Vec2 begin, I16Vec2 end, Color color, i16 thickness = 1)
		{
			handleLazyWrite();
			BresenHamLine<I16Vec2>(begin, end, [this, &color, thickness](I16Vec2 pos) {
				if (thickness == 1)
					SafeSet(pos, color);
				else
					for (i16 y = -thickness / 2; y < thickness / 2; y++)
						for (i16 x = -thickness / 2; x < thickness / 2; x++)
							SafeSet(pos + I16Vec2{ x, y }, color);
			});
			return *this;
		}
		TextureLayer& DrawLineUnsafe(I16Vec2 begin, I16Vec2 end, Color color, i16 thickness = 1)
		{
			handleLazyWrite();
			BresenHamLine<I16Vec2>(begin, end, [this, &color, thickness](I16Vec2 pos) {
				if (thickness == 1)
					UnsafeSet(pos, color);
				else
					for (i16 y = -thickness / 2; y < thickness / 2; y++)
						for (i16 x = -thickness / 2; x < thickness / 2; x++)
							UnsafeSet(pos + I16Vec2{ x, y }, color);
				});
			return *this;
		}
		TextureLayer& DrawLineGradiant(I16Vec2 begin, I16Vec2 end, Color start, Color endColor, i16 thickness = 1)
		{
				handleLazyWrite();
				BresenHamLine<I16Vec2>(begin, end, [this, &start, end, thickness, begin, &endColor](I16Vec2 pos) {
				if (thickness == 1)
					SafeSet(pos, start.Lerp(endColor, (float)(pos - begin).Length() / (float)(end - begin).Length()));
				else
					for (i16 y = -thickness / 2; y < thickness / 2; y++)
						for (i16 x = -thickness / 2; x < thickness / 2; x++)
							SafeSet(pos + I16Vec2{ x, y }, start.Lerp(endColor, (float)(pos - begin).Length() / (float)(end - begin).Length()));
			});
			return *this;
		}
		TextureLayer& DrawLineGradiantUnsafe(I16Vec2 begin, I16Vec2 end, Color start, Color endColor, i16 thickness = 1)
		{
			handleLazyWrite();
			BresenHamLine<I16Vec2>(begin, end, [this, &start, end, thickness, begin, &endColor](I16Vec2 pos) {
				if (thickness == 1)
					UnsafeSet(pos, start.Lerp(endColor, (float)(pos - begin).Length() / (float)(end - begin).Length()));
				else
					for (i16 y = -thickness / 2; y < thickness / 2; y++)
						for (i16 x = -thickness / 2; x < thickness / 2; x++)
							UnsafeSet(pos + I16Vec2{ x, y }, start.Lerp(endColor, (float)(pos - begin).Length() / (float)(end - begin).Length()));
			});
			return *this;
		}
		TextureLayer& DrawCircleOutline(I16Vec2 center, i16 radius, Color col, i16 thickness = 1)
		{
			handleLazyWrite();
			for (i16 x = -radius; x < radius; x++)
				for (i16 y = -radius; y < radius; y++)
				{
					i16 distSq = x * x + y * y;
					if (distSq < radius * radius && distSq >= (radius - thickness) * (radius - thickness))
						SafeSet(center + I16Vec2{ x, y }, col);
				}
			return *this;

		}
		TextureLayer& DrawCircleOutlineUnsafe(I16Vec2 center, i16 radius, Color col, i16 thickness = 1)
		{
			handleLazyWrite();
			for (i16 x = -radius; x < radius; x++)
				for (i16 y = -radius; y < radius; y++)
				{
					i16 distSq = x * x + y * y;
					if (distSq < radius * radius && distSq >= (radius - thickness) * (radius - thickness))
						UnsafeSet(center + I16Vec2{ x, y }, col);
				}
			return *this;

		}
		TextureLayer& DrawCircle(I16Vec2 center, i16 rad, Color col)
		{
			handleLazyWrite();
			for (i16 x = -rad; x < rad; x++)
				for (i16 y = -rad; y < rad; y++)
					if (x * x + y * y < rad * rad)
						SafeSet(center + I16Vec2{ x, y }, col);
			return *this;

		}
		TextureLayer& DrawCircleUnsafe(I16Vec2 center, i16 rad, Color col)
		{
			handleLazyWrite();
			for (i16 x = -rad; x < rad; x++)
				for (i16 y = -rad; y < rad; y++)
					if (x * x + y * y < rad * rad)
						UnsafeSet(center + I16Vec2{ x, y }, col);
			return *this;

		}
		TextureLayer& DrawRectangleOutline(I16Vec2 pos, I16Vec2 size, Color col, i16 thickness = 1)
		{
			handleLazyWrite();
			for (i16 x = 0; x < size.x; x++)
				for (i16 y = 0; y < size.y; y++)
					if (x < thickness || y < thickness || x >= size.x - thickness || y >= size.y - thickness)
						SafeSet(pos + I16Vec2{ x, y }, col);
			return *this;

		}
		TextureLayer& DrawRectangleOutlineUnsafe(I16Vec2 pos, I16Vec2 size, Color col, i16 thickness = 1)
		{
			handleLazyWrite();
			for (i16 x = 0; x < size.x; x++)
				for (i16 y = 0; y < size.y; y++)
					if (x < thickness || y < thickness || x >= size.x - thickness || y >= size.y - thickness)
						UnsafeSet(pos + I16Vec2{ x, y }, col);
			return *this;

		}
		TextureLayer& DrawRectangle(I16Vec2 pos, I16Vec2 size, Color col)
		{
			handleLazyWrite();
			for (i16 x = 0; x < size.x; x++)
				for (i16 y = 0; y < size.y; y++)
					SafeSet(pos + I16Vec2{ x, y }, col);
			return *this;

		}
		TextureLayer& DrawRectangleUnsafe(I16Vec2 pos, I16Vec2 size, Color col)
		{
			handleLazyWrite();
			for (i16 x = 0; x < size.x; x++)
				for (i16 y = 0; y < size.y; y++)
					UnsafeSet(pos + I16Vec2{ x, y }, col);
			return *this;

		}
		TextureLayer& DrawTriangleOutline(I16Vec2 p1, I16Vec2 p2, I16Vec2 p3, Color col, i16 thickness = 1)
		{
			handleLazyWrite();
			DrawLine(p1, p2, col, thickness);
			DrawLine(p2, p3, col, thickness);
			DrawLine(p3, p1, col, thickness);
			return *this;

		}
		TextureLayer& DrawTriangleOutlineUnsafe(I16Vec2 p1, I16Vec2 p2, I16Vec2 p3, Color col, i16 thickness = 1)
		{
			handleLazyWrite();
			DrawLineUnsafe(p1, p2, col, thickness);
			DrawLineUnsafe(p2, p3, col, thickness);
			DrawLineUnsafe(p3, p1, col, thickness);
			return *this;

		}
		TextureLayer& DrawTriangle(I16Vec2 p1, I16Vec2 p2, I16Vec2 p3, Color col)
		{
			handleLazyWrite();
			auto min = [](i16 a, i16 b) { return a < b ? a : b; };
			auto max = [](i16 a, i16 b) { return a > b ? a : b; };
			auto IsPointInTriangle = [](I16Vec2 p, I16Vec2 a, I16Vec2 b, I16Vec2 c) -> bool {
				auto sign = [](I16Vec2 p1, I16Vec2 p2, I16Vec2 p3) -> i16 {
					return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
					};
				bool b1, b2, b3;
				b1 = sign(p, a, b) < 0.0f;
				b2 = sign(p, b, c) < 0.0f;
				b3 = sign(p, c, a) < 0.0f;
				return ((b1 == b2) && (b2 == b3));
				};
			auto edge = [&](I16Vec2 a, I16Vec2 b) {
				BresenHamLine<I16Vec2>(a, b, [this, &col](I16Vec2 pos) {
					SafeSet(pos, col);
					});
				};
			auto fill = [&](I16Vec2 a, I16Vec2 b, I16Vec2 c) {
				i16 minX = min(a.x, min(b.x, c.x));
				i16 minY = min(a.y, min(b.y, c.y));
				i16 maxX = max(a.x, max(b.x, c.x));
				i16 maxY = max(a.y, max(b.y, c.y));
				for (i16 x = minX; x <= maxX; x++)
					for (i16 y = minY; y <= maxY; y++)
					{
						I16Vec2 p{ x, y };
						if (p.x < 0 || p.y < 0 || p.x >= mSize.x || p.y >= mSize.y)
							continue;
						if (IsPointInTriangle(p, a, b, c))
							SafeSet(p, col);
					}
				};
			edge(p1, p2);
			edge(p2, p3);
			edge(p3, p1);
			fill(p1, p2, p3);
			return *this;

		}
		TextureLayer& DrawTriangleUnsafe(I16Vec2 p1, I16Vec2 p2, I16Vec2 p3, Color col)
		{
			handleLazyWrite();
			auto min = [](i16 a, i16 b) { return a < b ? a : b; };
			auto max = [](i16 a, i16 b) { return a > b ? a : b; };
			auto IsPointInTriangle = [](I16Vec2 p, I16Vec2 a, I16Vec2 b, I16Vec2 c) -> bool {
				auto sign = [](I16Vec2 p1, I16Vec2 p2, I16Vec2 p3) -> i16 {
					return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
					};
				bool b1, b2, b3;
				b1 = sign(p, a, b) < 0.0f;
				b2 = sign(p, b, c) < 0.0f;
				b3 = sign(p, c, a) < 0.0f;
				return ((b1 == b2) && (b2 == b3));
				};
			auto edge = [&](I16Vec2 a, I16Vec2 b) {
				BresenHamLine<I16Vec2>(a, b, [this, &col](I16Vec2 pos) {
					UnsafeSet(pos, col);
					});
				};
			auto fill = [&](I16Vec2 a, I16Vec2 b, I16Vec2 c) {
				i16 minX = min(a.x, min(b.x, c.x));
				i16 minY = min(a.y, min(b.y, c.y));
				i16 maxX = max(a.x, max(b.x, c.x));
				i16 maxY = max(a.y, max(b.y, c.y));
				for (i16 x = minX; x <= maxX; x++)
					for (i16 y = minY; y <= maxY; y++)
					{
						I16Vec2 p{ x, y };
						if (p.x < 0 || p.y < 0 || p.x >= mSize.x || p.y >= mSize.y)
							continue;
						if (IsPointInTriangle(p, a, b, c))
							UnsafeSet(p, col);
					}
				};
			edge(p1, p2);
			edge(p2, p3);
			edge(p3, p1);
			fill(p1, p2, p3);
			return *this;

		}
		TextureLayer& DrawPolygonOutline(const std::vector<I16Vec2>& points, Color col, i16 thickness = 1)
		{
			handleLazyWrite();
			for (i16 i = 0; i < points.size(); i++)
				DrawLine(points[i], points[(i + 1) % points.size()], col, thickness);
			return *this;

		}
		TextureLayer& DrawPolygonOutlineUnsafe(const std::vector<I16Vec2>& points, Color col, i16 thickness = 1)
		{
			handleLazyWrite();
			for (i16 i = 0; i < points.size(); i++)
				DrawLineUnsafe(points[i], points[(i + 1) % points.size()], col, thickness);
			return *this;
		}
		// TODO: Add bezier curve drawing
		// TODO: Add polygon filling

		TextureLayer& Merge(const TextureLayer& other, float alpha = 0.5f);
		// TODO: Add handling for different sized layers

		Color GetAverageColor() const;


		I16Vec2 GetSize() const { return mSize; }
		i16 GetWidth() const { return mSize.x; }
		i16 GetHeight() const { return mSize.y; }
		u8 GetBitDepth() const { return mBitDepth; }
		u8 GetColorType() const { return mColorType; }
		u8 GetInterlacing() const { return mInterlacing; }
		u8 GetFilterType() const { return mFilterType; }
		u8 GetCompressionType() const { return mCompressionType; }

		TextureLayer& Resize(I16Vec2 newSize);
		TextureLayer& Resize(i16 newWidth, i16 newHeight) { Resize(I16Vec2{ newWidth, newHeight });  return *this;}

		TextureLayer& SetColorType(u8 colorType) { mColorType = colorType;  return *this; }
		TextureLayer& SetBitDepth(u8 bitDepth) { mBitDepth = bitDepth;  return *this; }
		TextureLayer& SetInterlacing(u8 interlacing) { mInterlacing = interlacing;  return *this; }
		TextureLayer& SetFilterType(u8 filterType) { mFilterType = filterType;  return *this; }
		TextureLayer& SetCompressionType(u8 compressionType) { mCompressionType = compressionType;  return *this; }
		TextureLayer& SafeSet(I16Vec2 pos, Color color)
		{
			BoundingBox<I16Vec2> b = BoundingBox<I16Vec2>{ {0, 0}, mSize - 1 };
			if (!b.Contains(pos)) [[unlikely]]
				return *this;

			auto& col = At(pos);
			col = color;
			return *this;
		}
		TextureLayer& UnsafeSet(I16Vec2 pos, Color color) { At(pos) = color; return *this; }
		
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
			for (i16 y = 0; y < mSize.y; y++)
			{
				const T* pixelRow = reinterpret_cast<const T*>(pixelData[y].get());
				for (i16 x = 0; x < mSize.x; x++)
				{
					auto* colorDataOfRow = pixelRow + x * 4; 
					// Assuming RGBA format which can be upheld due to libpng conversion
					Color& color = mData[x + y * mSize.x];
					color.x = static_cast<float>(colorDataOfRow[0]) / std::numeric_limits<T>::max();
					color.y = static_cast<float>(colorDataOfRow[1]) / std::numeric_limits<T>::max();
					color.z = static_cast<float>(colorDataOfRow[2]) / std::numeric_limits<T>::max();
					color.w = static_cast<float>(colorDataOfRow[3]) / std::numeric_limits<T>::max();
				}
			}
		}

		template<std::integral T>
		std::vector<std::unique_ptr<u8[]>> writeNBit()
		{
			auto BaseWrite = [&]<size_t colorCount>(){
				std::vector<std::unique_ptr<T[]>> pixelData;
				pixelData.resize(mSize.y);
				for (i16 y = 0; y < mSize.y; y++)
				{
					void* data = new T[mSize.x * colorCount];
#ifndef NDEBUG
					memset(data, 0xAB, mSize.x * colorCount);
#endif
					pixelData[y] = std::unique_ptr<T[]>((T*)data);
					for (i16 x = 0; x < mSize.x; x++)
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
		bool mIsLazyLoaded{ false };
		std::string mPath{ "" };
		u8 mColorType{};
		u8 mBitDepth{};
		u8 mInterlacing : 1 {};
		u8 mFilterType : 1 {};
		u8 mCompressionType : 1 {};
		I16Vec2 mSize{ 0, 0 };
		std::vector<Color> mData{};


	};

	class Texture
	{
	private:
		constexpr static u8 mFilterType = PNG_FILTER_TYPE_BASE;
		constexpr static u8 mCompressionType = PNG_COMPRESSION_TYPE_DEFAULT;
		constexpr static u8 mInterlacing = PNG_INTERLACE_NONE;
		struct TextureInternalLayer
		{
			TextureInternalLayer() = default;
			TextureInternalLayer(TextureLayer&& layer, float alpha = 1.f) : TL(std::move(layer)), alpha(alpha) {}
			TextureInternalLayer(const TextureLayer& layer, float alpha = 1.f) : TL(layer), alpha(alpha) {}
			TextureLayer TL;
			float alpha = 1.f;


		};
		std::vector<TextureInternalLayer> mLayers{};
		I16Vec2 mSize{};
		u8 mBitDepth{};
		u8 mColorType{};
		mutable u8 mSelectedLayer{0};
	public:
		Texture(
			I16Vec2 size,
			u8 bitDepth = 8,
			u8 colorType = PNG_COLOR_TYPE_RGBA,
			Color fillColor = { .5f, .5f, .5f, 1.f }
		) : mSize(size), mBitDepth(bitDepth), mColorType(colorType)
		{
			mLayers.reserve(4); // Reserve 4 layers for the user
			mLayers.push_back(TextureInternalLayer(TextureLayer(size, bitDepth, fillColor, colorType, mInterlacing, mFilterType, mCompressionType)));
		}
		Texture() = default; 
		Texture(const Texture& other) : mLayers(other.mLayers), mSize(other.mSize), mBitDepth(other.mBitDepth), mColorType(other.mColorType) {}
		Texture(TextureLayer&& layer) : mLayers(1, TextureInternalLayer(std::move(layer)))
		{
			mSize = mLayers[0].TL.GetSize();
			mBitDepth = mLayers[0].TL.GetBitDepth();
			mColorType = mLayers[0].TL.GetColorType();
		}
		Texture(const std::string& path, bool lazyLoad = true) : mLayers(1, TextureLayer(path, lazyLoad))
		{
			mSize = mLayers[0].TL.GetSize();
			mBitDepth = mLayers[0].TL.GetBitDepth();
			mColorType = mLayers[0].TL.GetColorType();
		}
		Texture& operator=(const Texture& other) {
			mLayers = other.mLayers; mSize = other.mSize; mBitDepth = other.mBitDepth; mColorType = other.mColorType;
			return *this;
		}
		Texture& operator=(TextureLayer&& layer) {
			mLayers = { TextureInternalLayer(std::move(layer)) };
			mSize = mLayers[0].TL.GetSize();
			mBitDepth = mLayers[0].TL.GetBitDepth();
			mColorType = mLayers[0].TL.GetColorType();
			return *this;
		}
		
		TextureLayer& operator[](i16 index) { return mLayers[index].TL; }
		const TextureLayer& operator[](i16 index) const { return mLayers[index].TL; }

		TextureLayer Flatten() const;

		Texture&& Rotate90() { 
			for (auto& layer : mLayers) 
				layer.TL.Roate90();
			return std::move(*this); 
		}
		void FinalizeAndWrite(const std::filesystem::path& path) const;
		void AddLayer(const TextureLayer& layer) { mLayers.push_back(TextureInternalLayer(layer)); }
		void AddLayer(TextureLayer&& layer) { mLayers.push_back(std::move(layer)); }
		void AddLayer() { mLayers.push_back(TextureLayer(mSize, mBitDepth, { 0.f, 0.f, 0.f, 0.f }, mColorType, mInterlacing, mFilterType, mCompressionType)); }
		void RemoveLayer(i16 index) { mLayers.erase(mLayers.begin() + index); }
		void SetLayer(i16 index) { mSelectedLayer = index; }
		TextureLayer& GetLayer(i16 index) { return mLayers[index].TL; }
		TextureLayer& GetActiveLayer() { return mLayers[mSelectedLayer].TL; }
		const TextureLayer& GetLayer(i16 index) const { return mLayers[index].TL; }
		const TextureLayer& GetActiveLayer() const { return mLayers[mSelectedLayer].TL; }
		i16 GetLayerCount() const { return mLayers.size(); }
		I16Vec2 GetSize() const { return mSize; }
		i16 GetWidth() const { return mSize.x; }
		i16 GetHeight() const { return mSize.y; }
		u8 GetBitDepth() const { return mBitDepth; }
		u8 GetColorType() const { return mColorType; }
		void DisableLayer(i16 index) { mLayers[index].alpha = 0.f; }
		void EnableLayer(i16 index) { mLayers[index].alpha = 1.f; }
		void SetLayerAlpha(i16 index, float alpha) { mLayers[index].alpha = alpha; }
		void SetSelectedLayerAlpha(float alpha) { mLayers[mSelectedLayer].alpha = alpha; }
		float GetLayerAlpha(i16 index) const { return mLayers[index].alpha; }
		float GetSelectedLayerAlpha() const { return mLayers[mSelectedLayer].alpha; }
		void SelectLayer(i16 index) { mSelectedLayer = index; }
		auto& GetSelectedLayer() { return mLayers[mSelectedLayer]; }
		const auto& GetSelectedLayer() const { return mLayers[mSelectedLayer]; }
		constexpr static u8 GetFilterType() { return mFilterType; }
		constexpr static u8 GetCompressionType() { return mCompressionType; }
		constexpr static u8 GetInterlacing() { return mInterlacing; }
	};

}