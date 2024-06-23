#pragma once
#include <functional>
#include <AgeAPI/Internal/Types.hpp>

namespace AgeAPI
{
	template<IntegralVec2 PositionType, typename ...Args>
	void DDA(const PositionType& begin, const PositionType& end, std::function<void(const PositionType&)> callback, Args&&... args)
	{
		using PositionTypeStore = decltype(begin.x);
		PositionTypeStore dx = end.x - begin.x;
		PositionTypeStore dy = end.y - begin.y;
		PositionTypeStore steps = std::abs(dx) > std::abs(dy) ? std::abs(dx) : std::abs(dy);
		float xIncrement = dx / (float)steps;
		float yIncrement = dy / (float)steps;
		float x = begin.x;
		float y = begin.y;
		for (PositionTypeStore i = 0; i < steps; i++)
		{
			callback({ (PositionTypeStore)x, (PositionTypeStore)y }, std::forward<Args>(args)...);
			x += xIncrement;
			y += yIncrement;
		}
	}


}