#include "Models.hpp"

void AgeAPI::Backend::Rp::Geometry::Write(const std::filesystem::path& base) const
{
	// Force base path exists
	std::filesystem::create_directories(base);
	std::filesystem::path GeoPath = base / mGeoOutputPath;

	std::filesystem::copy_file(mGeoCopyPath, GeoPath, std::filesystem::copy_options::overwrite_existing);
}
