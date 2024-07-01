#include <AgeData/Blocks/Flammable.hpp>
namespace AgeData::BlockComponents
{
    AgeAPI::ErrorString Flammable::WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon,
        AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const
    {
        auto& [writeLocation, alloc] = proxy;
        if (mValue.IsToggle())
            writeLocation.SetBool(mValue.GetToggle());
        else
        {
            writeLocation.SetObject();
            writeLocation.AddMember("catch_chance_modifier", mValue.GetData().mCatchChanceModifier, alloc);
            writeLocation.AddMember("destroy_chance_modifier", mValue.GetData().mDestroyChanceModifier, alloc);
        }
        return "";
    }
} 