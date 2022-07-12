#include <random>

#include "ui/button.hpp"
#include "drawable/animated_draw.hpp"
#include "world_data.hpp"

#include "level_selector.hpp"

#include "menu_script.hpp"

KK_COMPONENT_IMPL_BEGIN(MenuScript)

KK_COMPONENT_IMPL_END

void MenuScript::OnStart()
{
    ResetWorldData();

    World::SetInputsMode(World::InputsMode_UI);
    Time::SetTimeScale(1.f);

    // SOUNDS
    BGM1 = owner.get()->GetComponents<Sound>()[0];
    BGM2 = owner.get()->GetComponents<Sound>()[1];
    BGM3 = owner.get()->GetComponents<Sound>()[2];
    buttonSound = owner.get()->GetComponents<Sound>()[3];

    std::random_device randDevice;
    std::mt19937_64 gen(randDevice());
    std::uniform_int_distribution<> distribution(0, 2);

    if (distribution(gen) == 0)
        BGM = BGM1;
    else if (distribution(gen) == 1)
        BGM = BGM2;
    else
        BGM = BGM3;

    const auto& buttons = owner.get()->GetComponents<Button>();

    // Play
    buttons[0]->AddClickEvent([&]()
    {
        buttonSound->Play();
        World::SetInputsMode(World::InputsMode_Game);
        Time::SetTimeScale(1.f);
        // Load random scene (in OnEnable function)
        owner.get()->GetComponent<LevelSelector>()->enabled = true;
    });

    // Physic demo
    buttons[1]->AddClickEvent([&]()
    {
        buttonSound->Play();
        World::SetInputsMode(World::InputsMode_Game);
        World::LoadScene("game/assets/Scenes/physic.kk");
        Time::SetTimeScale(1.f);
    });

    // Quit
    buttons[2]->AddClickEvent([&]()
    {
        buttonSound->Play();
        Inputs::quit = true;
    });

    playerAnimation = GetEntityContainer().FindEntityWithComponent<AnimatedDraw>()->GetComponent<AnimatedDraw>();
    playerAnimation->animGraph.player.playSpeed = 0.5f;
}

void MenuScript::OnUpdate()
{
    BGM->Play();
    if (playerAnimation->animGraph.player.IsWaiting())
        playerAnimation->animGraph.PlayAnimation("Hero_Idle_2.fbx_mixamo.com", true);
}

void MenuScript::ResetWorldData()
{
    if (auto worldData = World::GetWorldData<FrakkarenaWorldData>())
    {
        // Reset worldData in case the player restart
        worldData->level = 0;
        worldData->score = worldData->comboScore = worldData->levelComboScore = 0;
        worldData->skeletonCount = worldData->golemCount = 0;
        worldData->damageLevel = worldData->damageSpeedLevel = worldData->lifeLevel = worldData->speedLevel = 0;
    }
}
