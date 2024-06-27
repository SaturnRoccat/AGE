
#include "CollisionBox.hpp"

namespace AgeData::BlockComponents
{
    AgeAPI::ErrorString CollisionBox::WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon,
        AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const
    {
        rapidjson::Value val(rapidjson::kObjectType);
        if (mValue.IsToggle())
            val.SetBool(mValue.GetToggle());
        else
        {
            auto& data = mValue.GetData();
            if (!data.IsValid())
                return std::format("Invalid collision box data, Origin: {}, {}, {}. Size: {}, {}, {}",
                    data.GetOrigin().x, data.GetOrigin().y, data.GetOrigin().z,
                    data.GetSize().x, data.GetSize().y, data.GetSize().z);
            data.WriteToJson(proxy.Derive(val));
        }
        proxy.mWriteLoc.AddMember("minecraft:collision_box", val, proxy.mAllocator);
        return "";

    }
}