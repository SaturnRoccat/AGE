#pragma once
#include <AgeAPI/internal/Utils/Vec.hpp>
#include <AgeAPI/internal/Utils/JsonProxy.hpp>

namespace AgeAPI
{
    
	class Color : public Vec4T<float>
	{
	public:
		using Vec = Vec4T<float>;
	public:
		Color() : Vec4T<float>(0, 0, 0, 0) {}
		Color(float r, float g, float b, float a) : Vec4T<float>(r, g, b, a) {}
		Color(const Color& other) : Vec4T<float>(other) {}
		Color(const Vec& other) : Vec4T<float>(other) {}
		Color(Vec&& other) noexcept : Vec4T<float>(std::move(other)) {}

		Color GrayScale() const
		{
			float redSmoosh = x * 0.3f;
			float greenSmoosh = y * 0.59f;
			float blueSmoosh = z * 0.11f;
			float gray = redSmoosh + greenSmoosh + blueSmoosh;
			return Color(gray, gray, gray, w);
		}

		ErrorString WriteToJson(const AgeAPI::JsonProxy& proxy) const
		{
			if (proxy.mWriteLoc.IsArray())
			{
				auto arr = proxy.mWriteLoc.GetArray();
				arr.PushBack(x, proxy.mAllocator);
				arr.PushBack(y, proxy.mAllocator);
				arr.PushBack(z, proxy.mAllocator);
				arr.PushBack(w, proxy.mAllocator);
			}
			else if (proxy.mWriteLoc.IsObject())
			{
				proxy.mWriteLoc.AddMember("r", x, proxy.mAllocator);
				proxy.mWriteLoc.AddMember("g", y, proxy.mAllocator);
				proxy.mWriteLoc.AddMember("b", z, proxy.mAllocator);
				proxy.mWriteLoc.AddMember("a", w, proxy.mAllocator);
			}
			else
			{
				return "Invalid JSON Proxy";
			}
			return ErrorString();

		}

	};

}