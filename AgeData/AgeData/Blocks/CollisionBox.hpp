#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>
>

namespace AgeData::BlockComponents
{
    class CollisionBox : public AgeAPI::Components::BlockComponentBase
    {
    public:
    
        CollisionBox(AgeAPI::IVec3 origin, AgeAPI::IVec3 size) : AgeAPI::Components::BlockComponentBase(
            0, 
            {1, 19, 50},
            "minecraft:collision_box"
        ), mValue({origin, size}) {}
        
        CollisionBox(bool value) : AgeAPI::Components::BlockComponentBase(
            0, 
            {1, 19, 50},
            "minecraft:collision_box"
        ), mValue(value) {}

        

    private:
        ToggleOrData<BlockBounds> mValue;
        
    };
}