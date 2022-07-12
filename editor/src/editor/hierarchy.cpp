#include <imgui.h>
#include <string>

#include "drawable/static_draw.hpp"
#include "camera.hpp"
#include "collider/box_collider.hpp"
#include "collider/sphere_collider.hpp"
#include "ui/text.hpp"
#include "ui/button.hpp"
#include "game/entity_manager.hpp"
#include "resources/mesh.hpp"
#include "game/inputs_manager.hpp"
#include "helpers/game_edit.hpp"
#include "utils/dragdrop_constants.hpp"

#include "editor/hierarchy.hpp"

using namespace Editor;

inline std::string BuildEntityLabel(const std::string& name, const Game::EntityIdentifier& id)
{
    return name + "###" + std::to_string(id);
}

void Hierarchy::OnImGuiRender(Game::EntityManager& io_entityManager)
{
    ImGui::Begin("Hierarchy");

    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && selected) // Will edit name if double clicked
        editSelectedName = true;

    bool isWindowHovered = ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered();

    OpenPopupAndUnselect(isWindowHovered);

    // reset remove id
    removeID = -1;

    if (selected && selected->unsettingParent)
    {
        selected->unsettingParent = false;
        selected->parent = nullptr;
    }

    RenderEntitiesHierarchy(io_entityManager, io_entityManager.GetRootEntities());

    // Create a dummy item to enable dragDropTarget
    ImGui::InvisibleButton("InvisibleDropTarget", ImVec2(500, 500));
    ParentUnsetDragDropTarget(io_entityManager);
    OpenPopupAndUnselect(ImGui::IsItemHovered());

    ImGui::End();

    if(openAddEntityPopup)
    {
        ImGui::OpenPopup("ADDENTITY");
    }

    AddEntityPopup(io_entityManager);

    // Remove an entity if remove ID had been modify
    if (removeID != -1)
        io_entityManager.RemoveEntityAt(removeID);
}

void Hierarchy::RenderEntitiesHierarchy(Game::EntityManager& io_entityManager, const std::unordered_map<Game::EntityIdentifier, Game::Entity*>& io_entities)
{
    for (auto& pair : io_entities)
    {
        RenderEntity(io_entityManager, *pair.second);
    }
}

void Editor::Hierarchy::RenderEntitiesHierarchy(Game::EntityManager& io_entityManager, const std::list<Game::Entity*>& io_entities)
{
    for (Game::Entity* entity : io_entities)
    {
        RenderEntity(io_entityManager, *entity);
    }
}

void Hierarchy::RenderEntity(Game::EntityManager& io_entityManager, Game::Entity& io_entity)
{
    ImGui::PushID(io_entity.GetID());

    bool treeOpen = false;
    bool addEntityPopup = false;

    std::string label = BuildEntityLabel(io_entity.name, io_entity.GetID()); // '###' avoid entity with the same name to be multi-selected

    bool isSelectedEntity = label == selectedLabel;
    if (selected && editSelectedName && isSelectedEntity)
    {
        bool pressEnter = Helpers::Edit(selected->name, "Rename"); // Rename

        label = selectedLabel = BuildEntityLabel(selected->name, io_entity.GetID()); // Update label

        if (!ImGui::IsItemActive() || pressEnter) // Quit
            editSelectedName = false;
    }
    else
    {
        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
        if (isSelectedEntity) nodeFlags |= ImGuiTreeNodeFlags_Selected;

        treeOpen = ImGui::TreeNodeEx(label.c_str(), nodeFlags);

        SetupEntityInteraction(io_entityManager, io_entity, label);
    }

    ImGui::PopID();

    if (!io_entity.childs.empty())
    {
        if (treeOpen)
            RenderEntitiesHierarchy(io_entityManager, io_entity.childs);
    }

    if (treeOpen)
        ImGui::TreePop();
}

void Hierarchy::SetupEntityInteraction(Game::EntityManager& io_entityManager, Game::Entity& io_entity, std::string& io_label)
{
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemToggledOpen())
    {
        selected = &io_entity;
        selectedLabel = io_label;
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
    {
        selected = &io_entity;
        selectedLabel = io_label;
        openAddEntityPopup = true;
        selectedWhenAdd = selected;
    }

    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        ImGui::SetDragDropPayload(Utils::EntityDragDropID, selected, sizeof(Game::Entity*));
        ImGui::Text("Drop on entity to set parent");
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Utils::EntityDragDropID))
        {
            Game::Entity* child = static_cast<Game::Entity*>(payload->Data);
            if (child)
            {
                auto it = std::find(io_entity.childs.begin(), io_entity.childs.end(), selected);
                // Set parent only if selected is not already a child
                if (it == io_entity.childs.end())
                {
                    selected->parent = nullptr;
                    selected->parent = &io_entity;
                }
                else
                    selected->parent = nullptr;
            }
        }
        ImGui::EndDragDropTarget();
    }

}

void Hierarchy::ParentUnsetDragDropTarget(Game::EntityManager& io_entityManager) const
{
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Utils::EntityDragDropID))
        {
            Game::Entity* child = static_cast<Game::Entity*>(payload->Data);
            if (child)
                selected->parent = nullptr;
        }
        ImGui::EndDragDropTarget();
    }
}

void Hierarchy::AddEntityPopup(Game::EntityManager& io_entityManager)
{
    if (ImGui::BeginPopup("ADDENTITY", ImGuiWindowFlags_AlwaysAutoResize))
    {
        openAddEntityPopup = false;

        if (selectedWhenAdd && ImGui::Selectable("Delete"))
        {
            removeID = selectedWhenAdd->GetID();
            selected = selectedWhenAdd = nullptr;
        }

        ImGui::Separator();

        Game::Entity* entity = nullptr;
        if (ImGui::Selectable("Empty"))
        {
            entity = io_entityManager.CreateEntity();
        }
        else if (ImGui::Selectable("Cube mesh"))
        {
            entity = io_entityManager.CreateEntity();
            entity->name = "Cube_" + entity->GetStringID();
            auto drawable = entity->AddComponent<Game::StaticDraw>();
            drawable->SetMesh(Resources::Mesh::cubeMesh);
            drawable->SetTexture("game/assets/Textures/gold.jpg", true);
        }
        else if (ImGui::Selectable("Sphere mesh"))
        {
            entity = io_entityManager.CreateEntity();
            entity->name = "Sphere_" + entity->GetStringID();
            auto drawable = entity->AddComponent<Game::StaticDraw>();
            drawable->SetMesh(Resources::Mesh::sphereMesh);
            drawable->SetTexture("game/assets/Textures/gold.jpg", true);
        }
        else if (ImGui::Selectable("Box collider"))
        {
            entity = io_entityManager.CreateEntity();
            entity->name = "BoxCollider_" + entity->GetStringID();
            Game::Collider* collider = entity->AddComponent<Game::BoxCollider>();
            collider->SetStatic();
        }
        else if (ImGui::Selectable("Sphere collider"))
        {
            entity = io_entityManager.CreateEntity();
            entity->name = "SphereCollider_" + entity->GetStringID();
            Game::Collider* collider = entity->AddComponent<Game::SphereCollider>();
            collider->SetStatic();
        }
        else if (ImGui::Selectable("Camera"))
        {
            entity = io_entityManager.CreateEntity();
            entity->name = "Camera_" + entity->GetStringID();
            entity->AddComponent<Game::Camera>();
        }
        else if (ImGui::Selectable("UI Text"))
        {
            entity = io_entityManager.CreateEntity();
            entity->name = "Text_" + entity->GetStringID();
            auto text = entity->AddComponent<Game::Text>();
            text->position = {50.f, 50.f};
        }
        else if (ImGui::Selectable("UI Button"))
        {
            entity = io_entityManager.CreateEntity();
            entity->name = "Button_" + entity->GetStringID();
            auto button = entity->AddComponent<Game::Button>();
            button->scale = {100.f, 30.f};
            button->position = {50.f, 50.f};
        }

        if (entity && selectedWhenAdd)
        {
            entity->parent = selectedWhenAdd;
            selectedWhenAdd = nullptr;
        }

        selected = entity;

        ImGui::EndPopup();
    }
}

void Hierarchy::OpenPopupAndUnselect(bool i_condition)
{
    if (i_condition && Game::Inputs::IsPressed(Game::EButton::MOUSE_RIGHT))
    {
        openAddEntityPopup = true;
        selectedWhenAdd = selected = nullptr;
    }

    if (i_condition && Game::Inputs::IsPressed(Game::EButton::MOUSE_LEFT))
        selectedWhenAdd = selected = nullptr;
}