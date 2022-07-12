#include <imgui.h>

#include "maths.hpp"
#include "game/time_manager.hpp"
#include "editor/debugger.hpp"

using namespace Editor;

void Debugger::OnImGuiRender()
{
    ImGui::Begin("Debugger");

#pragma region Performance
    float newTime = Game::Time::GetTime();
    float newDeltaTime = Game::Time::GetDeltaTime();
    int newFPS = round(1.f / newDeltaTime);
    // Update frame performance value sometimes so that we can read the value correctly.
    if (newTime - prevTime >= timeBeforeUpdate)
    {
        prevTime = newTime;
        deltaTime = newDeltaTime;
        FPS = newFPS;
    }

    ImGui::Text("Time : %.2f", newTime);

    float timeScale = Game::Time::GetTimeScale();
    ImGui::InputFloat("Time scale", &timeScale, 0.1f, 0.2f, "%.2f");
    timeScale = Maths::Clamp(timeScale, 0.f, 3.f);
    Game::Time::SetTimeScale(timeScale);

    ImGui::Text("Fixed Delta time : %.4f", Game::Time::GetFixedDeltaTime());
    ImGui::SameLine();
    ImGui::Text("Delta time : %.4f", deltaTime);

    ImGui::Spacing();

    ImGui::Text("FPS : %i", FPS);
#pragma endregion

    ImGui::End();
}