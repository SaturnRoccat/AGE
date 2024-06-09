#include "String.hpp"
#include <immintrin.h>
namespace AgeUtils
{
    bool StringCompare(std::string_view str1, std::string_view str2)
    {
        if (str1.size() != str2.size())
			return false;

		if (str1.size() == 0)
			return true;

		if (str1.size() < 32)
		{
			return str1 == str2;
		}

		const char* p1 = str1.data();
		const char* p2 = str2.data();
		size_t size = str1.size();

		size_t i = 0;
		for (; i < size - 32; i += 32)
		{
			auto v1 =  _mm256_loadu_si256(reinterpret_cast<const  __m256i*>(p1 + i));
			auto v2 =  _mm256_loadu_si256(reinterpret_cast<const  __m256i*>(p2 + i));
			__m256i cmp =  _mm256_cmpeq_epi8(v1, v2);
			int mask =  _mm256_movemask_epi8(cmp);
			if (mask != 0xFFFFFFFF)
				return false;
		}

		for (; i < size; ++i)
		{
			if (p1[i] != p2[i])
				return false;
		}

		return true;

    }
}
