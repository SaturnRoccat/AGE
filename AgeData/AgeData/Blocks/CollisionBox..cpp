
#include "CollisionBox.hpp"

namespace AgeData::BlockComponents
{
    inline AgeAPI::ErrorString CollisionBox::WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon,
        AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const
    {
        return mValue.WriteToJson(proxy);
    }
}