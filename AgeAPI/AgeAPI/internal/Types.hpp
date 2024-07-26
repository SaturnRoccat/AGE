#ifndef AGE_API_TYPES

#define AGE_API_TYPES
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <format>
#include <AgeAPI/internal/Error.hpp>
#include <AgeAPI/internal/Utils/BasicTypeDecls.hpp>
#include <AgeAPI/internal/Utils/SmallUtilities.hpp>
#include <AgeAPI/internal/Utils/SemanticVersion.hpp>
#include <AgeAPI/internal/Utils/Identifier.hpp>
#include <AgeAPI/internal/Utils/ExperimentalSettings.hpp>
#include <AgeAPI/internal/Utils/Vec.hpp>
#include <AgeAPI/internal/Utils/JsonProxy.hpp>
#include <AgeAPI/internal/Utils/Color.hpp>
#include <AgeAPI/internal/Utils/BoundingBox.hpp>
#include <AgeAPI/internal/Utils/Reference.hpp>
#include <AgeAPI/internal/Utils/ItemGroup.hpp>
#include <AgeAPI/internal/Utils/MenuCategory.hpp>
#include <AgeAPI/internal/Utils/UUID.hpp>
#include <absl/container/inlined_vector.h>
#include <absl/container/flat_hash_map.h>


#include <filesystem>
#include <optional>
#include <charconv>
#include <concepts>

namespace AgeAPI
{
	class Addon; // Used all over the place




};


namespace std
{

	template<typename T>
	struct hash<AgeAPI::BoundingBox<T>>
	{
		std::size_t operator()(const AgeAPI::BoundingBox<T>& box) const
		{
			return std::hash<T>{}(box.min) ^ std::hash<T>{}(box.max);
		}
	};

	template<>
	struct hash<AgeAPI::Color>
	{
		std::size_t operator()(const AgeAPI::Color& color) const
		{
			return std::hash<float>{}(color.x) ^ std::hash<float>{}(color.y) ^ std::hash<float>{}(color.z) ^ std::hash<float>{}(color.w);
		}
	};
}
#endif // !AGE_API_TYPES_H