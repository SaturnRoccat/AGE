#include "Traits.hpp"
#include <AgeAPI/internal/Addon/Addon.hpp>
namespace AgeAPI::Backend::Bp
{
    ReferenceExpected<TraitBase, ErrorString> TraitBase::AddComponent(std::unique_ptr<Components::BlockComponentBase> component, bool override, NonOwningPtr<Addon> addon)
    {
        if (!addon) [[likely]]
            addon = Addon::GetStaticInstance();

		// FIXME: NO VERSION CHECKING HERE NEEDS TO BE ADDED
		auto it = mComponents.find(component->GetComponentID().GetFullNamespace());
		if (!override && it != mComponents.end() && !component->CanBeDoublePushed())
			return ErrorString("Component already exists and cannot be overridden");
		else if (it != mComponents.end() && component->CanBeDoublePushed())
		{
		}

    }
}
