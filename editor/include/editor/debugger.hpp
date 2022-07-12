#pragma once

namespace Renderer
{
    class Graph;
}

namespace Editor
{
    /**
     * @brief Editor::Debugger is used to provide divers information or utils.
     * In other word, the debugger is the "tote everything"
     * where you can use ImGui without make a mess of the editor.
     */
    class Debugger
    {
    public:
        Debugger() = default;
        ~Debugger() = default;

        /**
         * @summary Display the imgui panel
         */
        void OnImGuiRender();

    private:
        float prevTime = 0.f;
        float deltaTime = 0.f;
        int FPS = 0;

        const float timeBeforeUpdate = 0.2f;
    };
}