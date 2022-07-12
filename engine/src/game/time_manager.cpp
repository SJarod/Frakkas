
#include <SDL.h>

#include "game/time_manager.hpp"

using namespace Game;

float Time::time = 0.f;
float Time::previousTime = 0.f;
float Time::deltaTime = 0.f;
float Time::timeScale = 1.f;

void Time::NewFrame()
{
    previousTime = time;
    time = SDL_GetTicks() / 1000.f;
    deltaTime = (time - previousTime);
}

float Time::GetTime()
{
    return SDL_GetTicks() / 1000.f;
}

float Time::GetDeltaTime()
{
    return deltaTime * timeScale;
}

float Time::GetRawDeltaTime()
{
    return deltaTime;
}

float Time::GetFixedDeltaTime()
{
    return fixedDeltaTime * timeScale;
}

float Time::GetRawFixedDeltaTime()
{
    return fixedDeltaTime;
}

float Time::GetTimeScale()
{
    return  timeScale;
}

void Time::SetTimeScale(float i_scale)
{
    timeScale = i_scale;
}
