#pragma once
#include <string>
#include <filesystem>
#include <concepts>
namespace AgeAPI::Backend::Rp
{
	template<typename T>
	concept IsCopyRef = std::is_reference_v<T> && std::is_copy_constructible_v<std::remove_reference_t<T>>;

	template<typename T>
	concept IsBaseString = std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, std::string>;

	class Geometry
	{
	public: 
		Geometry() = default;
		Geometry(const std::string& geoCopyPath, const std::string& geoOutputPath, const std::string& geoName)
			: mGeoCopyPath(geoCopyPath), mGeoOutputPath(geoOutputPath), mGeoName(geoName) {}
		template<typename T, typename U, typename V> requires IsBaseString<T> && IsBaseString<U> && IsBaseString<V>
		Geometry(T geoCopyPath, U geoOutputPath)
		{
			if constexpr (IsCopyRef<T>)
				mGeoCopyPath = geoCopyPath;
			else 
				mGeoCopyPath = std::move(geoCopyPath);
			if constexpr (IsCopyRef<U>)
				mGeoOutputPath = geoOutputPath;
			else
				mGeoOutputPath = std::move(geoOutputPath);
			if constexpr (IsCopyRef<V>)
				mGeoName = geoOutputPath;
			else
				mGeoName = std::move(geoOutputPath);
		}

		const std::string& GetGeoCopyPath() const { return mGeoCopyPath; }
		const std::string& GetGeoOutputPath() const { return mGeoOutputPath; }
		const std::string& GetGeoName() const { return mGeoName; }

		void Write(const std::filesystem::path& base) const;

	private:
		std::string mGeoCopyPath{};
		std::string mGeoOutputPath{};
		std::string mGeoName{};

	};


}