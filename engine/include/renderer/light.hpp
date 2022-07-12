#pragma once

#include "maths.hpp"

namespace Renderer
{
    class Light
    {
    public:
        Light() = default;
        ~Light() = default;

        Vector4 position = { 1.f, 1.f, 1.f, 0.f }; // w = 0 for directionnal light
        Vector3 ambient = { 0.3f, 0.3f, 0.3f };
        Vector3 diffuse = { 1.f, 1.f, 1.f };
        Vector3 specular = { 0.5f, 0.5f, 0.5f };

        bool toonShading = true;

        //number of steps (light shade)
        int stepAmount = 5;
        float stepSize = 0.1;

        //specular size
        float specSize = 0.1;

        bool shadow = true;
        int shadowPCF = 0;
    };
}