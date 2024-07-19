#pragma once
#include <memory>
#include <stdexcept>
#include <vector>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>

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


	/*
	* std::vector optimized for small sizes
	*/
	template<typename T, size_t Size = 1>
	class SmallVector
	{
	public:
		SmallVector() {} // Have to do this because of the union :<
		SmallVector(std::initializer_list<T> list)
		{
			if (list.size() > Size)
				mBigData = std::vector<T>(list);
			else
			{
				std::copy(list.begin(), list.end(), mSmallData.begin());
				mIndex = list.size();
			}
		}
		SmallVector(const std::vector<T>& other)
		{
			if (other.size() > Size)
				mBigData = other;
			else
			{
				std::copy(other.begin(), other.end(), mSmallData.begin());
				mIndex = other.size();
			}
		}
		SmallVector(std::vector<T>&& other)
		{
			if (other.size() > Size)
				mBigData = std::move(other);
			else
			{
				std::memmove(mSmallData.begin(), other.begin(), sizeof(T) * other.size());
				mIndex = other.size();
			}
		}
		SmallVector(const SmallVector<T, Size>& other)
		{
			if (other.size() > Size)
				mBigData = other.mBigData;
			else
			{
				std::copy(other.mSmallData.begin(), other.mSmallData.begin() + other.mIndex, mSmallData.begin());
				mIndex = other.mIndex;
			}

		}
		SmallVector(SmallVector<T, Size>&& other) noexcept
		{
			if (other.size() > Size)
				mBigData = std::move(other.mBigData);
			else
			{
				std::memmove(mSmallData.data(), other.mSmallData.data(), sizeof(T) * other.size());
				mIndex = other.mIndex;
			}
		}
		SmallVector<T, Size>& operator=(const SmallVector<T, Size>& other)
		{
			if (other.size() > Size)
				mBigData = other.mBigData;
			else
			{
				std::copy(other.mSmallData.begin(), other.mSmallData.end(), mSmallData.begin());
				mIndex = other.mIndex;
			}
			return *this;
		}
		SmallVector<T, Size>& operator=(SmallVector<T, Size>&& other) noexcept
		{
			if (other.size() > Size)
				mBigData = std::move(other.mBigData);
			else
			{
				std::memmove(mSmallData.begin(), other.mSmallData.begin(), sizeof(T) * other.size());
				mIndex = other.mIndex;
			}
			return *this;
		}
		SmallVector<T, Size>& operator=(const std::vector<T>& other)
		{
			if (other.size() > Size)
				mBigData = other;
			else
			{
				std::copy(other.begin(), other.end(), mSmallData.begin());
				mIndex = other.size();
			}
			return *this;
		}
		SmallVector<T, Size>& operator=(std::vector<T>&& other)
		{
			if (other.size() > Size)
				mBigData = std::move(other);
			else
			{
				std::memmove(mSmallData.begin(), other.begin(), sizeof(T) * other.size());
				mIndex = other.size();
			}
			return *this;
		}
		~SmallVector()
		{
			if (!mIsSmall)
				mBigData.~vector(); // Have to explicitly call the destructor due to the union
			mIndex = 0;

		}
		size_t size() const
		{
			return mIsSmall ? mIndex : mBigData.size();
		}
		T& operator[](i64 index)
		{
			return readData(index);
		}
		const T& operator[](i64 index) const
		{
			return readData(index);
		}
		void push_back(const T& data)
		{
			checkAndMoveData(size());
			writeDataBack(data);
			if (mIsSmall)
				++mIndex;
		}
		void push_back(T&& data)
		{
			checkAndMoveData(size());
			writeDataBack(std::move(data));
			if (mIsSmall)
				++mIndex;
		}
		void pop_back()
		{
			if (mIsSmall)
				--mIndex;
			else
				mBigData.pop_back();
		}
		void clear()
		{
			if (mIsSmall)
				mIndex = 0;
			else
				mBigData.clear();
		}
		bool empty() const
		{
			return mIsSmall ? mIndex == 0 : mBigData.empty();
		}
		void resize(size_t size)
		{
			if (willStillBeSmall(size))
			{
				if (!mIsSmall)
				{
					for (size_t i = 0; i < size; ++i)
						mSmallData[i] = mBigData[i];
				}
				mIndex = size;
			}
			else
			{
				mBigData.resize(size);
				checkAndMoveData(size);
				mIsSmall = false;
			}
		}
		std::vector<T> ToVector() const
		{
			if (mIsSmall)
				return std::vector<T>(mSmallData.begin(), mSmallData.begin() + mIndex);
			else
				return mBigData;
		}

		// Have to use an iterator class since we have different types of data that we need to iterate over
		class Iterator
		{
		public:
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			Iterator(pointer ptr, i64 index) : mPtr(ptr), mIndex(index) {}
			Iterator(const Iterator& other) : mPtr(other.mPtr), mIndex(other.mIndex) {}
			Iterator(Iterator&& other) noexcept : mPtr(std::move(other.mPtr)), mIndex(other.mIndex) {}
			Iterator& operator=(const Iterator& other)
			{
				mPtr = other.mPtr;
				mIndex = other.mIndex;
				return *this;
			}
			Iterator& operator=(Iterator&& other) noexcept
			{
				mPtr = std::move(other.mPtr);
				mIndex = other.mIndex;
				return *this;
			}
			reference operator*()
			{
				return *mPtr;
			}
			pointer operator->()
			{
				return mPtr;
			}
			Iterator& operator++()
			{
				++mIndex;
				++mPtr;
				return *this;
			}
			Iterator operator++(int)
			{
				Iterator temp = *this;
				++mIndex;
				++mPtr;
				return temp;
			}
			Iterator& operator--()
			{
				--mIndex;
				--mPtr;
				return *this;
			}
			Iterator operator--(int)
			{
				Iterator temp = *this;
				--mIndex;
				--mPtr;
				return temp;
			}
		private:
			pointer mPtr{ nullptr };
			i64 mIndex;

		};
		bool isSmall() const
		{
			return mIsSmall;
		}

		T* begin()
		{
			return mIsSmall ? mSmallData.data() : mBigData.data();
		}
		T* end()
		{
			return mIsSmall ? mSmallData.data() + mIndex : mBigData.data() + mBigData.size();
		}
		const T* begin() const
		{
			return mIsSmall ? mSmallData.data() : mBigData.data();
		}
		const T* end() const
		{
			return mIsSmall ? mSmallData.data() + mIndex : mBigData.data() + mBigData.size();
		}



	private:
		void writeDataBack(const T& data)
		{
			if (mIsSmall)
				mSmallData[mIndex++] = data;
			else
				mBigData.push_back(data);
		}
		void writeDataBack(T&& data)
		{
			if (mIsSmall)
				mSmallData[mIndex++] = std::move(data);
			else
				mBigData.push_back(std::move(data));
		}
		void checkAndMoveData(i64 index)
		{
			if (index >= Size && mIsSmall)
			{
				mBigData = std::vector<T>(mSmallData.begin(), mSmallData.end());
				mIsSmall = false;
			}

		}
		const T& readData(i64 index) const
		{
			return mIsSmall ? mSmallData[index] : mBigData[index];
		}
		T& readData(i64 index)
		{
			return mIsSmall ? mSmallData[index] : mBigData[index];
		}
		bool willStillBeSmall(size_t newSize) const
		{
			return newSize <= Size;
		}
		size_t fetchNextIndex() const
		{
			return mIsSmall ? mIndex : mBigData.size();
		}
	private:
		union
		{
			struct
			{
				std::array<T, Size> mSmallData;
				int mIndex;
			};
			std::vector<T> mBigData;
		};
		bool mIsSmall{ true };
	};
}