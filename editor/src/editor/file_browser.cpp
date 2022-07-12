#include <imgui.h>

#include "debug/log.hpp"

#include "game/inputs_manager.hpp"

#include "utils/dragdrop_constants.hpp"
#include "utils/normalize_filepath.hpp"

#include "editor/file_browser.hpp"

using namespace Editor;

extern const std::filesystem::path g_assetPath = "game/assets";

FileBrowser::FileBrowser() : currentDirectory(g_assetPath),
                             directoryIcon("editor/assets/DirectoryIcon.png", true),
                             fileIcon("editor/assets/FileIcon.png", true),
                             wavIcon("editor/assets/WAVicon.png", true),
                             mp3Icon("editor/assets/MP3icon.png", true),
                             objIcon("editor/assets/OBJicon.png", true),
                             fbxIcon("editor/assets/FBXicon.png", true),
                             gltfIcon("editor/assets/GLTFicon.png", true),
                             pngIcon("editor/assets/PNGicon.png", true),
                             jpgIcon("editor/assets/JPGicon.png", true),
                             kkIcon("editor/assets/KKicon.png", true),
                             cppIcon("editor/assets/CPPicon.png", true),
                             hppIcon("editor/assets/HPPicon.png", true)
{
    directoryIcon.CPULoad();
    fileIcon.CPULoad();
    wavIcon.CPULoad();
    mp3Icon.CPULoad();
    objIcon.CPULoad();
    fbxIcon.CPULoad();
    gltfIcon.CPULoad();
    pngIcon.CPULoad();
    jpgIcon.CPULoad();
    kkIcon.CPULoad();
    cppIcon.CPULoad();
    hppIcon.CPULoad();

    directoryIcon.GPULoad();
    fileIcon.GPULoad();
    wavIcon.GPULoad();
    mp3Icon.GPULoad();
    objIcon.GPULoad();
    fbxIcon.GPULoad();
    gltfIcon.GPULoad();
    pngIcon.GPULoad();
    jpgIcon.GPULoad();
    kkIcon.GPULoad();
    cppIcon.GPULoad();
    hppIcon.GPULoad();

    directoryIcon.CPUUnload();
    fileIcon.CPUUnload();
    wavIcon.CPUUnload();
    mp3Icon.CPUUnload();
    objIcon.CPUUnload();
    fbxIcon.CPUUnload();
    gltfIcon.CPUUnload();
    pngIcon.CPUUnload();
    jpgIcon.CPUUnload();
    kkIcon.CPUUnload();
    cppIcon.CPUUnload();
    hppIcon.CPUUnload();
}

FileBrowser::~FileBrowser()
{
    directoryIcon.GPUUnload();
    fileIcon.GPUUnload();
    wavIcon.GPUUnload();
    mp3Icon.GPUUnload();
    objIcon.GPUUnload();
    fbxIcon.GPUUnload();
    gltfIcon.GPUUnload();
    pngIcon.GPUUnload();
    jpgIcon.GPUUnload();
    kkIcon.GPUUnload();
    cppIcon.GPUUnload();
    hppIcon.GPUUnload();
}

void FileBrowser::OnImGuiRender()
{
	// Create assets directory if it doesn't exist
	if (!std::filesystem::is_directory(currentDirectory) || std::filesystem::exists(currentDirectory))
		std::filesystem::create_directory(currentDirectory);

	ImGui::Begin("File Browser");

	// Create back button to go up the assets directory hierarchy
	if (currentDirectory != std::filesystem::path(g_assetPath))
	{
		if (ImGui::Button("<- back"))
            currentDirectory = currentDirectory.parent_path();
	}

	// Change buttons size and padding
	static float padding = 16.f;
	static float thumbnailSize = 70.f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);

	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, nullptr, false);

    bool openDeletePopup = false;
	// Navigate through assets directory
	for (const auto& entry : std::filesystem::directory_iterator(currentDirectory))
    {
        std::filesystem::path path = entry.path();
        path.make_preferred();
        std::string filenameString = entry.path().filename().string();

        ImGui::PushID(filenameString.c_str());

        // Add button texture
        Resources::Texture& icon = ButtonIcon(entry);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

        if (icon.gpu)
        {
            ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon.gpu->data), {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});
            if(ImGui::IsItemHovered() && Game::Inputs::IsPressed(Game::EButton::MOUSE_RIGHT))
            {
                deletePath = entry.path();
                openDeletePopup = true;
            }
        }

        if (ImGui::BeginDragDropSource())
        {
            static std::filesystem::path itemPath;
            itemPath = Utils::NormalizeFilepath(entry.path().string());
            ImGui::SetDragDropPayload(Utils::ResourcePathDragDropID, &itemPath, sizeof(std::filesystem::path));
            ImGui::EndDragDropSource();
        }

        ImGui::PopStyleColor();

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            if (entry.is_directory())
                currentDirectory /= path.filename();
            else
            {
                std::string openPath = "\"" + path.string() + "\"";
                std::system(openPath.c_str());
            }
        }
        ImGui::TextWrapped("%s", filenameString.c_str());

        ImGui::NextColumn();

        ImGui::PopID();
	}

	ImGui::Columns(1);

	OptionsField(&padding, &thumbnailSize);

	ImGui::End();

    if (openDeletePopup)
        ImGui::OpenPopup("DELETE");

    if (ImGui::BeginPopup("DELETE", ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (ImGui::Selectable("Delete"))
        {
            if (!std::filesystem::remove(deletePath))
                Log::Error("Can't delete file/directory '", deletePath.string(), "'.");
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void FileBrowser::OptionsField(float* io_padding, float* io_thumbnailSize)
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Options"))
	{
		if (ImGui::BeginMenu("File Browser"))
		{
			ImGui::SliderFloat("Thumbnail Size", io_thumbnailSize, 50, 160);
			ImGui::SliderFloat("Padding", io_padding, 0, 64);

			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}

Resources::Texture& FileBrowser::ButtonIcon(const std::filesystem::directory_entry& entry)
{
    if (entry.is_directory())
        return directoryIcon;
    else if (entry.path().extension() == ".wav")
        return wavIcon;
    else if (entry.path().extension() == ".mp3")
        return mp3Icon;
    else if (entry.path().extension() == ".obj")
        return objIcon;
    else if (entry.path().extension() == ".fbx")
        return fbxIcon;
    else if (entry.path().extension() == ".gltf")
        return gltfIcon;
    else if (entry.path().extension() == ".png")
        return pngIcon;
    else if (entry.path().extension() == ".jpg")
        return jpgIcon;
    else if (entry.path().extension() == ".kk")
        return kkIcon;
    else if (entry.path().extension() == ".cpp")
        return cppIcon;
    else if (entry.path().extension() == ".hpp")
        return hppIcon;
    else
        return fileIcon;
}