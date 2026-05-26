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
#include <entity/EntityManager.h>
#include <entity/Entity.h>
#include <ui/UIManager.h>
#include <math/Rect.h>

namespace genesis 
{
    struct GameDesc
    {
        const char* windowTitle;
        Rect windowSize{1280, 720};
        Logger::LogLevel logLevel = Logger::LogLevel::Error;
    };

    class Game
    {
    GENESIS_DISABLE_COPY_AND_MOVE(Game)
    public:
        explicit Game(const GameDesc& desc);
        virtual ~Game();

        Logger& getLogger() noexcept;
        GameContext getContext() noexcept;
        void run();

    protected:
        virtual void onCreate();
        virtual void onUpdate(float deltaTime);

        Entity* getMainCamera() const noexcept;
        Rect getRenderResolution() const noexcept;
        AntiAliasing getAntiAliasing() const noexcept;

        void setMainCamera(Entity* camera);
        void setRenderResolution(const Rect& resolution);
        void setAntiAliasing(AntiAliasing antiAliasing);
        void addEffect(SharedPtr<PostProcess> effect);
        void clearEffects();

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

    private:
        UniquePtr<Logger> m_logger;
        UniquePtr<GraphicsEngine> m_graphicsEngine;
        UniquePtr<Window> m_window;
        
    private:
        Vector<SharedPtr<PostProcess>> m_effects;
        TimePoint m_previousTime;
        Entity* m_mainCamera;
        bool m_isRunning;
    };
}

#endif