#ifndef AGE_DATA_TYPES_HPP
#define AGE_DATA_TYPES_HPP
#include <AgeAPI/internal/Types.hpp>
#include <variant> 




namespace AgeData
{

    template<typename T, bool Default>
    class ToggleOrData
    {
    public:
        ToggleOrData() : mValue(false) {}
        ToggleOrData(const T& value) : mValue(value) {}
        ToggleOrData(bool value) : mValue(value) {}

        bool IsToggle() const { return std::holds_alternative<bool>(mValue); }

        bool GetToggle() const { return std::get<bool>(mValue); }
        void SetToggle(bool value) { mValue = value; }

        const T& GetData() const
        {
            return std::get<T>(mValue);
        }
        void SetData(const T& value) { mValue = value; }
        AgeAPI::ErrorString WriteToJson(const AgeAPI::JsonProxy& proxy) const
        {
            if (IsToggle())
                proxy.mWriteLoc.SetBool(GetToggle());
            else
                GetData().WriteToJson(proxy);
            return "";
        }

    private:
        std::variant<T, bool> mValue = Default;

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
        bool IsValid() const { return AgeAPI::BoundingBox<T>::Contains(mValue); }
    private:
        T mValue;
    };

    class BlockBounds : private std::pair<AgeAPI::IVec3, AgeAPI::IVec3>
    {
    public:
        BlockBounds(
            /// The minimal position of the bounds of the bounding box
            const AgeAPI::IVec3& origin = AgeAPI::IVec3(-8, 0, -8),
            /// The size of each side of the bounding box
            const  AgeAPI::IVec3& size = AgeAPI::IVec3(16, 16, 16)
        ) : std::pair<AgeAPI::IVec3, AgeAPI::IVec3>(
            size, origin

        ) {}

        bool IsValid() const
        {
            auto val = this->first + this->second;
            static AgeAPI::BoundingBox<AgeAPI::IVec3> bounds({ -8, 0, -8 }, { 8, 16, 8 });
            return bounds.Contains(val);
        }

        void WriteToJson(AgeAPI::JsonProxy proxy) const;
        const auto& GetOrigin() const { return this->second; }
        const auto& GetSize() const { return this->first; }
    };



}
#endif