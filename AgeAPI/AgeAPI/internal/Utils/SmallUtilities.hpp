#pragma once
#include <type_traits>




namespace AgeAPI
{
	template<typename T>
	T* NoConst (const T* ptr) { return const_cast<T*>(ptr); }
	template<typename T> requires std::is_enum_v<T>
	constexpr auto ToUnderlying(T e) noexcept requires std::is_enum_v<T>
	{
		return static_cast<std::underlying_type_t<T>>(e);
	}
	template<class... Ts>
	struct overloaded : Ts... { using Ts::operator()...; };
	template<class... Ts>
	overloaded(Ts...) -> overloaded<Ts...>;
}

template<typename T> requires std::is_enum_v<T>
constexpr auto operator|(T lhs, T rhs) noexcept requires std::is_enum_v<T>
{
	using U = std::underlying_type_t<T>;
	return static_cast<T>(static_cast<U>(lhs) | static_cast<U>(rhs));
}



#ifndef TO_UNDERLYING
#define TO_UNDERLYING(x) AgeAPI::ToUnderlying(x)
#endif