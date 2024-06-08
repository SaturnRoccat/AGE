#pragma once 
#include <AgeAPI/internal/Types.hpp>
#include <vector>
#include <algorithm>

namespace AgeAPI::Backend
{
	class AState
	{
	public:
		AState() = default;
		AState(const Identifier& stateId) : mStateId(stateId) {}

		virtual ~AState() = default;

		Identifier GetStateId() const { return mStateId; }

		virtual void WriteState(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const = 0;
	protected:
		Identifier mStateId{};
	};

	class BoolState : public AState
	{
	public:
		BoolState() = default;
		BoolState(const Identifier& stateId) : AState(stateId) {}
		void WriteState(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override;
	};

	class IntState : public AState
	{
	public:
		IntState() = default;
		IntState(const Identifier& stateId, const std::vector<int>& values) : AState(stateId), mValues(values) {}
		IntState(const Identifier& stateId, int min, int max) : AState(stateId)
		{
			for (int i = min; i <= max; i++)
				mValues.push_back(i);
		}

		void AddValue(int value) { mValues.push_back(value); }

		void WriteState(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override;

		auto& GetValues() const { return mValues; }
	private:
		std::vector<int> mValues{};
	};

	class StringState : public AState
	{
	public:
		StringState() = default;
		StringState(const Identifier& stateId, const std::vector<std::string>& values) : AState(stateId), mValues(values) {}

		void AddValue(const std::string& value) { mValues.push_back(value); }

		void WriteState(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override;

		auto& GetValues() const { return mValues; }
	private:
		std::vector<std::string> mValues{};
	};



}