#include "CLIOptions.hpp"

namespace AgeUtils
{
    HasError CLIManager::ParseArgs(int argc, char** argv)
    {
        for (int i = 1; i < argc; i++)
        {
			std::string_view arg = argv[i];
            if (arg.starts_with("--"))
            {
                arg.remove_prefix(2);
                if (mOptions.find(arg) == mOptions.end())
					return HasError::UnknownOption;
                auto& anyVal = this->mAnyVal[arg];
                auto argValue = mOptions[arg].get();
                if (argValue->mConsumesNextValue)
                {
					if (i + 1 >= argc)
                        return HasError::InvalidOptionValue;
					if (!argValue->StoreValue(argv[i + 1], anyVal))
                        return HasError::InvalidOptionValue;
					i++;
				}
                else
                    (void)argValue->StoreValue("", anyVal);

            }
		}
        for (auto& [key, option] : mOptions)
        {
			if (option->IsRequired() && mAnyVal.find(key) == mAnyVal.end())
				return HasError::MissingRequiredOption;
		}
		return HasError::NoError;
    }
}
