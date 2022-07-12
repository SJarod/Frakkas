#include <map>

#include "game/lowcomponent/component.hpp"
#include "game/lowcomponent/drawable.hpp"
#include "sound.hpp"
#include "drawable/static_draw.hpp"
#include "camera.hpp"
#include "collider/box_collider.hpp"
#include "collider/sphere_collider.hpp"
#include "game/entity_manager.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"

#include "resources/resources_manager.hpp"
#include "resources/serializer.hpp"

#include "physic/physic_scene.hpp"

#include "helpers/path_constants.hpp"

#include "renderer/graph.hpp"

using namespace Game;
using namespace Renderer;

Physic::PhysicScene* Graph::physicScene = nullptr;
bool Graph::playing = false;

UI::Canvas Graph::canvas;
UI::Canvas Graph::loadingCanvas;

std::vector<Game::Camera*> Graph::gameCameras;
std::vector<Game::Drawable*> Graph::renderEntities;
std::vector<Game::Component*> Graph::componentsToStart;
std::vector<Game::Sound*> Graph::sounds;
bool Graph::updateCamera = true;

Graph::Graph(Game::EntityManager* io_entityManager, Physic::PhysicScene* i_physicScene, Renderer::LowLevel::LowRenderer* i_renderer)
    :entityManager(io_entityManager), editorCameraman(0, "Editor cameraman"), renderer(i_renderer)
{
    physicScene = i_physicScene;
    editorCameraman.transform.position = Vector3::forward * 4.f;
    editorCamera = editorCameraman.AddComponent<Game::Camera>();

    panel = std::make_unique<Panel>();
    text = std::make_unique<Text>();
}

void Graph::RegisterComponent(Game::Component* i_newComponent)
{
    std::string ID = i_newComponent->GetID();
    std::string parentID = i_newComponent->GetMetaData().parentClassName;
    if (ID == "StaticDraw" || ID == "AnimatedDraw")
        renderEntities.emplace_back(reinterpret_cast<Drawable*>(i_newComponent));
    else if (ID == Game::Camera::MetaData().className)
        gameCameras.emplace_back(reinterpret_cast<Game::Camera*>(i_newComponent));
    else if (ID == Game::Sound::MetaData().className)
        sounds.emplace_back(reinterpret_cast<Game::Sound*>(i_newComponent));
    else if (parentID == BoxCollider::MetaData().parentClassName)
    {
        auto collider = reinterpret_cast<Collider*>(i_newComponent);
        collider->GetTransform().colliderComponentCount++;
        physicScene->AddCollider(collider, Vector3(1.f, 1.f, 1.f));
    }
    else if (parentID == Text::MetaData().parentClassName)
    {
        auto ui = reinterpret_cast<UIObject*>(i_newComponent);
        canvas.AddUIObject(ui);
    }
}

void Graph::UnregisterComponent(Game::Component* i_oldComponent)
{
    std::string ID = i_oldComponent->GetID();
    std::string parentID = i_oldComponent->GetMetaData().parentClassName;

    if (ID == "StaticDraw" || ID == "AnimatedDraw")
    {
        auto it = std::find(renderEntities.begin(), renderEntities.end(), reinterpret_cast<Drawable*>(i_oldComponent));
        if (it != renderEntities.end())
            renderEntities.erase(it);
    }
    else if (ID == Game::Camera::MetaData().className)
    {
        auto it = std::find(gameCameras.begin(), gameCameras.end(), reinterpret_cast<Game::Camera*>(i_oldComponent));
        if (it != gameCameras.end())
            gameCameras.erase(it);
        updateCamera = true;
    }
    else if (ID == Game::Sound::MetaData().className)
    {
        auto it = std::find(sounds.begin(), sounds.end(), reinterpret_cast<Game::Sound*>(i_oldComponent));
        if (it != sounds.end())
        {
            (*it)->UnloadSound();
            sounds.erase(it);
        }
    }
    else if (parentID == BoxCollider::MetaData().parentClassName)
    {
        auto collider = reinterpret_cast<Collider*>(i_oldComponent);
        collider->GetTransform().colliderComponentCount--;
        physicScene->RemoveBody(collider->GetPhysicBodyID());
    }
    else if (parentID == Text::MetaData().parentClassName)
    {
        auto ui = reinterpret_cast<UIObject*>(i_oldComponent);
        canvas.RemoveUIObject(ui);
    }
}

void Graph::Clear()
{
    // Reset all lists
    gameCameras.clear();
    renderEntities.clear();
    physicScene->Clear();
    for (Sound* sound : sounds)
        sound->UnloadSound();
    sounds.clear();
    canvas.Clear();
    loadingCanvas.Clear();
}

void Graph::SetGameCameraAuto() noexcept
{
    updateCamera = false;
    for (Game::Camera* cameraComponent : gameCameras)
    {
        if (cameraComponent->enabled)
        {
            gameCamera = cameraComponent;
            return;
        }
    }

    gameCamera = nullptr;
}

void Graph::RenderEditor(Renderer::LowLevel::LowRenderer& i_renderer, float i_aspectRatio)
{
    UpdateGlobalUniform(i_renderer,
        i_aspectRatio,
        *editorCamera,
        editorCameraman.transform.position);
    Render(i_renderer, *editorCamera);
    RenderColliders(i_renderer);
}

void Graph::RenderGame(Renderer::LowLevel::LowRenderer& i_renderer, float i_aspectRatio)
{
    if (updateCamera || !gameCamera || !gameCamera->enabled)
    {
        SetGameCameraAuto();
        return;
    }

    UpdateGlobalUniform(i_renderer,
        i_aspectRatio,
        *gameCamera,
        gameCamera->owner.get()->transform.position);

    Render(i_renderer, *gameCamera);
}

void Graph::Render(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Camera& i_camera)
{
    // light depth map
    if (light.shadow)
    {
        float bias = static_cast<float>(light.shadowPCF) + 1.f;
        i_renderer.BeginFrame(*i_renderer.depthMapFBO, bias);

        for (Drawable* drawable : renderEntities)
        {
            if (drawable && drawable->enabled)
                drawable->DrawDepthMap(i_renderer, drawable->owner.get()->transform);
        }

        i_renderer.EndFrame();
    }

    // normal rendering
    i_renderer.BeginFrame(*i_renderer.firstPassFBO);

    std::multimap<float, Drawable*> sortedDrawable;

    // draw opaque objects
    for (Drawable* drawable : renderEntities)
    {
        if (drawable && drawable->enabled)
        {
            if (drawable->IsOpaque())
            {
                drawable->Draw(i_renderer, light, drawable->owner.get()->transform);
                continue;
            }

            Vector3 entityPos = drawable->owner.get()->transform.GetWorldMatrix().DecomposeTranslation();
            Vector3 cameraPos = i_camera.Position();
            float distance = Maths::Abs(Vector3(entityPos - cameraPos).SqrLength());

            sortedDrawable.insert({ distance, drawable });
        }
    }

    // draw transparent objects
    for (std::multimap<float, Drawable*>::reverse_iterator it = sortedDrawable.rbegin(); it != sortedDrawable.rend(); ++it)
    {
        Drawable* drawable = it->second;
        drawable->Draw(i_renderer, light, drawable->owner.get()->transform);
    }

    sortedDrawable.clear();

    i_renderer.EndFrame();

    // post processing
    i_renderer.BeginFrame(*i_renderer.secondPassFBO);

    i_renderer.RenderPostProcess();

    i_renderer.EndFrame();
}

void Graph::RenderColliders(Renderer::LowLevel::LowRenderer& i_renderer)
{
    if (!i_renderer.renderCollider)
        return;

    i_renderer.ContinueFrame(*i_renderer.firstPassFBO);

    for (Collider* collider : Physic::PhysicScene::colliders)
    {
        if (collider && collider->enabled)
            collider->DebugDraw(i_renderer, collider->owner.get()->transform);
    }

    i_renderer.EndFrame();
}

void Graph::UpdateGlobalUniform(const Renderer::LowLevel::LowRenderer& i_renderer, float i_aspectRatio, Game::Camera& i_camera, const Vector3& i_cameraPos) const noexcept
{
    // An entity with Camera should be added to render
    
    Matrix4 proj = i_camera.GetProjectionMatrix(i_aspectRatio);
    Matrix4 cascadeProj = i_camera.GetCascadeProjectionMatrix(i_aspectRatio, i_camera.near, i_renderer.shadowDistance);
    Matrix4 view = i_camera.GetViewMatrix();

    // projection
    i_renderer.SetUniformToNamedBlock("uRenderMatrices", 0, proj);
    // view
    i_renderer.SetUniformToNamedBlock("uRenderMatrices", 64, view);

    // Camera view frustum
    Matrix4 invProjView = (view * cascadeProj).Inverse();

    Vector4 corners[8];
    {
        int i = 0;
        for (int x = 0; x < 2; ++x)
            for (int y = 0; y < 2; ++y)
                for (int z = 0; z < 2; ++z)
                {
                    int index = i++;
                    corners[index] = invProjView * Vector4{ 2.f * x - 1.f, 2.f * y - 1.f, 2.f * z - 1.f, 1.f };
                    corners[index] = corners[index] / corners[index].w;
                }
    }

    Vector3 frustumCenter = { 0.f, 0.f, 0.f };
    for (int i = 0; i < 8; ++i)
        frustumCenter = frustumCenter + Vector3(corners[i]);
    frustumCenter = frustumCenter / 8.f;

    // Light view frustum
    Matrix4 lightView = Matrix4::LookAt(light.position + frustumCenter, frustumCenter, { 0.f, 1.f, 0.f });

    // Light view frustum AABB
    Vector3 aabbMin = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
    Vector3 aabbMax = { std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min() };
    for (int i = 0; i < 8; ++i)
    {
        Vector4 lightCorner = lightView * corners[i];
        aabbMin.x = Maths::Min(aabbMin.x, lightCorner.x);
        aabbMax.x = Maths::Max(aabbMax.x, lightCorner.x);
        aabbMin.y = Maths::Min(aabbMin.y, lightCorner.y);
        aabbMax.y = Maths::Max(aabbMax.y, lightCorner.y);
        aabbMin.z = Maths::Min(aabbMin.z, lightCorner.z);
        aabbMax.z = Maths::Max(aabbMax.z, lightCorner.z);
    }

    // lightProjection
    i_renderer.SetUniformToNamedBlock("uLightMatrices", 0, Matrix4::Orthographic(aabbMin.x, aabbMax.x,
        aabbMin.y, aabbMax.y,
        aabbMin.z, aabbMax.z));
    // lightView
    i_renderer.SetUniformToNamedBlock("uLightMatrices", 64, lightView);
    // cameraPos
    i_renderer.SetUniformToNamedBlock("uRendering", 112, i_cameraPos);

    // Use last light as default
    // light.enabled
    i_renderer.SetUniformToNamedBlock("uRendering", 0, lightEnabled);
    // light.position
    i_renderer.SetUniformToNamedBlock("uRendering", 16, light.position);
    // light.ambient
    i_renderer.SetUniformToNamedBlock("uRendering", 32, light.ambient);
    // light.diffuse
    i_renderer.SetUniformToNamedBlock("uRendering", 48, light.diffuse);
    // light.specular
    i_renderer.SetUniformToNamedBlock("uRendering", 64, light.specular);

    // toonShading
    i_renderer.SetUniformToNamedBlock("uRendering", 80, light.toonShading);
    // stepAmount
    i_renderer.SetUniformToNamedBlock("uRendering", 84, light.stepAmount);
    // stepSize
    i_renderer.SetUniformToNamedBlock("uRendering", 88, light.stepSize);
    // specSize
    i_renderer.SetUniformToNamedBlock("uRendering", 92, light.specSize);

    // shadow
    i_renderer.SetUniformToNamedBlock("uRendering", 96, light.shadow);
    // shadowPCF
    i_renderer.SetUniformToNamedBlock("uRendering", 100, light.shadowPCF);
}

bool Graph::CreateScene(std::filesystem::path i_scenePath)
{
    i_scenePath.make_preferred();

    std::ofstream emptyFile(i_scenePath); // CREATE BUTTON
    if (!emptyFile.is_open())
    {
        Log::Warning("Can't open scene file : ", i_scenePath);
        emptyFile.close();
        return false;
    }

    emptyFile.close();
    LoadScene(i_scenePath);

    /// CAMERA
    Game::Entity* en = entityManager->CreateEntity("Camera");
    en->AddComponent<Game::Camera>();

    /// GROUND
    en = entityManager->CreateEntity("Ground");
    en->AddComponent<Game::BoxCollider>();

    auto drawable = en->AddComponent<Game::StaticDraw>();
    drawable->SetMesh(Resources::Mesh::cubeMesh);
    drawable->SetTexture("white", false);

    en->transform.position = {0.f, -5.f, 0.f};
    en->transform.scale = {10.f, 1.f, 10.f};

    /// PLAYER
    en = entityManager->CreateEntity("Player");
    auto collider = en->AddComponent<Game::SphereCollider>();
    collider->SetDynamic();

    drawable = en->AddComponent<Game::StaticDraw>();
    drawable->SetMesh(Resources::Mesh::icoSphereMesh);
    drawable->SetTexture("red", false);

    SaveScene();

    return true;
}

void Graph::ReloadScene(const bool i_cleaning)
{
    LoadScene(currentScenePath, i_cleaning);
}

void Graph::SceneLoadFinished()
{
    if (ThreadPool::Clear())
    {
        ThreadPool::AddTask([this]() {
            if (minimumLoadTime > 0.f)
                ThreadPool::Delay(minimumLoadTime);

            loading = false;
            });
    }
    else
        ThreadPool::AddTask([this]() { SceneLoadFinished(); });
};

void Graph::LoadScene(const std::filesystem::path& i_scenePath, const bool i_cleaning)
{
    ThreadPool::FinishTasks();

    if (i_cleaning)
        ResourcesManager::DestroyResources();

    std::filesystem::path lastScene = currentScenePath;
    currentScenePath = i_scenePath;
    currentScenePath.make_preferred();

    std::ifstream file(currentScenePath);

    if (file.is_open())
    {
        Clear();

        panel->position = { 8.f, 70.f };
        panel->scale = { 640.f, 78.f };
        panel->tint = { 0.f, 0.f, 0.f, 0.5f };
        text->position = { 11.f, 75.f };
        text->scale = { 1.f, 1.f };
        text->tint = { 1.f, 1.f, 1.f, 1.f };
        text->text = tips;
        if (!tips.empty())
        {
            loadingCanvas.AddUIObject(panel.get());
            loadingCanvas.AddUIObject(text.get());
        }
        loadingCanvas.StartLoadingScreen(loadingScreenPath);
        ThreadPool::FinishTasks();

        // Read light information
        if (Serializer::GetAttribute(file) == "light")
            Serializer::Read(file, light);

        if (Serializer::GetAttribute(file) == "shadowDistance")
            Serializer::Read(file, &renderer->shadowDistance);

        // Parse file to create entities
        entityManager->LoadEntities(file);

        SetGameCameraAuto();

        Log::Info("Load scene ", currentScenePath);

        for (OnSceneLoadEvent& event : sceneLoadEvents)
            event();

        ThreadPool::AddTask([this]() {
            SceneLoadFinished();
            });
    }
    else
    {
        Log::Warning("Try to load scene from file '", currentScenePath, "' that does not exist.");
        currentScenePath = lastScene;
    }
}

void Graph::SetLoadingParameters(bool i_cleaning,
    const std::filesystem::path& i_loadScenePath,
    const std::filesystem::path& i_loadingScreenPath,
    const std::string& i_tips,
    float i_minimumLoadTime)
{
    if (loadScenePath.empty())
    {
        loadScenePath = i_loadScenePath.empty() ? currentScenePath : i_loadScenePath;
        loadingScreenPath = i_loadingScreenPath.empty() ? UI::Canvas::defaultLoadingScreen : i_loadingScreenPath;
        tips = i_tips;
        minimumLoadTime = i_minimumLoadTime;
        cleaning = i_cleaning;
    }
}

void Graph::ProcessLoading()
{
    if (!loadScenePath.empty())
    {
        loading = true;
        LoadScene(loadScenePath, cleaning);
        loadScenePath.clear();
        cleaning = true;
    }
}

void Graph::SaveScene() const
{
    std::ofstream file(currentScenePath);

    Serializer::Write(file, "light", light);

    Serializer::Write(file, "shadowDistance", &renderer->shadowDistance);

    for (const auto& pair : entityManager->GetRootEntities())
        Serializer::Write(file, *pair.second);

    Log::Info("Save scene ", currentScenePath);
}

std::filesystem::path Graph::GetSceneFullPath(const std::string& i_sceneName)
{
    return Helpers::gameDirectoryPath + std::string("assets/Scenes/") + i_sceneName + ".kk";
}