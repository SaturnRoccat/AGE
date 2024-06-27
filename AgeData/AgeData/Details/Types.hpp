#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <variant> 




namespace AgeData
{

    template<typename T>
    class ToggleOrData
    {
    public:
        ToggleOrData() : mValue(false) {}
        ToggleOrData(const T& value) : mValue(value) {}
        ToggleOrData(bool value) : mValue(value) {}

        bool IsToggle() const { return std::holds_alternative<bool>(mValue); }

        bool GetToggle() const { return std::get<bool>(mValue); }
        void SetToggle(bool value) { mValue = value; }

        T GetData() const { return std::get<T>(mValue); }
        void SetData(const T& value) { mValue = value; }
    private:
        std::variant<T, bool> mValue;
    };

    template<typename T>
    class BoundedValue : public AgeAPI::BoundingBox<T>
    {
    public:
        BoundedValue(
            T value = T(),
            T min = T(),
            T max = T()
        ) : AgeAPI::BoundingBox<T>(min, max), mValue(value) {}
        BoundedValue(
            T value,
            AgeAPI::BoundingBox<T> bounds
        ) : AgeAPI::BoundingBox<T>(bounds), mValue(value) {}

        bool IsValid() const
        {
            return AgeAPI::BoundingBox<T>::Contains(mValue);
        }
    private:
        T mValue;
    };

    class BlockBounds : private std::pair<AgeAPI::FVec3, AgeAPI::FVec3>
    {
    public:
        BlockBounds(
            /// The minimal position of the bounds of the bounding box
            const AgeAPI::FVec3& origin = AgeAPI::FVec3(-8.f, 0.f, -8.f),
            /// The size of each side of the bounding box
           const  AgeAPI::FVec3& size = AgeAPI::FVec3(16.f, 16.f, 16.f)
        ) : std::pair<AgeAPI::FVec3, AgeAPI::FVec3>(
            size,origin           

        ) {}

        bool IsValid() const
        {
            auto val = this->first + this->second;
            static AgeAPI::BoundingBox<AgeAPI::FVec3> bounds({-8.f, 0.f, -8.f}, {8.f, 16.f, 8.f});
            return bounds.Contains(val);
        }
        
    };
    


}
