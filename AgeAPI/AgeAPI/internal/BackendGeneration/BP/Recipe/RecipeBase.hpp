#pragma once
#include <AgeAPI/internal/Types.hpp>


namespace AgeAPI::Recipes
{

	class BaseRecipe
	{
	protected:
		int mPriority{ 0 };
		SemanticVersion mFormatVersion{ 1,21,0 };
		Identifier mRecipeIdentifier{};
	public:
		template<class Self>
		auto&& GetIdentifier(this Self&& self)
		{
			return std::forward<Self>(self).mRecipeIdentifier;
		}

		BaseRecipe(const Identifier& ident, SemanticVersion formatVersion = { 1,21,0 }) : mRecipeIdentifier, mFormatVersion{ formatVersion } {}

		virtual void WriteToJson(JsonProxy proxy) = 0; 
	};

}