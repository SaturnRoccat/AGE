#include <AgeSharedUtils.hpp>
#include <print>
#include <array>

template<std::size_t N>
std::array<char, N> ExplodeString(const char* str)
{
	std::array<char, N> arr{};
	for (std::size_t i = 0; i < N; i++)
		arr[i] = str[i];
	return arr;
}
#define Str(x) x "\0"
#define ExpString(x) ExplodeString<sizeof(Str(x))>(Str(x))

int main(int argc, char** argv)
{


	return 0;
}