#include "ui/button.hpp"

#include "world_data.hpp"

#include "pause_trigger.hpp"

KK_COMPONENT_IMPL_BEGIN(PauseTrigger)

KK_COMPONENT_IMPL_END

void PauseTrigger::OnStart()
{
    const auto& buttons = owner.get()->GetComponents<Button>();
    buttons[0]->AddClickEvent([&]()
      {
          enabled = false;
      });
    buttons[1]->AddClickEvent([&]()
      {
        Time::SetTimeScale(1.f);
        World::LoadScene("game/assets/Scenes/menu.kk");
      });
}

void PauseTrigger::OnEnable()
{
    Time::SetTimeScale(0.f);
    World::SetInputsMode(World::InputsMode_UI | World::InputsMode_Game);

    for (auto& comp : owner.get()->components)
        if (comp->GetID() != GetID())
            comp->enabled = true;
}

void PauseTrigger::OnDisable()
{
    for (auto& comp : owner.get()->components)
        if (comp->GetID() != GetID())
            comp->enabled = false;

    Time::SetTimeScale(1.f);
    World::SetInputsMode(World::InputsMode_Game);
}
