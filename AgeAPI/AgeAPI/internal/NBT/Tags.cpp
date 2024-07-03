#include <AgeAPI/internal/NBT/Tags.hpp>

namespace AgeAPI::NBT
{
	AnyTag ConstructBasedOnID(u8 id)
	{
		switch (id)
		{
		case 0:
			return TagEnd{};
		case 1:
			return TagByte{};
		case 2:
			return TagShort{};
		case 3:
			return TagInt{};
		case 4:
			return TagLong{};
		case 5:
			return TagFloat{};
		case 6:
			return TagDouble{};
		case 7:
			return TagByteArray{};
		case 8:
			return TagString{};
		case 9:
			return TagList{};
		case 10:
			return TagCompound{};
		case 11:
			return TagIntArray{};
		case 12:
			return TagLongArray{};
		default:
			throw std::runtime_error("Invalid tag type");
		}
	}
	TagList& TagList::ReadTag(BinaryStream<u8>& stream)
	{
		u8 type = stream.Read<u8>();
		i32 size = stream.Read<i32>();
		switch (type)
		{
		case 0:
			return *this;
		case 1:
		{
			auto tag = TagByte{};
			std::vector<decltype(tag)> arr(size);
			for (int i = 0; i < size; i++)
				arr[i] = (tag.ReadTag(stream));
			mValue = std::move(arr);
			break;
		}
		case 2:
		{
			auto tag = TagShort{};
			std::vector<decltype(tag)> arr(size);
			for (int i = 0; i < size; i++)
				arr[i] = (tag.ReadTag(stream));
			mValue = std::move(arr);
			break;

		}
		case 3:
		{
			auto tag = TagInt{};
			std::vector<decltype(tag)> arr(size);
			for (int i = 0; i < size; i++)
				arr[i] = (tag.ReadTag(stream));
			mValue = std::move(arr);
			break;

		}
		case 4:
		{
			auto tag = TagLong{};
			std::vector<decltype(tag)> arr(size);
			for (int i = 0; i < size; i++)
				arr[i] = (tag.ReadTag(stream));
			mValue = std::move(arr);
			break;

		}
		case 5:
		{
			auto tag = TagFloat{};
			std::vector<decltype(tag)> arr(size);
			for (int i = 0; i < size; i++)
				arr[i] = (tag.ReadTag(stream));
			mValue = std::move(arr);
			break;

		}
		case 6:
		{
			auto tag = TagDouble{};
			std::vector<decltype(tag)> arr(size);
			for (int i = 0; i < size; i++)
				arr[i] = (tag.ReadTag(stream));
			mValue = std::move(arr);
			break;

		}
		case 7:
		{
			auto tag = TagByteArray{};
			std::vector<decltype(tag)> arr(size);
			for (int i = 0; i < size; i++)
				arr[i] = (tag.ReadTag(stream));
			mValue = std::move(arr);
			break;

		}
		case 8:
		{
			auto tag = TagString{};
			std::vector<decltype(tag)> arr(size);
			for (int i = 0; i < size; i++)
				arr[i] = (tag.ReadTag(stream));
			mValue = std::move(arr);
			break;

		}
		case 9:
		{
			auto tag = TagList{};
			std::vector<decltype(tag)> arr(size);
			for (int i = 0; i < size; i++)
				arr[i] = (tag.ReadTag(stream));
			mValue = std::move(arr);
			break;

		}
		case 10:
		{
			auto tag = TagCompound{};
			std::vector<decltype(tag)> arr(size);
			for (int i = 0; i < size; i++)
				arr[i] = (tag.ReadTag(stream));
			mValue = std::move(arr);
			break;

		}
		case 11:
		{
			auto tag = TagIntArray{};
			std::vector<decltype(tag)> arr(size);
			for (int i = 0; i < size; i++)
				arr[i] = (tag.ReadTag(stream));
			mValue = std::move(arr);
			break;

		}
		case 12:
		{
			auto tag = TagLongArray{};
			std::vector<decltype(tag)> arr(size);
			for (int i = 0; i < size; i++)
				arr[i] = (tag.ReadTag(stream));
			mValue = std::move(arr);
			break;

		}
		default:
			throw std::runtime_error("Invalid tag type");
		}
		return *this;
	}
}