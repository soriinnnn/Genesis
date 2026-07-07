#ifndef GENESIS_GAME_H
#define GENESIS_GAME_H
#include <core/Core.h>
#include <core/Logger.h>
#include <core/utils/Types.h>
#include <core/utils/Macros.h>
#include <display/Display.h>
#include <physics/PhysicsEngine.h>
#include <input/InputManager.h>
#include <resources/ResourceManager.h>
#include <script/ScriptManager.h>
#include <entity/EntityManager.h>
#include <entity/Entity.h>
#include <ui/UIManager.h>
#include <math/Rect.h>

GENESIS_NAMESPACE_BEGIN

struct GameDesc
{
    const char* windowTitle;
    const char* windowIcon{};
    Rect windowSize{1280, 720};
    Logger::LogLevel logLevel = Logger::LogLevel::Error;
};

class Game
{
GENESIS_DISABLE_COPY_AND_MOVE(Game)
public:
    virtual ~Game();

    Logger& getLogger() noexcept;
    GameContext getContext() noexcept;
    AntiAliasing getAntiAliasing() const noexcept;
    TextureFiltering getTextureFiltering() const noexcept;

    void setAntiAliasing(AntiAliasing antiAliasing);
    void setTextureFiltering(TextureFiltering filter);

    void run();
    void quit();

protected:
    explicit Game(const GameDesc& desc);

    virtual void onCreate();
    virtual void onUpdate(float deltaTime);

    Entity* getMainCamera() const noexcept;
    Rect getRenderResolution() const noexcept;

    void setMainCamera(Entity* camera);
    void setRenderResolution(const Rect& resolution);
    void setSkybox(SharedPtr<SkyBox> skybox);

private:
    void onInternalUpdate();
    float getDeltaTime();

protected:
    UniquePtr<Display> m_display;
    UniquePtr<PhysicsEngine> m_physicsEngine;
    UniquePtr<InputManager> m_inputManager;
    UniquePtr<ResourceManager> m_resourceManager;
    UniquePtr<EntityManager> m_entityManager;
    UniquePtr<UIManager> m_uiManager;
    UniquePtr<ScriptManager> m_scriptManager;
    Vector<SharedPtr<PostProcess>> m_effects;

private:
    UniquePtr<Logger> m_logger;
    UniquePtr<GraphicsEngine> m_graphicsEngine;
    UniquePtr<Window> m_window;

private:
    SharedPtr<SkyBox> m_skybox;
    TimePoint m_previousTime;
    EntityId m_mainCameraId;
    float m_accumulator;
    bool m_hasMainCamera;
    bool m_isRunning;
};

GENESIS_NAMESPACE_END

#endif