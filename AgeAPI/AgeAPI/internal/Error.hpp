#pragma once
#include <string>
#include <expected>

namespace AgeAPI
{
	class Error
	{
	public:
		Error() = default;
		virtual std::string GetAsString() = 0;
		virtual ~Error() = default;
		virtual bool ContainsError() { return true; }
	private:
	};

	class ErrorString : public Error
	{
	public:
		ErrorString() = default;
		ErrorString(const std::string&& error) : mError(error) {}
		ErrorString(const char* error) : mError(error) {}
		std::string GetAsString() override { return mError; }
		bool ContainsError() override { return !mError.empty(); }
	private:
		std::string mError{};
	};

	template <typename T>
	concept IntegralLike = std::integral<T> || std::is_enum<T>().value;


	template<IntegralLike T= int>
	class ErrorIntegralLike : public Error
	{
		public:
		ErrorIntegralLike() = default;
		ErrorIntegralLike(T error) : mErrorCode(error) {}
		std::string GetAsString() override { return std::to_string(mErrorCode); }
		int GetErrorCode() { return mErrorCode; }
		bool ContainsError() override { return mErrorCode != 0; }
	private:
		T mErrorCode{};
	};


	using ErrorInt = ErrorIntegralLike<int>;
	using ConstructionError = ErrorString;
	using SmallError = ErrorInt;

	enum SyntaxErr : int
	{
		InvalidDotIdentifier,
	};

	using SyntaxError = ErrorIntegralLike<SyntaxErr>;






}