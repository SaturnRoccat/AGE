#ifndef AGE_DATA_LOOT_BLOCK_HPP
#define AGE_DATA_LOOT_BLOCK_HPP
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>

namespace AgeData::BlockComponents
{
    class Loot : public AgeAPI::Components::BlockComponentBase
    {
    public:
        Loot(const std::string& val = "") : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 50 },
            "minecraft:loot"
        ), mValue(val) {
        }

        void SetTable(const std::string& val) { mValue = val; }
        const std::string& GetTable() const { return mValue; }
        //TODO: Implement Ctors to allow binding with the LootTable class in the future
        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const override
        {
            proxy.mWriteLoc.SetString(mValue, proxy.mAllocator);
        }
        BlockComponentBase* Clone() const override {
            return new Loot(mValue);
        }
    private:
        std::string mValue{};
    };
}
#endif // AGE_DATA_LOOT_BLOCK_HPP