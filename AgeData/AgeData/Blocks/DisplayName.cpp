#include <AgeData/Blocks/DisplayName.hpp>
namespace AgeData::BlockComponents
{
    AgeAPI::ErrorString DisplayName::WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon,
        AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const
    {
        proxy.mWriteLoc.SetString(mLocalisationKey, proxy.mAllocator);
        return "";
    }
} 