#pragma once

#include <string>

namespace Utils
{
    /**
     * @brief Replace '\' or '\\' by '/' to have Linux-norm filepath.
     * @param io_filepath The filepath to normalize.
     */
    static void NormalizeFilepath(std::string& io_filepath)
    {
        size_t pos = 0;
        while(pos != std::string::npos)
        {
            pos = io_filepath.find('\\', pos);
            if (pos != std::string::npos)
            {
                io_filepath.replace(pos, 1, "/");
                pos++;
            }
        }
        pos = 0;
        while(pos != std::string::npos)
        {
            pos = io_filepath.find("\\\\", pos);
            if (pos != std::string::npos)
            {
                io_filepath.replace(pos, 2, "/");
                pos += 2;
            }
        }
    }

    /**
     * @brief Replace '\' or '\\' by '/' to have Linux-norm filepath.
     * @param io_filepath The filepath to normalize.
     * @return The normalized result of io_filepath.
     */
    static std::string NormalizeFilepath(const std::string& io_filepath)
    {
        std::string path = io_filepath;
        NormalizeFilepath(path);
        return path;
    }
}