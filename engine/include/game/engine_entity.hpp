//
// Created by flori on 3/23/2022.
//

#pragma once

#include "game/entity.hpp"
#include "renderer/model.hpp"
#include "maths.hpp"
#include "game/entity.hpp"

namespace Physics
{
    class Transform;
}

namespace Game
{
    class EngineEntity : public Entity
    {
    public:
        [[nodiscard]] const Renderer::Model& GetModel() const;

        Physics::Transform& GetTransform();

    private:
        Renderer::Model model;
    };
}
