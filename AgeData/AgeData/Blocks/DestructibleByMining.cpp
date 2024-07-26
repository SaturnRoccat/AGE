#include <AgeData/Blocks/DestructibleByMining.hpp>
namespace AgeData::BlockComponents
{
    AgeAPI::ErrorString DestructibleByMining::WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon,
        AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) const
    {
        if (mValue.IsToggle())
            proxy.mWriteLoc.SetBool(mValue.GetToggle());
        else
            proxy.mWriteLoc.AddMember("seconds_to_destroy", mValue.GetData(), proxy.mAllocator);
        return "";
    }
} 