//
// Created by m.mehalin on 23/02/2022.
//

#pragma once

#include <cmath>

namespace Maths
{
////////////////////////////// VARIABLES

    struct Constants {
        static constexpr float pi = 3.14159265358979323846264f;
        static constexpr float piHalf = pi / 2.f;
        static constexpr float epsilon = 1e-24f;
        static constexpr float weakEpsilon = 0.00001f; // Less precise but sufficient
    };
}
