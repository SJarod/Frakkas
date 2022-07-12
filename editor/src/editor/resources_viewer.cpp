#include <Tracy.hpp>

#include "resources/resources_manager.hpp"

#include "editor/resources_viewer.hpp"

static void ToByteSizeStr(char* o_buffer, const size_t i_size)
{
	static const char* unitsStr[] = { "--", "B", "KB", "MB", "GB", "TB", "PT", "ET" };
	int unitIndex = 1;

	float showedSize = static_cast<float>(i_size);
	if (showedSize == 0.f)
	{
		sprintf(o_buffer, "%s", unitsStr[0]);
		return;
	}
	while (showedSize > 1024.f)
	{
		showedSize /= 1024.f;
		++unitIndex;
	}

	sprintf(o_buffer, "%.2f %s", showedSize, unitsStr[unitIndex]);
}

void Editor::ResourcesViewer::OnImGuiRender()
{
	const std::unordered_map<std::string, std::shared_ptr<Resource>>& resources = ResourcesManager::ViewAllResources();
	const int resourcesCount = resources.size();
	static const char* typesStr[] = { "undefined", "shader", "mesh", "skeletal mesh", "texture", "animation pack", "audio" };

	ImGui::Begin("Resources Viewer");

	ImGuiTableFlags features = ImGuiTableFlags_Resizable;
	ImGuiTableFlags decorations = 0;
	decorations |= ImGuiTableFlags_RowBg;
	decorations |= ImGuiTableFlags_BordersV;
	decorations |= ImGuiTableFlags_BordersH;
	ImGuiTableFlags scrolling = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY;

	size_t ramAll = 0;
	size_t vramAll = 0;
	char buffer[255];
	if (ImGui::BeginTable("Resources", 4, features | decorations | scrolling))
	{
		ImGui::TableSetupScrollFreeze(4, 1);

		ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
		ImGui::TableNextColumn();
		ImGui::Text("Name");
		ImGui::TableNextColumn();
		ImGui::Text("Type");
		ImGui::TableNextColumn();
		ImGui::Text("RAM");
		ImGui::TableNextColumn();
		ImGui::Text("VRAM");

		for (auto& res : resources)
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("%s", res.first.c_str());
			ImGui::TableNextColumn();
			ImGui::Text("%s", typesStr[static_cast<int>(res.second->resourceType)]);
			ImGui::TableNextColumn();
			ToByteSizeStr(buffer, res.second->ram);
			ImGui::Text("%s", buffer);
			ImGui::TableNextColumn();
			ToByteSizeStr(buffer, res.second->vram);
			ImGui::Text("%s", buffer);

			ramAll += res.second->ram;
			vramAll += res.second->vram;
		}

		ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
		ImGui::TableNextColumn();
		ImGui::Text("%d resources", resourcesCount);
		ImGui::TableNextColumn();
		ImGui::Text("--");
		ImGui::TableNextColumn();
		ToByteSizeStr(buffer, ramAll);
		ImGui::Text("%s", buffer);
		ImGui::TableNextColumn();
		ToByteSizeStr(buffer, vramAll);
		ImGui::Text("%s", buffer);

		ImGui::EndTable();
	}

	ImGui::End();
}