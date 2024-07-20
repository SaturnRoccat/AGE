#pragma once
#include <memory>
#include <stdexcept>
#include <vector>
#include <array>
#include <variant>
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
		using arr = std::array<T, Size>;
	public:
		SmallVector() = default;
		SmallVector(const SmallVector& other)
		{
			if (other.IsSmall())
				mData = std::get<arr>(other.mData);
			else
				mData = std::get<vec>(other.mData);
			mIndex = other.mIndex;
		}
		SmallVector& operator=(const SmallVector& other)
		{
			if (other.IsSmall())
				mData = std::get<arr>(other.mData);
			else
				mData = std::get<vec>(other.mData);
			mIndex = other.mIndex;
			return *this;
		}
		SmallVector(SmallVector&& other) noexcept
		{
			if (other.IsSmall())
				mData = std::get<arr>(std::move(other.mData));
			else
				mData = std::get<vec>(std::move(other.mData));
			mIndex = other.mIndex;
		}
		SmallVector& operator=(SmallVector&& other) noexcept
		{
			if (other.IsSmall())
				mData = std::get<arr>(std::move(other.mData));
			else
				mData = std::get<vec>(std::move(other.mData));
			mIndex = other.mIndex;
			return *this;
		}
		SmallVector(const vec& data)
		{
			reasignData(data);
		}
		SmallVector(vec&& data)
		{
			reasignData(std::move(data));
		}
		SmallVector(const arr& data)
		{
			reasignData(data);
		}
		SmallVector(arr&& data)
		{
			reasignData(std::move(data));
		}
		SmallVector(std::initializer_list<T> data)
		{
			reasignData(data);
		}
		SmallVector& operator=(const vec& data)
		{
			reasignData(data);
			return *this;
		}
		SmallVector& operator=(vec&& data)
		{
			reasignData(std::move(data));
			return *this;
		}
		SmallVector& operator=(const arr& data)
		{
			reasignData(data);
			return *this;
		}
		SmallVector& operator=(arr&& data)
		{
			reasignData(std::move(data));
			return *this;
		}
		SmallVector& operator=(std::initializer_list<T> data)
		{
			reasignData(data);
			return *this;
		}
		reference operator[](size_type index)
		{
			if (IsSmall())
				return std::get<arr>(mData)[index];
			return std::get<vec>(mData)[index];
		}
		const_reference operator[](size_type index) const
		{
			if (index >= mIndex)
				throw std::out_of_range("Index Out Of Range");
			if (IsSmall())
				return std::get<arr>(mData)[index];
			return std::get<vec>(mData)[index];
		}
		reference at(size_type index)
		{
			if (index >= mIndex)
				throw std::out_of_range("Index Out Of Range");
			if (IsSmall())
				return std::get<arr>(mData)[index];
			return std::get<vec>(mData)[index];
		}
		const_reference at(size_type index) const
		{
			if (index >= mIndex)
				throw std::out_of_range("Index Out Of Range");
			if (IsSmall())
				return std::get<arr>(mData)[index];
			return std::get<vec>(mData)[index];
		}
		reference front()
		{
			if (mIndex == 0)
				throw std::out_of_range("Index Out Of Range");
			if (IsSmall())
				return std::get<arr>(mData).front();
			return std::get<vec>(mData).front();
		}
		const_reference front() const
		{
			if (mIndex == 0)
				throw std::out_of_range("Index Out Of Range");
			if (IsSmall())
				return std::get<arr>(mData).front();
			return std::get<vec>(mData).front();
		}
		reference back()
		{
			if (mIndex == 0)
				throw std::out_of_range("Index Out Of Range");
			if (IsSmall())
				return std::get<arr>(mData).back();
			return std::get<vec>(mData).back();
		}
		const_reference back() const
		{
			if (mIndex == 0)
				throw std::out_of_range("Index Out Of Range");
			if (IsSmall())
				return std::get<arr>(mData).back();
			return std::get<vec>(mData).back();
		}
		iterator begin()
		{
			if (IsSmall())
				return std::get<arr>(mData).begin();
			return std::get<vec>(mData).begin();
		}
		const_iterator begin() const
		{
			if (IsSmall())
				return std::get<arr>(mData).data();
			return std::get<vec>(mData).data();

		}
		const_iterator cbegin() const
		{
			if (IsSmall())
				return std::get<arr>(mData).cbegin();
			return std::get<vec>(mData).cbegin();
		}
		iterator end()
		{
			if (IsSmall())
				return std::get<arr>(mData).begin() + mIndex;
			return std::get<vec>(mData).end();
		}
		const_iterator end() const
		{
			if (IsSmall())
				return std::get<arr>(mData).data() + mIndex;
			return std::get<vec>(mData).data() + std::get<vec>(mData).size();
		}
		const_iterator cend() const
		{
			if (IsSmall())
				return std::get<arr>(mData).cbegin() + mIndex;
			return std::get<vec>(mData).cend();
		}
		reverse_iterator rbegin()
		{
			if (IsSmall())
				return std::get<arr>(mData).rbegin();
			return std::get<vec>(mData).rbegin();
		}
		const_reverse_iterator rbegin() const
		{
			if (IsSmall())
				return std::get<arr>(mData).rbegin();
			return std::get<vec>(mData).rbegin();
		}
		const_reverse_iterator crbegin() const
		{
			if (IsSmall())
				return std::get<arr>(mData).crbegin();
			return std::get<vec>(mData).crbegin();
		}
		reverse_iterator rend()
		{
			if (IsSmall())
				return std::get<arr>(mData).rbegin() + mIndex;
			return std::get<vec>(mData).rend();
		}
		const_reverse_iterator rend() const
		{
			if (IsSmall())
				return std::get<arr>(mData).rbegin() + mIndex;
			return std::get<vec>(mData).rend();
		}
		const_reverse_iterator crend() const
		{
			if (IsSmall())
				return std::get<arr>(mData).crbegin() + mIndex;
			return std::get<vec>(mData).crend();
		}
		bool empty() const
		{
			return mIndex == 0;
		}
		size_type size() const
		{
			if (IsSmall())
				return mIndex;
			return std::get<vec>(mData).size();
		}
		size_type max_size() const
		{
			return std::numeric_limits<size_type>::max();
		}
		void reserve(size_type new_cap)
		{
			if (doesSizeFitInSmall(new_cap))
				return;
			if (IsSmall())
			{
				auto& array = std::get<arr>(mData);
				mData = vec(array.begin(), array.begin() + mIndex);
			}
			std::get<vec>(mData).reserve(new_cap);
		}
		size_type capacity() const
		{
			if (IsSmall())
				return Size;
			return std::get<vec>(mData).capacity();
		}
		void shrink_to_fit()
		{
			if (IsSmall())
				return;
			std::get<vec>(mData).shrink_to_fit();
		}
		void clear()
		{
			if (IsSmall())
			{
				for (auto& elem : std::get<arr>(mData))
					elem.~T();
				mIndex = 0;
				return;
			}
			std::get<vec>(mData).clear();
		}
		iterator insert(const_iterator pos, const T& value)
		{
			if (mIndex == Size)
			{
				moveDataToVec();
			}
			if (IsSmall())
			{
				auto& array = std::get<arr>(mData);
				auto index = pos - array.begin();
				for (size_t i = mIndex; i > index; --i)
					array[i] = array[i - 1];
				array[index] = value;
				++mIndex;
				return array.begin() + index;
			}
			return std::get<vec>(mData).insert(pos, value);
		}
		iterator insert(const_iterator pos, T&& value)
		{
			if (mIndex == Size)
			{
				moveDataToVec();
			}
			if (IsSmall())
			{
				auto& array = std::get<arr>(mData);
				auto index = pos - array.begin();
				for (size_t i = mIndex; i > index; --i)
					array[i] = array[i - 1];
				array[index] = std::move(value);
				++mIndex;
				return array.begin() + index;
			}
			return std::get<vec>(mData).insert(pos, std::move(value));
		}
		iterator insert(const_iterator pos, size_type count, const T& value)
		{
			if (mIndex + count > Size)
			{
				moveDataToVec();
			}
			if (IsSmall())
			{
				auto& array = std::get<arr>(mData);
				auto index = pos - array.begin();
				for (size_t i = mIndex; i > index; --i)
					array[i + count - 1] = array[i - 1];
				for (size_t i = 0; i < count; ++i)
					array[index + i] = value;
				mIndex += count;
				return array.begin() + index;
			}
			return std::get<vec>(mData).insert(pos, count, value);
		}
		template<typename InputIt>
		iterator insert(const_iterator pos, InputIt first, InputIt last)
		{
			auto count = std::distance(first, last);
			if (mIndex + count > Size)
			{
				moveDataToVec();
			}
			if (IsSmall())
			{
				auto& array = std::get<arr>(mData);
				auto index = pos - array.begin();
				for (size_t i = mIndex; i > index; --i)
					array[i + count - 1] = array[i - 1];
				for (size_t i = 0; i < count; ++i)
					array[index + i] = *first++;
				mIndex += count;
				return array.begin() + index;
			}
			return std::get<vec>(mData).insert(pos, first, last);
		}
		iterator insert(const_iterator pos, std::initializer_list<T> ilist)
		{
			auto count = ilist.size();
			if (mIndex + count > Size)
			{
				moveDataToVec();
			}
			if (IsSmall())
			{
				auto& array = std::get<arr>(mData);
				auto index = pos - array.begin();
				for (size_t i = mIndex; i > index; --i)
					array[i + count - 1] = array[i - 1];
				for (size_t i = 0; i < count; ++i)
					array[index + i] = ilist.begin()[i];
				mIndex += count;
				return array.begin() + index;
			}
			return std::get<vec>(mData).insert(pos, ilist);
		}
		template<typename... Args>
		iterator emplace(const_iterator pos, Args&&... args)
		{
			if (mIndex == Size)
			{
				moveDataToVec();
			}
			if (IsSmall())
			{
				auto& array = std::get<arr>(mData);
				auto index = pos - array.begin();
				for (size_t i = mIndex; i > index; --i)
					array[i] = array[i - 1];
				array[index] = T(std::forward<Args>(args)...);
				++mIndex;
				return array.begin() + index;
			}
			return std::get<vec>(mData).emplace(pos, std::forward<Args>(args)...);
		}
		iterator erase(const_iterator pos)
		{
			if (IsSmall())
			{
				auto& array = std::get<arr>(mData);
				auto index = pos - array.begin();
				for (size_t i = index; i < mIndex - 1; ++i)
					array[i] = array[i + 1];
				--mIndex;
				return array.begin() + index;
			}
			return std::get<vec>(mData).erase(pos);
		}
		iterator erase(const_iterator first, const_iterator last)
		{
			if (IsSmall())
			{
				auto& array = std::get<arr>(mData);
				auto index = first - array.begin();
				auto count = last - first;
				for (size_t i = index; i < mIndex - count; ++i)
					array[i] = array[i + count];
				mIndex -= count;
				return array.begin() + index;
			}
			return std::get<vec>(mData).erase(first, last);
		}
		void push_back(const T& value)
		{
			if (mIndex == Size)
			{
				moveDataToVec();
			}
			if (IsSmall())
			{
				std::get<arr>(mData)[mIndex++] = value;
				return;
			}
			mIndex++;
			std::get<vec>(mData).push_back(value);
		}
		void push_back(T&& value)
		{
			if (mIndex == Size)
			{
				moveDataToVec();
			}
			if (IsSmall())
			{
				std::get<arr>(mData)[mIndex++] = std::move(value);
				return;
			}
			std::get<vec>(mData).push_back(std::move(value));
		}
		template<typename... Args>
		reference emplace_back(Args&&... args)
		{
			if (mIndex == Size)
			{
				moveDataToVec();
			}
			if (IsSmall())
			{
				auto& array = std::get<arr>(mData);
				array[mIndex] = T(std::forward<Args>(args)...);
				return array[mIndex++];
			}
			mIndex++;
			return std::get<vec>(mData).emplace_back(std::forward<Args>(args)...);
		}
		void pop_back()
		{
			if (mIndex == 0)
				throw std::out_of_range("Index Out Of Range");
			if (IsSmall())
			{
				--mIndex;
				return;
			}
			--mIndex;
			std::get<vec>(mData).pop_back();
		}
		void resize(size_type count)
		{
			if (count > Size)
			{
				moveDataToVec();
			}
			if (IsSmall())
			{
				auto& array = std::get<arr>(mData);
				if (count > mIndex)
				{
					for (size_t i = mIndex; i < count; ++i)
						array[i] = T();
				}
				mIndex = count;
				return;
			}
			mIndex = count;
			std::get<vec>(mData).resize(count);
		}
		void resize(size_type count, const T& value)
		{
			if (count > Size)
			{
				moveDataToVec();
			}
			if (IsSmall())
			{
				auto& array = std::get<arr>(mData);
				if (count > mIndex)
				{
					for (size_t i = mIndex; i < count; ++i)
						array[i] = value;
				}
				mIndex = count;
				return;
			}
			std::get<vec>(mData).resize(count, value);
		}
		void swap(SmallVector& other)
		{
			std::swap(mData, other.mData);
			std::swap(mIndex, other.mIndex);
		}
		~SmallVector() = default;

		T& Get(size_t index)
		{
			return get(index);
		}
		const T& Get(size_t index) const
		{
			return get(index);
		}
		T& At(size_t index)
		{
			if (index >= mIndex)
				throw std::out_of_range("Index Out Of Range");
			return get(index);
		}
		const T& At(size_t index) const
		{
			if (index >= mIndex)
				throw std::out_of_range("Index Out Of Range");
			return get(index);
		}


		bool IsSmall() const
		{
			return std::holds_alternative<arr>(mData);
		}
	private:
		bool doesSizeFitInSmall(size_t size)
		{
			return size <= Size;
		}
		void moveDataToVec()
		{
			if (IsSmall())
			{
				auto& array = std::get<arr>(mData);
				mData = vec(array.begin(), array.begin() + mIndex);
			}
		}
		template<typename ContainerType>
		void reasignData(ContainerType&& data)
		{
			if (doesSizeFitInSmall(data.size()))
			{
				mData = std::array<T, Size>{};
				std::copy(data.begin(), data.end(), std::get<arr>(mData).begin());
			}
			else
			{
				mData = std::vector<T>(data.begin(), data.end());
			}
			mIndex = data.size();
		}
		T& get(size_t index)
		{
			if (IsSmall())
				return std::get<arr>(mData)[index];
			return std::get<vec>(mData)[index];
		}
		const T& get(size_t index) const
		{
			if (IsSmall())
				return std::get<arr>(mData)[index];
			return std::get<vec>(mData)[index];
		}
		template<typename ContainerType>
		void reasignData(const ContainerType& data)
		{
			if (doesSizeFitInSmall(data.size()))
			{
				mData = std::array<T, Size>{};
				std::copy(data.begin(), data.end(), std::get<arr>(mData).begin());
			}
			else
			{
				mData = std::vector<T>(data.begin(), data.end());
			}
			mIndex = data.size();
		}
	private:
		std::variant<arr,vec> mData{};
		int mIndex{0};
	};
}