#pragma once
#include <functional>
#include <AgeAPI/Internal/Types.hpp>

namespace AgeAPI
{

	template<IntegralVec PositionType, typename ...Args>
	void BresenHamLine(PositionType begin, PositionType end, std::function<void(const PositionType&)> callback, Args&&... args)
	{
		using ValT = decltype(begin.x);
		auto dx = abs(end.x - begin.x);
		auto dy = abs(end.y - begin.y);

		ValT sx = begin.x < end.x ? 1 : -1;
		ValT sy = begin.y < end.y ? 1 : -1;

		ValT err = dx - dy;
		ValT e2;
		PositionType current = begin;
		while (current != end)
		{
			callback(current, std::forward<Args>(args)...);
			e2 = 2 * err;
			if (e2 > -dy)
			{
				err -= dy;
				current.x += sx;
			}
			if (e2 < dx)
			{
				err += dx;
				current.y += sy;
			}
		}
	}
}