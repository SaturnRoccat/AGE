#pragma once
#include <memory>
#include <stdexcept>

namespace AgeAPI
{
	using u8 = unsigned char;
	using i8 = char;
	using u16 = unsigned short;
	using i16 = short;
	using u32 = unsigned int;
	using i32 = int;
	using u64 = unsigned long long;
	using i64 = long long;
	using f32 = float;
	using f64 = double;

    

	template<typename T>
	class NonOwningPtr
	{
	public:
		NonOwningPtr(T* ptr) : mPtr(ptr) {}
		NonOwningPtr(const NonOwningPtr<T>& other) : mPtr(other.mPtr) {}
		NonOwningPtr(NonOwningPtr<T>&& other) noexcept : mPtr(std::move(other.mPtr)) {}
		NonOwningPtr() = default;
		T* operator->()
		{
#ifndef NDEBUG
			if (!mPtr)
				throw std::runtime_error("NonOwningPtr Destroyed While Still Being Required");
#endif
			return mPtr;
		}
		T& operator*()
		{
#ifndef NDEBUG
			if (!mPtr)
				throw std::runtime_error("NonOwningPtr Destroyed While Still Being Required");
#endif
			return *mPtr;
		}
		T* Get()
		{
#ifndef NDEBUG
			if (!mPtr)
				throw std::runtime_error("NonOwningPtr Destroyed While Still Being Required");
#endif

			return mPtr;
		}
		const T* Get() const
		{
#ifndef NDEBUG
			if (!mPtr)
				throw std::runtime_error("NonOwningPtr Destroyed While Still Being Required");
#endif
			return mPtr;
		}
		operator T* ()
		{
#ifndef NDEBUG
			if (!mPtr)
				throw std::runtime_error("NonOwningPtr Destroyed While Still Being Required");
#endif
			return mPtr;
		}

		void operator delete(void* ptr) = delete;
	private:
		T* mPtr{ nullptr };
	};

	template <typename T, typename U>
	std::unique_ptr<T> StaticUniquePointerCast(std::unique_ptr<U>&& p)
	{
		return std::unique_ptr<T>{static_cast<T*>(p.release())};
	}

	template <typename T, typename U>
	std::unique_ptr<T> PollymorphicUniquePointerCast(std::unique_ptr<U>&& p)
	{

	}
}