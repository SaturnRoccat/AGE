#include <AgeData/Blocks/DestructibleByExplosion.hpp>
namespace AgeData::BlockComponents
{
    AgeAPI::ErrorString DestructibleByExplosion::WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon,
        AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) const
    {
        if (mValue.IsToggle())
            proxy.mWriteLoc.SetBool(mValue.GetToggle());
        else
            proxy.mWriteLoc.AddMember("explosion_resistance", mValue.GetData(), proxy.mAllocator);
        return "";
    }
} 