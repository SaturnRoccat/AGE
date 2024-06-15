#include <AgeSharedUtils.hpp>
#include <AgeAPI/internal/Addon/AddonSpecifics/Manifest.hpp>
#include <print>
#include <array>
#include <rapidjson/document.h>
#include <AgeAPI/internal/BackendGeneration/BP/BehaviourPack.hpp>
#include <rapidjson/prettywriter.h>
#include <AgeAPI/internal/Addon/Addon.hpp>

	


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
using namespace AgeAPI;
using namespace AgeAPI::Components;
using namespace AgeAPI::Backend::Bp;

class TestBeh : public Components::BlockComponentBase
{
public:
	TestBeh() : BlockComponentBase(0, { 1, 20, 80 }, "custom:component") {}
	ErrorString WriteToJson(std::unique_ptr<Addon>& addon, JsonProxy& proxy, NonOwningPtr<Backend::Bp::BlockBehaviour> blk) const override
	{
		auto& allocator = proxy.mAllocator;
		auto& val = proxy.mWriteLoc;
		val.AddMember("test", "test", allocator);
		rapidjson::Value arr(rapidjson::kArrayType);
		for (int i = 0; i < 10; i++)
		{
			rapidjson::Value obj(rapidjson::kObjectType);
			obj.AddMember("test", "test", allocator);
			arr.PushBack(obj, allocator);
		}
		val.AddMember("array", arr, allocator);
		return ErrorString();
	}
};

class TestBeh2 : public Components::BlockComponentBase
{
	public:
	TestBeh2() : BlockComponentBase(0, { 1, 20, 80 }, "custom:componentw") {}
	ErrorString WriteToJson(std::unique_ptr<Addon>& addon, JsonProxy& proxy, NonOwningPtr<Backend::Bp::BlockBehaviour> blk) const override
	{
		auto& allocator = proxy.mAllocator;
		auto& val = proxy.mWriteLoc;
		val.AddMember("val", 100, allocator);
		return ErrorString();
	}
};



int main(int argc, char** argv)
{
	Addon thing;
	auto thingPtr = std::make_unique<Addon>(std::move(thing));



	BlockBehaviour blk("test:ident", { 1, 20, 80 });
	blk.AddBlockComponent(std::make_unique<TestBeh>());
	blk.AddBlockComponent(std::make_unique<TestBeh2>());

	auto doc = blk.BuildBlockBehaviourDocument(thingPtr);
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc.value().Accept(writer);
	std::println("{}", buffer.GetString());
	return 0;
}