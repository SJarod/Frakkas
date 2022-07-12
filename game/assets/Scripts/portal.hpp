#pragma once

#include "collider/box_collider.hpp"
#include "drawable/static_draw.hpp"
#include "sound.hpp"
#include "ui/text.hpp"

#include "component_generator.hpp"

namespace Game
{
    struct FrakkarenaWorldData;


    KK_COMPONENT(Portal);

    void ActivatePortal(float i_activationTime, float i_closeTime, Sound* io_openSound);

private:
    BoxCollider* triggerCollider = nullptr;
    StaticDraw* portalTex = nullptr;
    Text* uiText = nullptr;
    float activationTime = 15.f;
    float closeTime = 20.f; // Time until the portal close

    Sound* openSound = nullptr;

    void OnStart() override;

    void OnUpdate() override;

    void OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;

    void IncreaseRandomStatLevel(FrakkarenaWorldData* worldData, int count);


    KK_COMPONENT_END
}
