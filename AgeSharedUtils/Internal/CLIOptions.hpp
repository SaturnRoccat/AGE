#pragma once
#include <string>
#include <optional>
#include <any>
#include <expected>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string_view>
#include <stdexcept>


namespace AgeUtils
{
	class CLIOption 
	{
	protected:
		std::string mOptionFlagName{ "" };
		std::string mOptionDescription{ "" };
		bool mRequired { false };
		bool mHasDefaultValue { false };
		bool mConsumesNextValue { true };
		friend class CLIManager;
	public:
		CLIOption() = default;
		CLIOption(
			std::string optionFlagName,
			std::string optionDescription,
			bool required = false,
			bool hasDefaultValue = false,
			bool consumesNextValue = true
		) : mOptionFlagName(optionFlagName), mOptionDescription(optionDescription), mRequired(required), mHasDefaultValue(hasDefaultValue), mConsumesNextValue(consumesNextValue) {}

		virtual bool StoreValue(std::string_view value, std::any& locationStore) = 0;
		std::string_view ViewName () const { return mOptionFlagName; }
		std::string GetOptionFlagName() const { return mOptionFlagName; }
		std::string GetOptionDescription() const { return mOptionDescription; }
		bool IsRequired() const { return mRequired; }
		bool HasDefaultValue() const { return mHasDefaultValue; }
	};

	class StringOption : public CLIOption
	{
	private:
		std::string mValue{};
	public:
		StringOption() = default;
		StringOption(
			std::string optionFlagName,
			std::string optionDescription,
			bool required = false,
			std::string defaultValue = "",
			bool hasDefaultValue = false
		) : CLIOption(optionFlagName, optionDescription, required, hasDefaultValue), mValue(defaultValue) {}

		inline bool StoreValue(std::string_view value, std::any& locationStore) override
		{
			if (value.empty())
				if (mRequired)
					return false;
				else
					value = mValue;

			mValue = value;
			locationStore = mValue;
			return true;
		}
		std::string GetValue() const { return mValue; }
	};

	class IntOption : public CLIOption
	{
	private:
		int mValue{};
	public:
		IntOption() = default;
		IntOption(
				std::string optionFlagName,
				std::string optionDescription,
				bool required = false,
				int defaultValue = 0,
				bool hasDefaultValue = false
			) : CLIOption(optionFlagName, optionDescription, required, hasDefaultValue), mValue(defaultValue) {}

		inline bool StoreValue(std::string_view value, std::any& locationStore) override
		{
			if (value.empty())
				if (mRequired)
					return false;
				else
					value = std::to_string(mValue);

			try
			{
				mValue = std::stoi(std::string(value));
				locationStore = mValue;
			}
			catch (std::invalid_argument& e)
			{
				return false;
			}
			catch (std::out_of_range& e)
			{
				return false;
			}
			return true;
		}
		int GetValue() const { return mValue; }
	};

	class BoolOption : public CLIOption
	{
	private:
		bool mValue{};
	public:
BoolOption() = default;
		BoolOption(
			std::string optionFlagName,
			std::string optionDescription,
			bool required = false,
			bool defaultValue = false,
			bool hasDefaultValue = false
		) : CLIOption(optionFlagName, optionDescription, required, hasDefaultValue), mValue(defaultValue) {}

		inline bool StoreValue(std::string_view value, std::any& locationStore) override
		{
			if (value.empty())
				if (mRequired)
					return false;
				else
					value = mValue ? "true" : "false";

			if (value == "true")
				mValue = true;
			else if (value == "false")
				mValue = false;
			else if (value == "1")
				mValue = true;
			else if (value == "0")
				mValue = false;
			else
				return false;
			locationStore = mValue;
			return true;
		}
		bool GetValue() const { return mValue; }
	};

	class FloatOption : public CLIOption
	{
	private:
		float mValue{};
	public:
FloatOption() = default;
		FloatOption(
			std::string optionFlagName,
			std::string optionDescription,
			bool required = false,
			float defaultValue = 0.0f,
			bool hasDefaultValue = false
		) : CLIOption(optionFlagName, optionDescription, required, hasDefaultValue), mValue(defaultValue) {}

		inline bool StoreValue(std::string_view value, std::any& locationStore) override
		{
			if (value.empty())
				if (mRequired)
					return false;
				else
					value = std::to_string(mValue);

			try
			{
				mValue = std::stof(std::string(value));
			}
			catch (std::invalid_argument& e)
			{
				return false;
			}
			catch (std::out_of_range& e)
			{
				return false;
			}
			locationStore = mValue;
			return true;
		}
		float GetValue() const { return mValue; }
	};

	class FlagOption : public CLIOption
	{
	private:
		bool mValue{};
	public:
		FlagOption() = default;
		FlagOption(
			std::string optionFlagName,
			std::string optionDescription,
			bool required = false,
			bool defaultValue = false,
			bool hasDefaultValue = false
		) : CLIOption(optionFlagName, optionDescription, required, hasDefaultValue, false), mValue(defaultValue) {}

		inline bool StoreValue(std::string_view value, std::any& locationStore) override
		{
			mValue = true;
			locationStore = mValue;
			return true;
		}
		bool GetValue() const { return mValue; }
	};

	enum HasError
	{
		NoError,
		ArgAlreadyExists,
		MissingRequiredOption,
		InvalidOptionValue,
		UnknownOption,

	};

	class CLIManager
	{
	public:
		CLIManager() = default;
		CLIManager(std::vector<std::unique_ptr<CLIOption>> options)
		{
			for (auto& option : options)
				mOptions[option->GetOptionFlagName()] = std::move(option);
		}
		HasError AddOption(std::unique_ptr<CLIOption> option) 
		{ 
			auto flagName = option->ViewName();
			if (mOptions.find(flagName) != mOptions.end())
				return HasError::ArgAlreadyExists;
			mOptions[flagName] = std::move(option);
			return HasError::NoError;
		}

		HasError ParseArgs(int argc, char** argv);
		template<typename T>
		std::expected<T, HasError> GetValue(std::string_view optionName)
		{
			if (mAnyVal.find(optionName) == mAnyVal.end())
				return std::unexpected(HasError::UnknownOption);
			return std::any_cast<T>(mAnyVal[optionName]);
		}
	private:
		std::unordered_map<std::string_view, std::unique_ptr<CLIOption>> mOptions{};
		std::unordered_map<std::string_view, std::any> mAnyVal{};
	};
}
