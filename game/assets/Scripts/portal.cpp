#include <random>

#include "sound.hpp"
#include "world_data.hpp"
#include "player.hpp"
#include "level_selector.hpp"
#include "portal_manager.hpp"

#include "portal.hpp"

KK_COMPONENT_IMPL_BEGIN(Portal)

KK_COMPONENT_IMPL_END

void Portal::OnStart()
{
    portalTex = owner.get()->childs.back()->GetComponent<StaticDraw>();
    triggerCollider = owner.get()->GetComponent<BoxCollider>();
}

void Portal::ActivatePortal(float i_activationTime, float i_closeTime, Sound* io_openSound)
{
    // UI
    uiText = owner.get()->AddComponent<Text>();
    uiText->position = Vector2(40.f, 10.f);
    uiText->scale = Vector2(2.f, 2.f);
    uiText->enabled = false;

    activationTime = i_activationTime;
    closeTime = i_closeTime;
    openSound = io_openSound;
    enabled = true;
}

void Portal::OnUpdate()
{
    if (activationTime > 0.f)
    {
        activationTime -= Time::GetDeltaTime();

        uiText->text = std::to_string(activationTime);
    }
    else
    {
        openSound->Play();

        if (!triggerCollider->enabled)
        {
            triggerCollider->enabled = true;
            portalTex->enabled = true;
            uiText->enabled = true;
            uiText->tint = Vector4(1.f, 0.2f, 0.2f, 1.f);
            owner.get()->childs.front()->GetComponent<StaticDraw>()->model.material.tint = Vector4(0.f, 0.f, 1.f, 1.f);
        }

        closeTime -= Time::GetDeltaTime();
        uiText->text = std::to_string(closeTime);
        portalTex->model.material.tint = Vector4(0.f, 0.f, 0.f, (Maths::Abs(Maths::Cos(Time::GetTime() * 2.f)) + 1.f) / 2.f);

        if (closeTime <= 0.f) // Kill Player
            GetEntityContainer().FindEntityWithComponent<Player>()->GetComponent<Player>()->life->Hurt(100);
    }
}

void Portal::OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    if(auto player = i_otherCollider.owner.get()->GetRootEntity()->GetComponent<Player>())
    {
        if (auto worldData  = World::GetWorldData<FrakkarenaWorldData>())
        {

            worldData->playerLife = player->life->life;
            worldData->level++;
            worldData->comboScore += worldData->levelComboScore;
            worldData->levelComboScore = 0;
            IncreaseRandomStatLevel(worldData, 2);
        }

        // Load random scene (in OnEnable function)
        GetEntityContainer().FindEntityWithComponent<LevelSelector>()->GetComponent<LevelSelector>()->enabled = true;
    }
}

void Portal::IncreaseRandomStatLevel(FrakkarenaWorldData* worldData, int count)
{
    std::random_device randDevice;
    std::mt19937_64 gen(randDevice());
    std::discrete_distribution<> discreteDistri({1, 1, 1, 1});

    for (int i = 0; i < count; i++)
    {
        int rand = discreteDistri(gen);
        switch(rand)
        {
            case 0: worldData->lifeLevel++; break;
            case 1 : worldData->damageLevel++; break;
            case 2: worldData->speedLevel++; break;
            case 3: worldData->damageSpeedLevel++; break;
            default: break;
        }
    }
}
