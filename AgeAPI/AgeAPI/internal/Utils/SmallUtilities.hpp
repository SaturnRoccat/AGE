#pragma once
#include <type_traits>

#ifndef TO_UNDERLYING
#define TO_UNDERLYING(x) static_cast<std::underlying_type_t<decltype(x)>>(x)
#endif

namespace AgeAPI
{
	template<typename T>
	T* NoConst (const T* ptr) { return const_cast<T*>(ptr); }

	template<class... Ts>
	struct overloaded : Ts... { using Ts::operator()...; };
	template<class... Ts>
	overloaded(Ts...) -> overloaded<Ts...>;
}