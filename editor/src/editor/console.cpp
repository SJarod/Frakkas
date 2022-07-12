#include <fstream>
#include <Tracy.hpp>

#include "debug/log.hpp"
#include "utils/platform.hpp"

#include "editor/console.hpp"

using namespace Editor;

Console::~Console()
{
    ClearLog();
}

void Console::OnImGuiRender()
{
    ImGui::Begin("Console");

    DisplayLogList();

    if (ImGui::IsWindowAppearing())
        scrollToBottom = true;

    // clear console
    if (ImGui::Button("Clear"))
        ClearLog();
    ImGui::SameLine();

    // auto-scroll menu
    ImGui::Checkbox("Auto-scroll", &autoScroll);
    ImGui::SameLine();

    // Filter bar
    filter.Draw("Filter (\".h\") (\"error\")", 180);
    ImGui::Separator();

    // Reserve enough left-over height for 1 separator + 1 input text
    const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeightToReserve), false, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

    // Change log color
    for (int i = 0; i < items.Size; i++)
    {
        const char* item = items[i];
        if (!filter.PassFilter(item))
            continue;

        ImVec4 color;
        bool has_color = false;
        if (strstr(item, "[WARNING]"))
        {
            color = ImVec4(1.0f, 0.9f, 0.4f, 1.0f); // yellow
            has_color = true;
        }
        else if (strstr(item, "[ERROR]"))
        {
            color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); // red
            has_color = true;
        }
        else if (strstr(item, "[DEBUG]"))
        {
            color = ImVec4(0.1, 0.7f, 1.0f, 1.0f); // blue
            has_color = true;
        }
        if (has_color)
            ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextUnformatted(item);
        if (has_color)
            ImGui::PopStyleColor();
    }

    if (scrollToBottom || (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
        ImGui::SetScrollHereY(1.0f);

    scrollToBottom = false;

    ImGui::PopStyleVar();
    ImGui::EndChild();

    ImGui::End();
}

void Console::AddLog(const std::string& i_log)
{
#ifdef KK_WINDOWS
    items.push_back(_strdup(i_log.c_str()));
#else
    items.push_back(strdup(i_log.c_str()));
#endif

    if (autoScroll)
        scrollToBottom = true;
}

void Console::ClearLog()
{
    for (int i = 0; i < items.Size; i++)
        free(items[i]);
    items.clear();
}

void Console::DisplayLogList()
{
    std::vector<std::string>& listLogs = Log::GetLogList();

    if (!listLogs.empty())
    {
        for (int i = 0; i < listLogs.size(); i++)
            AddLog(listLogs[i]);

        listLogs.clear();
    }
}