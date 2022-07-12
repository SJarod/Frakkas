#include <random>

#include "portal.hpp"

#include "portal_manager.hpp"

KK_COMPONENT_IMPL_BEGIN(PortalManager)

    KK_FIELD_PUSH(PortalManager, activationTime, EDataType::FLOAT)
    KK_FIELD_PUSH(PortalManager, closeTime, EDataType::FLOAT)

KK_COMPONENT_IMPL_END

void PortalManager::OnStart()
{
    portalSound = owner.get()->GetComponent<Sound>();

    auto portals = GetEntityContainer().FindEntitiesWithComponent<Portal>();

    std::random_device randDevice;
    std::mt19937_64 gen(randDevice());
    std::uniform_int_distribution<> distribution(0, portals.size()-1);
    auto portal = portals[distribution(gen)]->GetComponent<Portal>();

    portalSound->positionAuto = false; // Prevent sound position change
    portalSound->SetSoundPosition(portal->Position());

    portal->ActivatePortal(activationTime, closeTime, portalSound);
}