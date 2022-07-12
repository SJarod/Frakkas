#pragma once

#include <string>
#include <memory>

class Engine;

namespace Game
{
    using InputsMode = unsigned int;

    // Inherits this structure to have your own world data
    struct WorldData
    {
        virtual ~WorldData() = default;
    };

    /**
     * @Summary World class is a kind of gate for entities to access Engine features as inputs listening or scene management.
     * It also has some default values as default scene.
     */
    class World
    {
    public:
        static constexpr InputsMode InputsMode_Game = 1 << 0;
        static constexpr InputsMode InputsMode_UI = 1 << 1;

        explicit World(Engine& i_engine);
        ~World() = default;


        std::string defaultScenePath = "game/assets/Scenes/menu.kk";

        /**
         * @brief Set or Replace the current world data structure.
         * @param i_worldData The world unique  pointer data that components will access.
         */
        static void SetWorldData(std::unique_ptr<WorldData> i_worldData);

        /**
         * @tparam TDataType The type of your own WorldData structure.
         * @return A pointer to world data. If no world data is set or if TDataType is wrong, return nullptr.
         */
         template<typename TDataType>
        static TDataType* GetWorldData();

        /**
         * @brief Setup the inputs mode to enable UI or Game
         * @param i_flag InputsMode flag to describe the current inputs mode.
         */
        static void SetInputsMode(InputsMode i_flag);

        static InputsMode GetInputsMode();

        /**
         * @brief Load a new scene.
         * @param i_scenePath The path to the scene asset.
         * @param i_loadingScreenPath The path to the loading screen to display.
         * @param i_tips The tips to display on the loading screen.
         * @param i_minimumLoadTime Delay the loading time to match the specified time.
         */
        static void LoadScene(const std::string& i_scenePath,
            const std::string& i_loadingScreenPath = "",
            const std::string& i_tips = "",
            float i_minimumLoadTime = 0.f);

        static void ReloadScene(const std::string& i_loadingScreenPath = "");

    private:
        static std::unique_ptr<WorldData> worldData;

        static InputsMode inputsMode;
        static Engine* engine;
    };
}

template<typename TDataType>
TDataType* Game::World::GetWorldData()
{
    if (TDataType* data = dynamic_cast<TDataType*>(worldData.get()))
        return data;
    else
        return nullptr;
}
