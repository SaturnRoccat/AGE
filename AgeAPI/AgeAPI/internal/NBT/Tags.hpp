#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <variant>
#include <span>
#include <unordered_map>
#include <iostream>
#include <format>
#include <print>

namespace AgeAPI::NBT
{
	template<typename T>
	class BinaryStream
	{
	private:
		std::vector<T> mBuffer;
		size_t mOffset;
	public:
		BinaryStream(std::vector<T> buffer = {}) : mBuffer(buffer), mOffset(0) {}

		std::vector<T>& GetBuffer() { return mBuffer; }

		void Write(const T& value)
		{
			ResizeOnInvalidation();
			mBuffer[mOffset++] = value;
		}

		T Read()
		{


			return mBuffer[mOffset++];
		}

		template<typename U>
		void Write(const U& value)
		{
			const T* ptr = reinterpret_cast<const u8*>(&value);
			for (size_t i = 0; i < sizeof(U); i++)
				Write(ptr[i]);
		}

		u8 Peak() const
		{
			return mBuffer[mOffset];
		}

		template<typename U>
		void WriteBuff(const std::span<U> buffer)
		{
			for (U value : buffer)
				Write(value);
		}


		template<typename U>
		U Read()
		{
			U value;
			u8* ptr = reinterpret_cast<u8*>(&value);
			for (size_t i = 0; i < sizeof(U); i++)
				ptr[i] = Read();
			return value;
		}

		std::span<T> Read(size_t count)
		{
			std::span<T> self(mBuffer.data(), mBuffer.size() - mOffset);
			std::span<T> buffer = self.subspan(0, count);
			mOffset += count;
			return buffer;
		}

		void Write(const std::span<T> buffer)
		{
			for (T value : buffer)
				Write(value);
		}


		void GoTo(size_t offset) { mOffset = offset; }
		size_t GetOffset() const { return mOffset; }
	private:
		void ResizeOnInvalidation()
		{
			if (mOffset >= mBuffer.size())
				mBuffer.resize(mBuffer.size() * 2);
		}
	};

	template<typename T>
	concept TagImpl = requires(T t)
	{
		{ t.ReadTag(BinaryStream<u8>{}) } -> std::same_as<T&>;
		{ t.WriteTag(BinaryStream<u8>{}) } -> std::same_as<void>;
		{ t.GetTagId() } -> std::same_as<u8>;
		{ t.PrintTag() } -> std::same_as<void>;
		{ t.GetValue() };
		{ t.GetTagString() } -> std::same_as<const std::string&>;
		{ T{} };
	};


	class TagLongArray
	{
	private:
		std::vector<i64> mValue;
	public:
		TagLongArray(std::vector<i64> value = {}) : mValue(value) {}

		TagLongArray& ReadTag(BinaryStream<u8>& stream)
		{
			i32 size = stream.Read<i32>();
			mValue.clear();
			for (i32 i = 0; i < size; i++)
				mValue.push_back(stream.Read<i64>());
			return *this;
		}

		void WriteTag(BinaryStream<u8>& stream) const
		{
			stream.Write<i32>(mValue.size());
			stream.WriteBuff<i64>(std::span<i64>((i64*)mValue.data(), mValue.size()));
		}

		u8 GetTagId() const
		{
			return 12;
		}

		void PrintTag() const
		{
			std::print("TAG_Long_Array(");
			for (i64 value : mValue)
				std::print("{}, ", value);
			std::print(")\n");
		}

		const std::vector<i64>& GetValue() const
		{
			return mValue;
		}
		std::vector<i64>& GetValue()
		{
			return mValue;
		}
		const std::string& GetTagString() const
		{
			static std::string str = "TAG_Long_Array";
			return str;
		}

	};

	class TagIntArray
	{
	private:
		std::vector<i32> mValue;
	public:
		TagIntArray(std::vector<i32> value = {}) : mValue(value) {}

		TagIntArray& ReadTag(BinaryStream<u8>& stream)
		{
			i32 size = stream.Read<i32>();
			mValue.clear();
			for (i32 i = 0; i < size; i++)
				mValue.push_back(stream.Read<i32>());
			return *this;
		}

		void WriteTag(BinaryStream<u8>& stream) const
		{
			stream.Write<i32>(mValue.size());
			stream.WriteBuff<u8>(std::span<u8>((u8*)mValue.data(), mValue.size() * sizeof(i32)));
		}

		u8 GetTagId() const
		{
			return 11;
		}

		void PrintTag() const
		{
			std::print("TAG_Int_Array(");
			for (i32 value : mValue)
				std::print("{}, ", value);
			std::print(")\n");
		}

		const std::vector<i32>& GetValue() const
		{
			return mValue;
		}
		std::vector<i32>& GetValue()
		{
			return mValue;
		}
		const std::string& GetTagString() const
		{
			static std::string str = "TAG_Int_Array";
			return str;
		}
	};

	class TagCompound;

	class TagList;

	class TagString
	{
	private:
		std::string mValue;
	public:
		TagString(std::string value = "") : mValue(value) {}
		TagString(const char* value) : mValue(value) {}

		TagString& ReadTag(BinaryStream<u8>& stream)
		{
			i16 size = stream.Read<i16>();
			auto Value = stream.Read(size);
			mValue = std::string(Value.begin(), Value.end());
			return *this;
		}

		void WriteTag(BinaryStream<u8>& stream) const
		{
			stream.Write<i16>(mValue.size());
			stream.Write(std::span<u8>((u8*)(mValue.data()), mValue.size())); //Evil
		}

		u8 GetTagId() const
		{
			return 8;
		}

		void PrintTag() const
		{
			std::print("TAG_String(\"{}\")\n", mValue);
		}

		const std::string& GetValue() const
		{
			return mValue;
		}

		std::string& GetValue()
		{
			return mValue;
		}
	

		const std::string& GetTagString() const
		{
			static std::string str = "TAG_String";
			return str;
		}

		bool operator==(const TagString& other) const
		{
			return mValue == other.mValue;
		}
		bool operator!=(const TagString& other) const
		{
			return mValue != other.mValue;
		}
	};
}

namespace std
{
	template<>
	struct hash<AgeAPI::NBT::TagString>
	{
		size_t operator()(const AgeAPI::NBT::TagString& tag) const
		{
			return std::hash<std::string>()(tag.GetValue());
		}
	};

}

namespace AgeAPI::NBT
{
	class TagByteArray
	{
	private:
		std::vector<u8> mValue;
	public:
		TagByteArray(std::vector<u8> value = {}) : mValue(value) {}

		TagByteArray& ReadTag(BinaryStream<u8>& stream)
		{
			i32 size = stream.Read<i32>();
			auto Value = stream.Read(size);
			mValue = std::vector<u8>(Value.begin(), Value.end());
			return *this;
		}
		void WriteTag(BinaryStream<u8>& stream) const
		{
			stream.Write<i32>(mValue.size());
			stream.Write(mValue);
		}
		u8 GetTagId() const
		{
			return 7;
		}
		void PrintTag() const
		{
			std::print("TAG_Byte_Array(");
			for (u8 value : mValue)
				std::print("{}, ", value);
			std::print(")\n");
		}
		const std::vector<u8>& GetValue() const
		{
			return mValue;
		}
		
		std::vector<u8>& GetValue()
		{
			return mValue;
		}
		void PushBack(u8 value)
		{
			mValue.push_back(value);
		}
		const std::string& GetTagString() const
		{
			static std::string str = "TAG_Byte_Array";
			return str;
		}
	};

	class TagDouble
	{
		private:
		f64 mValue;
		public:
		TagDouble(f64 value = 0) : mValue(value) {}

		TagDouble& ReadTag(BinaryStream<u8>& stream)
		{
			mValue = stream.Read<f64>();
			return *this;
		}

		void WriteTag(BinaryStream<u8>& stream) const
		{
			stream.Write(mValue);
		}

		u8 GetTagId() const
		{
			return 6;
		}

		void PrintTag() const
		{
			std::println("TAG_Double({})", mValue);
		}

		const f64& GetValue() const
		{
			return mValue;
		}
		f64& GetValue()
		{
			return mValue;
		}
		const std::string& GetTagString() const
		{
			static std::string str = "TAG_Double";
			return str;
		}
	};

	class TagFloat
	{
	private:
		f32 mValue;
	public:
		TagFloat(f32 value = 0) : mValue(value) {}

		TagFloat& ReadTag(BinaryStream<u8>& stream)
		{
			mValue = stream.Read<f32>();
			return *this;
		}


		void WriteTag(BinaryStream<u8>& stream) const
		{
			stream.Write(mValue);
		}

		u8 GetTagId() const
		{
			return 5;
		}

		void PrintTag() const
		{
			std::println("TAG_Float({})", mValue);
		}

		const f32& GetValue() const
		{
			return mValue;
		}
		f32& GetValue()
		{
			return mValue;
		}
		const std::string& GetTagString() const
		{
			static std::string str = "TAG_Float";
			return str;
		}
	};

	class TagLong
	{
	private:
		i64 mValue;
	public:
		TagLong(i64 value = 0) : mValue(value) {}

		TagLong& ReadTag(BinaryStream<u8>& stream)
		{
			mValue = stream.Read<i64>();
			return *this;
		}

		void WriteTag(BinaryStream<u8>& stream) const
		{
			stream.Write(mValue);
		}

		u8 GetTagId() const
		{
			return 4;
		}

		void PrintTag() const
		{
			std::println("TAG_Long({})", mValue);
		}

		const i64& GetValue() const
		{
			return mValue;
		}
		i64& GetValue()
		{
			return mValue;
		}
		const std::string& GetTagString() const
		{
			static std::string str = "TAG_Long";
			return str;
		}
	};

	class TagInt
	{
	private:
		i32 mValue;
	public:
		TagInt(i32 value = 0) : mValue(value) {}

		TagInt& ReadTag(BinaryStream<u8>& stream)
		{
			mValue = stream.Read<i32>();
			return *this;
		}

		void WriteTag(BinaryStream<u8>& stream) const
		{
			stream.Write(mValue);
		}

		u8 GetTagId() const
		{
			return 3;
		}

		void PrintTag() const
		{
			std::println("TAG_Int({})", mValue);
		}

		const i32& GetValue() const
		{
			return mValue;
		}
		i32& GetValue()
		{
			return mValue;
		}
		const std::string& GetTagString() const
		{
			static std::string str = "TAG_Int";
			return str;
		}
	};

	class TagShort
	{
	private:
		i16 mValue;
	public:
		TagShort(i16 value = 0) : mValue(value) {}

		TagShort& ReadTag(BinaryStream<u8>& stream)
		{
			mValue = stream.Read<i16>();
			return *this;
		}

		void WriteTag(BinaryStream<u8>& stream) const
		{
			stream.Write(mValue);
		}

		u8 GetTagId() const
		{
			return 2;
		}

		void PrintTag() const
		{
			std::println("TAG_Short({})", mValue);
		}

		const i16& GetValue() const
		{
			return mValue;
		}
		i16& GetValue()
		{
			return mValue;
		}
		const std::string& GetTagString() const
		{
			static std::string str = "TAG_Short";
			return str;
		}
	};;

	class TagByte
	{
	private:
		u8 mValue;
	public:

		TagByte(u8 value = 0) : mValue(value) {}

		TagByte& ReadTag(BinaryStream<u8>& stream)
		{
			mValue = stream.Read<u8>();
			return *this;
		}

		void WriteTag(BinaryStream<u8>& stream) const
		{
			stream.Write(mValue);
		}

		u8 GetTagId() const
		{
			return 1;
		}

		void PrintTag() const
		{
			std::println("TAG_Byte({})", mValue);
		}

		const u8& GetValue() const
		{
			return mValue;
		}
		u8& GetValue()
		{
			return mValue;
		}
		const std::string& GetTagString() const
		{
			static std::string str = "TAG_Byte";
			return str;
		}
	};

	class TagEnd
	{

	private:
	public:

		TagEnd() {}

		TagEnd& ReadTag(BinaryStream<u8>& stream)
		{
			return *this;
		}

		void WriteTag(BinaryStream<u8>& stream) const
		{
		}

		u8 GetTagId() const
		{
			return 0;
		}

		const u8& GetValue() const
		{
			return 0;
		}
		u8& GetValue()
		{
			static u8 value = 0;
			return value;
		}


		void PrintTag() const
		{
			std::println("TAG_End");
		}

		const std::string& GetTagString() const
		{
			static std::string str = "TAG_End";
			return str;
		}

	};
	using AnyTag = 
		std::variant<TagByte, TagShort, TagInt, TagLong, TagFloat, TagDouble, TagString, TagByteArray, TagIntArray, TagLongArray, TagCompound, TagList, TagEnd>;

	using TagArray =
		std::variant<
			std::vector<TagEnd>,
			std::vector<TagByte>,
			std::vector<TagShort>,
			std::vector<TagInt>,
			std::vector<TagLong>,
			std::vector<TagFloat>,
			std::vector<TagDouble>,
			std::vector<TagByteArray>,
			std::vector<TagString>,
			std::vector<TagList>,
			std::vector<TagCompound>,
			std::vector<TagIntArray>,
			std::vector<TagLongArray>
		>;

	using TagMap = std::unordered_map<TagString, AnyTag>;

	AnyTag ConstructBasedOnID(u8 id);
	// Little hack to get around the fact that we can't have a recursive type
	class TagListType
	{
	public:
		TagListType() = default;
	};
	class TagCompoundType
	{
	public:
		TagCompoundType() = default;
	};

	class TagList
	{
	private:
		TagArray mValue;
	public:
		TagList(const TagArray& value = {}) : mValue(value) {}
		/*TagList(TagByte value) : mValue(std::vector<TagByte>{}) {}
		TagList(TagShort value) : mValue(std::vector<TagShort>{}) {}
		TagList(TagInt value) : mValue(std::vector<TagInt>{}) {}
		TagList(TagLong value) : mValue(std::vector<TagLong>{}) {}
		TagList(TagFloat value) : mValue(std::vector<TagFloat>{}) {}
		TagList(TagDouble value) : mValue(std::vector<TagDouble>{}) {}
		TagList(TagByteArray value) : mValue(std::vector<TagByteArray>{}) {}
		TagList(TagString value) : mValue(std::vector<TagString>{}) {}
		TagList(TagListType value) : mValue(std::vector<TagList>{}) {}
		TagList(TagCompoundType value) : mValue(std::vector<TagCompound>{}) {}
		TagList(TagIntArray value) : mValue(std::vector<TagIntArray>{}) {}
		TagList(TagLongArray value) : mValue(std::vector<TagLongArray>{}) {}*/

		void SetType(TagListType)
		{
			mValue = std::vector<TagList>{};
		}


		TagList& ReadTag(BinaryStream<u8>& stream);

		/*void PushBack(const AnyTag& tag)
		{
			std::visit([&](auto& value)
				{
					using VecType = typename std::remove_reference_t<decltype(value)>::value_type;
					value.push_back
					(
						std::get<VecType>(tag)
					);
				}, mValue);
		}
		*/
		template<typename T>
		void PushBack(const T& value)
		{
			std::get<std::vector<T>>(mValue).push_back(value);
		}
		template<typename T>
		void EmplaceBack(T&& value)
		{
			std::get<std::vector<T>>(mValue).emplace_back(std::move(value));
		}

		void WriteTag(BinaryStream<u8>& stream) const
		{
			std::visit([&](auto& value)
				{
					if (value.size() == 0)
					{
						stream.Write<u8>(0);
						stream.Write<i32>(0);
						return;
					}
					stream.Write<u8>(value[0].GetTagId());
					stream.Write<i32>(value.size());
					for (auto& tag : value)
						tag.WriteTag(stream);
				}, mValue);
		}

		u8 GetTagId() const
		{
			return 9;
		}

		void PrintTag() const
		{
			std::print("TAG_List(");
			std::visit([&](auto& value)
				{
					for (auto& tag : value)
						tag.PrintTag();
				}, mValue);
			std::print(")\n");
		}

		const TagArray& GetValue() const
		{
			return mValue;
		}
		TagArray& GetValue()
		{
			return mValue;
		}
		const std::string& GetTagString() const
		{
			static std::string str = "TAG_List";
			return str;
		}
		void AbsorbTagArray(TagArray&& other)
		{
			mValue = std::move(other);
		}
	};

	class TagCompound
	{
	private:
		TagMap mValue;
	public:
		TagCompound(const TagMap& value = {}) : mValue(value) {}

		TagCompound& ReadTag(BinaryStream<u8>& stream)
		{
			mValue.clear();
			while (true)
			{
				u8 type = stream.Read<u8>();
				if (type == 0)
				{
					break;
				}
				TagString key;
				key.ReadTag(stream);
				AnyTag tag = ConstructBasedOnID(type);
				std::visit([&](auto& value)
					{
						value.ReadTag(stream);
					}, tag);
				mValue[key] = tag;
			}
			return *this;
		}

		void WriteTag(BinaryStream<u8>& stream) const
		{
			for (auto& [key, tag] : mValue)
			{
				std::visit([&](auto& value)
					{
						stream.Write<u8>(value.GetTagId());
						key.WriteTag(stream);
						value.WriteTag(stream);
					}, tag);
			}
			stream.Write<u8>(0);
		}

		u8 GetTagId() const
		{
			return 10;
		}

		void PrintTag() const
		{
			std::print("TAG_Compound(");
			for (auto& [key, tag] : mValue)
			{
				std::print("\"{}\": ", key.GetValue());
				std::visit([&](auto& value)
					{
						value.PrintTag();
					}, tag);
			}
			std::print(")\n");
		}
	
		template<typename T>
		T& GetTag(const TagString& key) 
		{ 
			return std::get<T>(mValue.at(key));
		}

		const TagMap& GetValue() const
		{
			return mValue;
		}
		TagMap& GetValue()
		{
			return mValue;
		}
		const std::string& GetTagString() const
		{
			static std::string str = "TAG_Compound";
			return str;
		}

		template<typename T>
		T& GetValue(const TagString& key) { return GetTag<T>(key).GetValue(); }

		template<>
		std::string& GetValue<std::string>(const TagString& key)
		{
			return GetTag<TagString>(key).GetValue();
		}
		template<>
		std::vector<u8>& GetValue<std::vector<u8>>(const TagString& key)
		{
			return GetTag<TagByteArray>(key).GetValue();
		}

		template<>
		std::vector<i32>& GetValue<std::vector<i32>>(const TagString& key)
		{
			 return GetTag<TagIntArray>(key).GetValue();
			//return std::get<std::vector<i32>>(GetTag<TagIntArray>(key));
		}

		template<>
		std::vector<i64>& GetValue<std::vector<i64>>(const TagString& key)
		{
			return GetTag<TagLongArray>(key).GetValue();
			//return std::get<std::vector<i64>>(GetTag<TagLongArray>(key));
		}


		template<std::floating_point T>
		T GetValue(const TagString& key)
		{
			if constexpr (std::is_same_v<T, f32>)
				return GetTag<TagFloat>(key).GetValue();
			else
				return GetTag<TagDouble>(key).GetValue();

		}
		template<std::integral T>
		T GetValue(const TagString& key)
		{
			if constexpr (std::is_same_v<T, i8>)
				return GetTag<TagByte>(key).GetValue();
			else if constexpr (std::is_same_v<T, i16>)
				return GetTag<TagShort>(key).GetValue();
			else if constexpr (std::is_same_v<T, i32>)
				return GetTag<TagInt>(key).GetValue();
			else if constexpr (std::is_same_v<T, i64>)
				return GetTag<TagLong>(key).GetValue();
		}

		auto& operator[](const TagString& key)
		{
			return mValue[key];
		}

		i64 size() const
		{
			return mValue.size();
		}


		const auto& find(const TagString& key) const
		{
			return mValue.find(key);
		}

		const auto& at(const TagString& key) const
		{
			return mValue.at(key);
		}
	};

}
