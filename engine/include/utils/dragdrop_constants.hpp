#pragma once

#include <string>

namespace Utils
{
    static constexpr char ResourcePathDragDropID[] = "RESOURCE";
    static constexpr char EntityDragDropID[] = "ENTITY";

    static constexpr char TextureExtensions[] = ".jpg .png";
    static constexpr char MeshExtensions[] = ".obj .fbx";
    static constexpr char SoundExtensions[] = ".mp3 .wav";
    static constexpr char AnimationExtensions[] = ".fbx";
    static constexpr char CodeExtensions[] = ".cpp .hpp";
    static constexpr char SceneExtensions[] = ".kk";

    /**
     * @brief Check if an extension is in an extensions list.
     * @param i_toFind The single extension to find.
     * @param i_extensions Multiple extensions in one string to list specific resource.
     * @return true if i_toFind found in i_extensions, else return false.
     */
    inline bool FindExtension(const std::string& i_toFind, const std::string& i_extensions)
    {
        return i_extensions.find(i_toFind) != std::string::npos;
    }
}