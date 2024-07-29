#pragma once
#include <string>
#include <expected>
#include <variant>
#include <type_traits>
#include <optional>

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
		operator bool() { return mError.empty(); }
	private:
		std::string mError{};
	};

	class ErrorInt : public Error
	{
		public:
		ErrorInt() = default;
		ErrorInt(int error) : mErrorCode(error) {}
		std::string GetAsString() override { return std::to_string(mErrorCode); }
		int GetErrorCode() { return mErrorCode; }
		bool ContainsError() override { return mErrorCode != 0; }

	private:
		int mErrorCode{};
	};

	using ConstructionError = ErrorString;
	using SmallError = ErrorInt;

	template<typename T, typename E>
	class ReferenceExpected
	{
	public:
		using Ty = std::remove_reference_t<T>;
		ReferenceExpected(Ty& value) : mResult(std::ref(value)) {}
		ReferenceExpected(const E& error) : mResult(error) {}
		ReferenceExpected(E&& error) : mResult(std::move(error)) {}

		ReferenceExpected(const ReferenceExpected& other) : mResult(other.mResult) {}
		ReferenceExpected(ReferenceExpected&& other) noexcept: mResult(std::move(other.mResult)) {}
		ReferenceExpected& operator=(const ReferenceExpected& other)
		{
			mResult = other.mResult;
			return *this;
		}
		ReferenceExpected& operator=(ReferenceExpected&& other)
		{
			mResult = std::move(other.mResult);
			return *this;
		}
		ReferenceExpected& operator=(Ty& value)
		{
			mResult = std::ref(value);
			return *this;
		}
		ReferenceExpected& operator=(const E& error)
		{
			mResult = error;
			return *this;
		}
		ReferenceExpected& operator=(E&& error)
		{
			mResult = std::move(error);
			return *this;
		}
		Ty& operator*() { return value(); }
		const Ty& operator*() const { return value(); }
		Ty& operator->() { return value(); }
		const Ty& operator->() const { return value(); }

		bool has_value() const { return std::holds_alternative<std::reference_wrapper<Ty>>(mResult); }
		bool has_error() const { return std::holds_alternative<E>(mResult); }

		Ty& value() { return std::get<std::reference_wrapper<Ty>>(mResult).get(); }
		const Ty& value() const { return std::get<std::reference_wrapper<Ty>>(mResult).get(); }

		E& error() { return std::get<E>(mResult); }
		const E& error() const { return std::get<E>(mResult); }

		operator bool() const { return has_value(); }
	private:
		std::variant<std::reference_wrapper<Ty>, E> mResult;
	};

}