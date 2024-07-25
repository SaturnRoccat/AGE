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

	/*
	* std::vector optimized for small sizes
	*/
	template<typename T, size_t Size = 1>
	class SmallVector
	{
	public:
		using value_type = T;
		using reference = T&;
		using const_reference = const T&;
		using pointer = T*;
		using const_pointer = const T*;
		using iterator = T*;
		using const_iterator = const T*;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using vec = std::vector<T>;
		using arr = std::array<std::byte, Size * sizeof(T)>;
	public:
		SmallVector() : mData{}, mIndex(0) {}
		SmallVector(std::initializer_list<T> list)
		{
			addContainerDataBack(list);
		}
		SmallVector(const std::array<T, Size>& arr)
		{
			addContainerDataBack(arr);
		}
		SmallVector(const std::vector<T>& vec)
		{
			addContainerDataBack(vec);
		}
		SmallVector(const SmallVector<T, Size>& other)
		{
			if (other.IsSmall())
				std::memcpy(getSmall().data(), other.getSmall().data(), Size * sizeof(T));
			else
				getVec() = other.getVec();
			mIndex = other.mIndex;
		}
		SmallVector(SmallVector<T, Size>&& other) noexcept
		{
			if (other.IsSmall())
				std::memmove(getSmall().data(), other.getSmall().data(), Size * sizeof(T));
			else
				mData = std::move(other.getVec());
			mIndex = other.mIndex;
		}
		SmallVector<T, Size>& operator=(const SmallVector<T, Size>& other)
		{
			if (other.IsSmall())
				std::memcpy(getSmall().data(), other.getSmall().data(), Size * sizeof(T));
			else
				mData = other.getVec();
			mIndex = other.mIndex;
			return *this;
		}
		SmallVector<T, Size>& operator=(SmallVector<T, Size>&& other) noexcept
		{
			if (other.IsSmall())
				std::memmove(getSmall().data(), other.getSmall().data(), Size * sizeof(T));
			else
				getVec() = std::move(other.getVec());
			mIndex = other.mIndex;
			return *this;
		}
		~SmallVector()
		{
			if (IsSmall())
			{
				for (size_t i = 0; i < mIndex; i++)
					fetchIndex(i)->~T();
			}
		}
		reference operator[](size_t index)
		{
			return *fetchIndex(index);
		}
		const_reference operator[](size_t index) const
		{
			return *fetchIndex(index);
		}
		reference front()
		{
			return getDataFront();
		}
		const_reference front() const
		{
			return getDataFront();
		}
		reference back()
		{
			return getDataBack();
		}
		const_reference back() const
		{
			return getDataBack();
		}
		iterator begin()
		{
			return fetchIndex(0);
		}
		const_iterator begin() const
		{
			return fetchIndex(0);
		}
		const_iterator cbegin() const
		{
			return fetchIndex(0);
		}
		iterator end()
		{
			return data() + mIndex;
		}
		const_iterator end() const
		{
			return data() + mIndex;
		}
		const_iterator cend() const
		{
			return getDataBack();
		}
		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}
		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(end());
		}
		const_reverse_iterator crbegin() const
		{
			return const_reverse_iterator(end());
		}
		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}
		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin());
		}
		const_reverse_iterator crend() const
		{
			return const_reverse_iterator(begin());
		}
		size_t size() const
		{
			return mIndex;
		}
		size_t capacity() const
		{
			if (IsSmall())
				return Size;
			else
				return getVec().capacity();
		}
		bool empty() const
		{
			return mIndex == 0;
		}
		void clear()
		{
			if (IsSmall())
			{
				for (size_t i = 0; i < mIndex; i++)
					fetchIndex(i)->~T();
			}
			else 
				getVec().clear();
			mIndex = 0;
		}
		void push_back(const T& data)
		{
			addDataBack(data);
		}
		void push_back(T&& data)
		{
			addDataBack(std::move(data));
		}
		void pop_back()
		{
			if (IsSmall())
				fetchIndex(mIndex - 1)->~T();
			mIndex--;
		}
		T* data()
		{
			if (IsSmall())
				return smallAsPointer();
			else
				return getVec().data();
		}
		const T* data() const
		{
			if (IsSmall())
				return smallAsPointer();
			else
				return getVec().data();
		}
		void reserve(size_t size)
		{
			if (size > Size)
			{
				if (IsSmall())
					fallOverToVector();
				getVec().reserve(size);
			}
		}
		void resize(size_t size)
		{
			if (size > Size)
			{
				if (IsSmall())
					fallOverToVector();
				getVec().resize(size);
			}
			else
				mIndex = size;
		}

		
		bool IsSmall() const
		{
			return std::holds_alternative<arr>(mData);
		}
	private:
		bool doesIndexFitInSmall(size_t index) const
		{
			return index < Size;
		}
		T* fetchIndex(size_t index)
		{
			if (IsSmall())
				return nullptr;
			else
				return getVec().data() + index;
		}
		const T* fetchIndex(size_t index) const
		{
			if (IsSmall())
				return nullptr;
			else
				return getVec().data() + index;
		}
		arr& getSmall()
		{
			return std::get<arr>(mData);
		}
		const arr& getSmall() const
		{
			return std::get<arr>(mData);
		}
		vec& getVec()
		{
			return std::get<vec>(mData);
		}
		const vec& getVec() const
		{
			return std::get<vec>(mData);
		}
		void addDataBack(const T& data)
		{
			if (IsSmall() && !doesIndexFitInSmall(mIndex))
				fallOverToVector();
			if (IsSmall())
				new (smallAsPointer() + mIndex) T(data);
			else
				getVec().push_back(data);
			mIndex++;
		}
		void addDataBack(T&& data)
		{
			if (!doesIndexFitInSmall(mIndex))
				fallOverToVector();
			if (IsSmall())
				new (smallAsPointer() + mIndex) T(std::move(data));
			else
				getVec().push_back(std::move(data));
			mIndex++;
		}
		T& getDataBack()
		{
			if (IsSmall())
				return *std::launder<T>(smallAsPointer() + mIndex - 1);
			else
				return getVec().back();
		}
		const T& getDataBack() const
		{
			if (IsSmall())
				return *std::launder<const T>(smallAsPointer() + mIndex - 1);
			else
				return getVec().back();
		}
		T& getDataFront()
		{
			if (IsSmall())
				return std::launder<T>(smallAsPointer());
			else
				return getVec().front();
		}
		const T& getDataFront() const
		{
			if (IsSmall())
				return *std::launder<const T>(smallAsPointer());
			else
				return getVec().front();
		}
		void fallOverToVector()
		{
			if (!IsSmall())
				return;
			vec newVec{};
			newVec.reserve(Size);
			for (size_t i = 0; i < mIndex; i++)
				newVec.push_back(std::move(smallAsPointer()[i]));
			mData = std::move(newVec);
		}

		template<typename Container>
		void addContainerDataBack(const Container& container)
		{
			reserve(container.size());
			for (const auto& element : container)
				addDataBack(element);
		}

		T* smallAsPointer()
		{
			return reinterpret_cast<T*>(getSmall().data());
		}

		const T* smallAsPointer() const
		{
			return reinterpret_cast<const T*>(getSmall().data());
		}


	private:
		std::variant<arr,vec> mData{};
		int mIndex{0};
	};
}