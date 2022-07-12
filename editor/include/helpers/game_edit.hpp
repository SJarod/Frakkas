#pragma once

#include <string>
#include <unordered_map>

#include "engine.hpp"
#include "debug/log.hpp"
#include "utils/string_format.hpp"

#include "game/inputs_manager.hpp"
#include "game/entity.hpp"
#include "game/reflection/data_descriptor.hpp"

#include "editor/editor_render.hpp"

namespace Game
{
	struct Transform;
}

namespace Renderer
{
    class Light;
    class Model;
    class SkeletalModel;
}

namespace Resources
{
    class Sound;
    class SkeletonNodeData;
}

namespace Animation
{
    class AnimationGraph;
}

struct ClassMetaData;

namespace Helpers
{
     /**
     * @brief Call ImGui::DragScalarN and activate cursor game mode for comfortability.
     * @tparam TScalarType Type of the scalar : int or float
     * @param i_label The variable name to print on editor.
     * @param io_data The pointer to the scalar array.
     * @param i_count The number of scalar in the array.
     * @param i_min The minimum value to clamp at.
     * @param i_max The maximum value to clamp at.
     * @param i_speed The quantity step that changes the scalar value when dragging.
     * @return true if user edit the scalar, false if nothing happened.
     */
     template<typename TScalarType> requires std::is_scalar_v<TScalarType>
    bool DragScalar(const std::string_view& i_label, TScalarType* io_data, int i_count, TScalarType i_min = std::numeric_limits<TScalarType>::lowest(), TScalarType i_max = std::numeric_limits<TScalarType>::max(), float i_speed = 0.1f)
    {
        ImGuiDataType_ dataType = ImGuiDataType_Float;
        if constexpr (std::is_same_v<TScalarType, int>)
            dataType = ImGuiDataType_S32;
        else if constexpr (std::is_floating_point_v<TScalarType>)
            dataType = ImGuiDataType_Float;
        else
        {
                Log::Warning("Intended to drag-edit a wrong type.");
                return false;
        }

        if (ImGui::DragScalarN(i_label.data(), dataType, io_data, i_count, i_speed, &i_min, &i_max))
        {
            if (!Editor::EditorRender::editingDrag)
            {
                Engine::SetCursorGameMode(true);
                Editor::EditorRender::editingDrag = true;
                Editor::EditorRender::mouseLockPosition = Game::Inputs::GetMousePosition();
            }
            return true;
        }
        return false;
    }

    /**
     * @brief Call ImGui::DragScalarN and activate cursor game mode for comfortability.
     * @tparam TScalarType Type of the scalar : int or float
     * @param i_label The variable name to print on editor.
     * @param io_data The pointer to the scalar array.
     * @param i_count The number of scalar in the array.
     * @param i_range Prevent the scalar data to be out of range. i_range.x is minimum, i_range.y is maximum
     * @return true if user edit the scalar, false if nothing happened.
     */
    template<typename TScalarType> requires std::is_scalar_v<TScalarType>
    bool DragScalar(const std::string_view& i_label, TScalarType* io_data, int i_count, const Vector2& i_range)
    {
        // Apply range only if there is a difference
        if (i_range.x < i_range.y)
        {
            auto min = static_cast<TScalarType>(i_range.x);
            auto max = static_cast<TScalarType>(i_range.y);
            return DragScalar(i_label, io_data, i_count,min, max);
        }
        else
            return DragScalar(i_label, io_data, i_count);
    }

	/**
    * @brief ImGui editing function. Allows the player to edit a string variable.
     * @param io_string The string to display and edit.
     * @param i_label The variable name of the string. Called 'label' for ImGui field.
     * @return true if the user press ENTER while editing text, else return false
     */

    bool Edit(std::string& io_string, const char* i_label);
    /**
     * @brief ImGui editing function. Allows the player to change transform parameters.
     * @param io_transform the transform to edit
     */
    void Edit(Game::Transform& io_transform);

    /**
     * @brief ImGui editing function. Show available animations in the graph
     * @param io_animGraph the animation graph
     */
    void Edit(Animation::AnimationGraph& io_animGraph);

    /**
     * @brief Display the entire skeleton with ImGui.
     */
    void DisplaySkeleton(Renderer::SkeletalModel& io_skmodel);

    /**
     * @brief ImGui editing function. Show skeleton bone hierarchy.
     * @param io_skmodel the skeletal model
     */
    void Edit(Renderer::SkeletalModel& io_skmodel);

    /**
     * @brief ImGui editing function. Allows the player to change entity parameters.
     * @param io_entity the entity to edit
     * @param i_guizmoOperation A int used by ImGui to defined which guizmo utility is being used.
     * 0 - none / 1 - translate / 2 - rotate / 3 - scale
     */
    void Edit(Game::Entity& io_entity, ImGuizmo::OPERATION& i_guizmoOperation);

    /**
     * @brief ImGui editing function. Allows the player to change light parameters.
     * @param io_engine the engine containing the light
     * @param o_showMap should the shadow map be displayed ?
     */
    void Edit(Engine& io_engine, bool& o_showMap);

    /**
     * @brief Get a string from a pointer of scalars to render it with ImGui::Text()
     * @param i_fieldName Name of the field to begin the text.
     * @param i_data The data to convert into string.
     * @param i_scalarCount The number of scalar to print from the data pointer
     * @return The data converted in string.
     */
    template<typename TScalarType> requires std::is_scalar_v<TScalarType>
    std::string ScalarDataToString(std::string i_fieldName, unsigned char* i_data, int i_scalarCount)
    {
        std::string fmt = i_fieldName + " : ";
        auto vect = reinterpret_cast<TScalarType*>(i_data);
        for (int i = 0; i < i_scalarCount; ++i)
            fmt = StringFormat::GetFormat(fmt, vect[i], ", ");
        return fmt;
    }

    /**
     * @brief ImGui editing function. Edit a component.
     * @param io_component The component itself, cast in an unsigned char* pointer for simple memory work.
     * @param io_metaData The metaData of the component, which means all the reflected information to use io_component properly.
     * @return false if the component should be removed.
     */
    bool Edit(unsigned char* io_component, const ClassMetaData& io_metaData, bool& io_enabled);

    /**
     * @brief ImGui editing function. Edit all the component's field.
     * @param io_component The component itself, cast in an unsigned char* pointer for simple memory work.
     * @param io_metaData The class meta data to get access to component field properly.
     */
    void Edit(unsigned char* io_component, const ClassMetaData& io_metaData);
}