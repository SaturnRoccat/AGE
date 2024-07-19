#pragma once
#include <string>
#include <functional>
#include <random>

namespace AgeAPI
{
	namespace detail
	{
		template<typename T>
		class EnableTReference
		{
		public:
			using ReferenceType = T;

			EnableTReference(std::function<T()> InitializeCallback) : mReferenceID(InitializeCallback()) {}

			EnableTReference(const EnableTReference& other) = delete; // Cant have two things with the same reference ID
			EnableTReference(EnableTReference&& other) noexcept : mReferenceID(std::move(other.mReferenceID)) {} // We can allow the move constructor
			EnableTReference& operator=(const EnableTReference& other) = delete; // Cant have two things with the same reference ID
			EnableTReference& operator=(EnableTReference&& other) noexcept { mReferenceID = std::move(other.mReferenceID); return *this; } // We can allow the move assignment operator


			const T& GetReference() const { return mReferenceID; }
			T& GetReference() { return mReferenceID; }
		protected:
			void SetReference(const T& reference) { mReferenceID = reference; }
			void SetReference(T&& reference) { mReferenceID = std::move(reference); }
		private:
			T mReferenceID;
		};
	}

	class EnableIntReference : public detail::EnableTReference<int>
	{
	public:
		EnableIntReference() : detail::EnableTReference<int>([]() {
			return std::random_device{}(); 
			}
		) {}
		EnableIntReference(int reference) : detail::EnableTReference<int>([reference]() {
			return reference;
			}
		) {}
	};

	class EnableStringReference : public detail::EnableTReference<std::string>
	{
	public:
		EnableStringReference() : detail::EnableTReference<std::string>([]() {
			return std::to_string(std::random_device{}());
		}
		) {}
		EnableStringReference(const std::string& reference) : detail::EnableTReference<std::string>([reference]() {
			return reference;
			}
		) {}
	};

	template<typename T>
	concept ReferenceEnabled = std::is_base_of_v<detail::EnableTReference<T>, T>;
	// Have the normal value type in the IsReferenceEnabled concept
	template<typename T>
	concept IsReferenceEnabled = ReferenceEnabled<T>;

}
