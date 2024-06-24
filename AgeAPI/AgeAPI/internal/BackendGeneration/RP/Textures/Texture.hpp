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
		TextureLayer(const TextureLayer& bottom, const TextureLayer& Top, float alpha);

		TextureLayer() = default;

		void Write(const std::string& path);

		Color& operator[](IVec2 pos) { handleLazyWrite(); return mData[pos.x + pos.y * mSize.x]; }
		Color& operator[](i32 index) { handleLazyWrite(); return mData[index]; }

		const Color& operator[](IVec2 pos) const { return mData[pos.x + pos.y * mSize.x]; }
		const Color& operator[](i32 index) const { return mData[index]; }

		Color& At(IVec2 pos) { handleLazyWrite(); return mData[pos.x + pos.y * mSize.x]; }
		Color& At(i32 index) { handleLazyWrite(); return mData[index]; }

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
		void DrawTriangleOutline(IVec2 p1, IVec2 p2, IVec2 p3, Color col, i32 thickness = 1)
		{
			handleLazyWrite();
			DrawLine(p1, p2, col, thickness);
			DrawLine(p2, p3, col, thickness);
			DrawLine(p3, p1, col, thickness);
		}
		void DrawTriangleOutlineUnsafe(IVec2 p1, IVec2 p2, IVec2 p3, Color col, i32 thickness = 1)
		{
			handleLazyWrite();
			DrawLineUnsafe(p1, p2, col, thickness);
			DrawLineUnsafe(p2, p3, col, thickness);
			DrawLineUnsafe(p3, p1, col, thickness);
		}
		void DrawTriangle(IVec2 p1, IVec2 p2, IVec2 p3, Color col)
		{
			handleLazyWrite();
			auto min = [](i32 a, i32 b) { return a < b ? a : b; };
			auto max = [](i32 a, i32 b) { return a > b ? a : b; };
			auto IsPointInTriangle = [](IVec2 p, IVec2 a, IVec2 b, IVec2 c) -> bool {
				auto sign = [](IVec2 p1, IVec2 p2, IVec2 p3) -> i32 {
					return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
				};
				bool b1, b2, b3;
				b1 = sign(p, a, b) < 0.0f;
				b2 = sign(p, b, c) < 0.0f;
				b3 = sign(p, c, a) < 0.0f;
				return ((b1 == b2) && (b2 == b3));
			};
			auto edge = [&](IVec2 a, IVec2 b) {
				BresenHamLine<IVec2>(a, b, [this, &col](IVec2 pos) {
										SafeSet(pos, col);
														});
			};
			auto fill = [&](IVec2 a, IVec2 b, IVec2 c) {
				i32 minX = min(a.x, min(b.x, c.x));
				i32 minY = min(a.y, min(b.y, c.y));
				i32 maxX = max(a.x, max(b.x, c.x));
				i32 maxY = max(a.y, max(b.y, c.y));
				for (i32 x = minX; x <= maxX; x++)
					for (i32 y = minY; y <= maxY; y++)
					{
						IVec2 p{ x, y };
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
		}
		void DrawTriangleUnsafe(IVec2 p1, IVec2 p2, IVec2 p3, Color col)
		{
			handleLazyWrite();
			auto min = [](i32 a, i32 b) { return a < b ? a : b; };
			auto max = [](i32 a, i32 b) { return a > b ? a : b; };
			auto IsPointInTriangle = [](IVec2 p, IVec2 a, IVec2 b, IVec2 c) -> bool {
				auto sign = [](IVec2 p1, IVec2 p2, IVec2 p3) -> i32 {
					return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
					};
				bool b1, b2, b3;
				b1 = sign(p, a, b) < 0.0f;
				b2 = sign(p, b, c) < 0.0f;
				b3 = sign(p, c, a) < 0.0f;
				return ((b1 == b2) && (b2 == b3));
				};
			auto edge = [&](IVec2 a, IVec2 b) {
				BresenHamLine<IVec2>(a, b, [this, &col](IVec2 pos) {
					UnsafeSet(pos, col);
					});
				};
			auto fill = [&](IVec2 a, IVec2 b, IVec2 c) {
				i32 minX = min(a.x, min(b.x, c.x));
				i32 minY = min(a.y, min(b.y, c.y));
				i32 maxX = max(a.x, max(b.x, c.x));
				i32 maxY = max(a.y, max(b.y, c.y));
				for (i32 x = minX; x <= maxX; x++)
					for (i32 y = minY; y <= maxY; y++)
					{
						IVec2 p{ x, y };
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
		}
		void DrawPolygonOutline(const std::vector<IVec2>& points, Color col, i32 thickness = 1)
		{
			handleLazyWrite();
			for (i32 i = 0; i < points.size(); i++)
				DrawLine(points[i], points[(i + 1) % points.size()], col, thickness);
		}
		void DrawPolygonOutlineUnsafe(const std::vector<IVec2>& points, Color col, i32 thickness = 1)
		{
			handleLazyWrite();
			for (i32 i = 0; i < points.size(); i++)
				DrawLineUnsafe(points[i], points[(i + 1) % points.size()], col, thickness);
		}
		// TODO: Add bezier curve drawing
		// TODO: Add polygon filling

		void Merge(const TextureLayer& other, float alpha = 0.5f);
		// TODO: Add handling for different sized layers


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
		struct TextureInternalLayer
		{
			TextureInternalLayer() = default;
			TextureInternalLayer(TextureLayer&& layer, float alpha = 1.f) : TL(std::move(layer)), alpha(alpha) {}
			TextureInternalLayer(const TextureLayer& layer, float alpha = 1.f) : TL(layer), alpha(alpha) {}
			TextureLayer TL;
			float alpha = 1.f;


		};
		std::vector<TextureInternalLayer> mLayers{4};
		IVec2 mSize{};
		u8 mBitDepth{};
		u8 mColorType{};
		u8 mSelectedLayer{0};
	private:
	public:
		Texture(
			IVec2 size,
			u8 bitDepth = 8,
			u8 colorType = PNG_COLOR_TYPE_RGBA,
			Color fillColor = { 0.f, 0.f, 0.f, 0.f }
		) : mSize(size), mBitDepth(bitDepth), mColorType(colorType)
		{
			mLayers.push_back(TextureInternalLayer(TextureLayer(size, bitDepth, fillColor, colorType, mInterlacing, mFilterType, mCompressionType)));
		}
		Texture() = default;
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
		TextureLayer& operator[](i32 index) { return mLayers[index].TL; }
		const TextureLayer& operator[](i32 index) const { return mLayers[index].TL; }

		TextureLayer Flatten();

		void FinalizeAndWrite(const std::string& path);

		void AddLayer(const TextureLayer& layer) { mLayers.push_back(TextureInternalLayer(layer)); }
		void AddLayer(TextureLayer&& layer) { mLayers.push_back(std::move(layer)); }
		void AddLayer() { mLayers.push_back(TextureLayer(mSize, mBitDepth, { 0.f, 0.f, 0.f, 0.f }, mColorType, mInterlacing, mFilterType, mCompressionType)); }
		void RemoveLayer(i32 index) { mLayers.erase(mLayers.begin() + index); }
		void SetLayer(i32 index) { mSelectedLayer = index; }
		TextureLayer& GetLayer(i32 index) { return mLayers[index].TL; }
		TextureLayer& GetActiveLayer() { return mLayers[mSelectedLayer].TL; }
		const TextureLayer& GetLayer(i32 index) const { return mLayers[index].TL; }
		const TextureLayer& GetActiveLayer() const { return mLayers[mSelectedLayer].TL; }
		i32 GetLayerCount() const { return mLayers.size(); }
		IVec2 GetSize() const { return mSize; }
		i32 GetWidth() const { return mSize.x; }
		i32 GetHeight() const { return mSize.y; }
		u8 GetBitDepth() const { return mBitDepth; }
		u8 GetColorType() const { return mColorType; }
		void DisableLayer(i32 index) { mLayers[index].alpha = 0.f; }
		void EnableLayer(i32 index) { mLayers[index].alpha = 1.f; }
		void SetLayerAlpha(i32 index, float alpha) { mLayers[index].alpha = alpha; }
		void SetSelectedLayerAlpha(float alpha) { mLayers[mSelectedLayer].alpha = alpha; }
		float GetLayerAlpha(i32 index) const { return mLayers[index].alpha; }
		float GetSelectedLayerAlpha() const { return mLayers[mSelectedLayer].alpha; }
		void SelectLayer(i32 index) { mSelectedLayer = index; }
		auto& GetSelectedLayer() { return mLayers[mSelectedLayer]; }
		constexpr static u8 GetFilterType() { return mFilterType; }
		constexpr static u8 GetCompressionType() { return mCompressionType; }
		constexpr static u8 GetInterlacing() { return mInterlacing; }
	};

}