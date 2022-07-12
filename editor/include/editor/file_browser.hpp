#pragma once

#include <filesystem>

#include "resources/texture.hpp"

namespace Editor
{
    class FileBrowser
    {
    public:
        FileBrowser();
        ~FileBrowser();

        /**
        * @summary Display the ImGui panel
        */
        void OnImGuiRender();

    private:
        std::filesystem::path currentDirectory;
        std::filesystem::path deletePath;

        Resources::Texture directoryIcon;
        Resources::Texture fileIcon;

        Resources::Texture wavIcon;
        Resources::Texture mp3Icon;
        Resources::Texture objIcon;
        Resources::Texture fbxIcon;
        Resources::Texture gltfIcon;
        Resources::Texture pngIcon;
        Resources::Texture jpgIcon;
        Resources::Texture kkIcon;
        Resources::Texture cppIcon;
        Resources::Texture hppIcon;

        /**
        * @summary Add "Options" field to Main menu bar to modify buttons size and padding
        * @param padding the space between each icon
        * @param thumbnailSize the icon's size
        */
        static void OptionsField(float* padding, float* thumbnailSize);

        /**
        * @summary Change button texture depending file extension
        */
        Resources::Texture& ButtonIcon(const std::filesystem::directory_entry& entry);
    };
}