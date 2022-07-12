#include <random>

#include "world_data.hpp"
#include "level_selector.hpp"

KK_COMPONENT_IMPL_BEGIN(LevelSelector)

KK_COMPONENT_IMPL_END

void LevelSelector::OnEnable()
{
    std::random_device randDevice;
    std::mt19937_64 gen(randDevice());
    std::uniform_int_distribution<> sceneDistrib(0, scenePaths.size()-1);
    std::uniform_int_distribution<> tipDistrib(0, tips.size()-1);
    std::uniform_int_distribution<> forestDistrib(0, forestSceneScreens.size()-1);
    std::uniform_int_distribution<> snowDistrib(0, snowSceneScreens.size()-1);
    std::uniform_int_distribution<> desertDistrib(0, desertSceneScreens.size()-1);
    std::uniform_int_distribution<> darkDistrib(0, darkSceneScreens.size()-1);

    std::string scenePath = scenePaths[sceneDistrib(gen)];
    std::string loadingTexture;
    std::string tip = tips[tipDistrib(gen)];

    if (scenePath == forestScenePath)
        loadingTexture = forestSceneScreens[forestDistrib(gen)];// Set forest background
    else if (scenePath == snowScenePath)
        loadingTexture = snowSceneScreens[snowDistrib(gen)];// Set snow background
    else if (scenePath == desertScenePath)
        loadingTexture = desertSceneScreens[desertDistrib(gen)];// Set desert background
    else if (scenePath == darkScenePath)
        loadingTexture = darkSceneScreens[darkDistrib(gen)];// Set dark background

    World::LoadScene(scenePath, loadingTexture, tip, minimumLoadingTime);
}
