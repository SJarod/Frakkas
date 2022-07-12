#pragma once

#include <imgui.h>

#include "game/entity.hpp"
#include "game/entity_manager.hpp"
#include "sound.hpp"
#include "drawable/static_draw.hpp"

#include "renderer/graph.hpp"
#include "utils/dragdrop_constants.hpp"
#include "helpers/string_helpers.hpp"

namespace Helpers
{
    /**
     * @brief Read path extension to add a component to the output entity, or load a new scene.
     * @param i_path The input file path dropped.
     * @param o_entity The output entity to receive new component
     * @param io_graph The graph to handle .kk drop. Optional.
     * @return true if a resource has been load, false if nothing happens.
     */
    inline bool ReadPath(const std::filesystem::path& i_path, Game::Entity& o_entity, Renderer::Graph* io_graph = nullptr)
    {
        std::string extension = i_path.extension().string();
        if (Utils::FindExtension(extension, Utils::SoundExtensions))
        {
            auto soundComp = o_entity.AddComponent<Game::Sound>();
            soundComp->SetSoundPath(i_path);
            return true;;
        }
        else if (Utils::FindExtension(extension, Utils::MeshExtensions))
        {
            auto staticDraw = o_entity.AddComponent<Game::StaticDraw>();
            staticDraw->SetMesh(i_path.string());
            return true;
        }
        else if (Utils::FindExtension(extension, Utils::CodeExtensions))
        {
            std::filesystem::path fileName = i_path.filename().replace_extension();
            ClassMetaData* md = Game::Component::FindMetaData(Helpers::SnakeCaseToCamelCase(fileName.string()));
            if (md)
            {
                o_entity.AddComponent(std::unique_ptr<Game::Component>(md->constructor()));
                return true;
            }
        }
        else if (Utils::FindExtension(extension, Utils::SceneExtensions)) // Handle .KK Loading if graph is send
        {
            io_graph->SetLoadingParameters(true, i_path.string());
            return true;
        }

        return false;
    }

    /**
     * @brief Call ImGui "BeginDragDropTarget" to enable the last ImGui item as a resource path receiver.
     * @param o_entity The entity to add component if the data drop is a resource.
     * @param io_graph The graph to handle .kk drop. Optional.
     * @return true if a resource has been load, false if nothing happens.
     */
	inline bool DragDropTargetComponent(Game::Entity& o_entity, Renderer::Graph* io_graph = nullptr)
	{
        bool dropped = false;
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Utils::ResourcePathDragDropID))
            {
                std::filesystem::path path = *static_cast<std::filesystem::path*>(payload->Data);

                dropped = ReadPath(path, o_entity, io_graph);
            }
            ImGui::EndDragDropTarget();
        }
        return dropped;
	}

    /**
     * @brief Call ImGui "BeginDragDropTarget" to enable the last ImGui item as a resource path receiver.
     * @param o_entityManager The entity manager to add new entity with the component build by the resource dropped.
     * @param io_graph The graph to handle .kk drop. Optional.
     * @return true if a resource has been load, false if nothing happens.
     */
    inline bool DragDropTargetComponent(Game::EntityManager& o_entityManager, Renderer::Graph* io_graph = nullptr)
    {
        bool dropped = false;
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Utils::ResourcePathDragDropID))
            {
                std::filesystem::path path = *static_cast<std::filesystem::path*>(payload->Data);

                Game::Entity* entity = o_entityManager.CreateEntity();

                dropped = ReadPath(path, *entity, io_graph);

                if (!dropped)
                    o_entityManager.RemoveEntityAt(entity->GetID());
            }
            ImGui::EndDragDropTarget();
        }
        return dropped;
    }
}