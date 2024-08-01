#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>

namespace AgeData::BlockComponents
{
    using AgeAPI::u8;
    using AgeAPI::IVec3;
    using AgeAPI::FVec3;
    class Tag : public AgeAPI::Components::BlockComponentBase
    {
    public:
        template<typename T> requires std::is_constructible_v<std::string, T&&>
        Tag(
            T&& tag
        ) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 60 },
            ""
        ), mTag(std::forward<T>(tag))
        {
            this->SetIdentifier(std::format("tag:{}", mTag));
        }

        BlockComponentBase* Clone() const override {
            return new Tag(*this);
        }
        void SetTag(const std::string& tag) 
        { 
            mTag = tag;
            this->SetIdentifier(std::format("tag:{}", tag));
        }
        const std::string& GetTag() const { return mTag; }

        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) const override 
        {
            return {};
        }
    private:
        std::string mTag{};
    };
}