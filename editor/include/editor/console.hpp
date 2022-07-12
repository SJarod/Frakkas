#pragma once

#include <imgui.h>

namespace Editor
{
    class Console
    {
    public:
        Console() = default;
        ~Console();

        /**
        * @summary Display the ImGui panel
        */
        void OnImGuiRender();

    private:
        ImVector<char*>       items;
        ImGuiTextFilter       filter;
        bool                  autoScroll = true;
        bool                  scrollToBottom = true;

        void AddLog(const std::string& i_log);
        void ClearLog();
        void DisplayLogList();
    };
}