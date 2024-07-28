#pragma once
#include <memory>
#include <stdexcept>
#include <vector>
#include <array>
#include <variant>
#include <bit>
#include <new>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>

namespace AgeAPI
{
	// Enable & for any enum
	template<typename T, typename U>
	constexpr std::enable_if_t<std::is_enum_v<T> && std::is_enum_v<U>, T> operator&(T lhs, U rhs)
	{
		using UT = std::underlying_type_t<T>;
		using UU = std::underlying_type_t<U>;
		return static_cast<T>(static_cast<UT>(lhs) & static_cast<UU>(rhs));
	}

	template<typename Enum, typename... Ts>
	constexpr Enum MixEnum(Ts... ts)
	{
		uint64_t result = 0;
		(void(result = result | (uint64_t)ts), ...);
		return static_cast<Enum>(result);
	}


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
		NonOwningPtr<T>& operator=(const NonOwningPtr<T>& other)
		{
			mPtr = other.mPtr;
			return *this;
		}
		NonOwningPtr<T>& operator=(NonOwningPtr<T>&& other) noexcept
		{
			mPtr = std::move(other.mPtr);
			return *this;
		}
		T* operator->()
		{
			return mPtr;
		}
		T& operator*()
		{
			return *mPtr;
		}
		T* Get()
		{

			return mPtr;
		}
		const T* Get() const
		{
			return mPtr;
		}
		operator T* ()
		{
			return mPtr;
		}
		NonOwningPtr<T> operator=(T* ptr)
		{
			mPtr = ptr;
			return *this;
		}
		bool operator==(const NonOwningPtr<T>& other) const
		{
			return mPtr == other.mPtr;
		}
		bool operator!=(const NonOwningPtr<T>& other) const
		{
			return mPtr != other.mPtr;
		}
		bool operator==(T* ptr) const
		{
			return mPtr == ptr;
		}
		bool operator!=(T* ptr) const
		{
			return mPtr != ptr;
		}
		bool operator==(std::nullptr_t) const
		{
			return mPtr == nullptr;
		}
		bool operator!=(std::nullptr_t) const
		{
			return mPtr != nullptr;
		}
		bool operator!() const
		{
			return mPtr == nullptr;
		}
		operator bool() const
		{
			return mPtr != nullptr;
		}
		operator T& ()
		{
			return *mPtr;
		}
		operator T& () const
		{
			return *mPtr;
		}



		void operator delete(void* ptr) = delete;
	private:
		T* mPtr{ nullptr };
	};

	template<typename T> requires std::is_copy_constructible_v<T>
	std::shared_ptr<T> ConstRefToShared(const T& data)
	{
		return std::make_shared<T>(data);
	}

	template<typename T> requires std::is_move_assignable_v<T>
	std::shared_ptr<T> MoveToShared(T&& data)
	{
		return std::make_shared<T>(std::move(data));
	}
	template<typename T> requires std::is_move_assignable_v<T>
	std::unique_ptr<T> MoveToUnique(T&& data)
	{
		return std::make_unique<T>(std::move(data));
	}

	template <typename T, typename U>
	std::unique_ptr<T> StaticUniquePointerCast(std::unique_ptr<U>&& p)
	{
		return std::unique_ptr<T>{static_cast<T*>(p.release())};
	}
	template <typename T, typename U>
	std::unique_ptr<T> StaticUniquePointerCast(std::unique_ptr<U> p)
	{
		return std::unique_ptr<T>{static_cast<T*>(p.release())};
	}
	template <typename T, typename U> requires std::is_base_of_v<T, U>
	std::unique_ptr<T> ChildToParentUniquePointerCast(std::unique_ptr<U>&& p)
	{
		return std::unique_ptr<T>{static_cast<T*>(p.release())};
	}
	template <typename T, typename U> requires std::is_base_of_v<T, U>
	std::unique_ptr<T> ChildToParentUniquePointerCast(std::unique_ptr<U> p)
	{
		return std::unique_ptr<T>{static_cast<T*>(p.release())};
	}
	template <typename T>
	std::unique_ptr<T> NoCheckVoidUniqueCast(void* memory)
	{
		return std::unique_ptr<T>{static_cast<T*>(memory)};
	}

	template <typename T, typename U>
	std::unique_ptr<T> NCUC(U* p)
	{
		return std::unique_ptr<T>{static_cast<T*>(p)};
	}
	template <typename T, typename U>
	std::unique_ptr<T> NCUM(U&& data) requires std::is_move_assignable_v<U>
	{
		return std::unique_ptr<T>{static_cast<T*>(new U(std::move(data)))};
	}
	template<typename T, typename U, typename ...Args>
	std::unique_ptr<T> PollyCast(Args&&... args)
	{
		auto* ptr = new U(std::forward<Args>(args)...);
		return std::unique_ptr<T>{static_cast<T*>(ptr)};
	}
	template<typename T, typename U, typename ...Args>
	std::unique_ptr<T> PC(Args&&... args)
	{
		return PollyCast<T, U>(std::forward<Args>(args)...);
	}
	template<typename T, typename U> requires std::is_copy_constructible_v<U>
	std::unique_ptr<T> PC(const U& data)
	{
		auto* ptr = new U(data);
		return std::unique_ptr<T>{static_cast<T*>(ptr)};
	}
	template<typename T> requires std::is_move_assignable_v<T>
	std::unique_ptr<T> PC(T&& data)
	{
		auto* ptr = new T(std::move(data));
		return std::unique_ptr<T>{static_cast<T*>(ptr)};
	}

	class ScopedToggle
	{
	public:
		ScopedToggle(bool& toggle) : mToggle(toggle)
		{
			mToggle = !mToggle;
		}
		~ScopedToggle()
		{
			mToggle = !mToggle;
		}
	private:
		bool& mToggle;
	};
}