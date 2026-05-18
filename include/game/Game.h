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

        EntityId getMainCamera() const noexcept;
        Rect getRenderResolution() const noexcept;
        bool getVSync() const noexcept;

        void setMainCamera(EntityId camera);
        void setRenderResolution(uint32 width, uint32 height);
        void setVSync(bool enabled);

        void addEffect(SharedPtr<PostProcess> effect);
        void clearEffects();

    private:
        void onInternalUpdate();
        float getDeltaTime();

    protected:
        UniquePtr<Logger> m_logger;
        UniquePtr<Display> m_display;
        UniquePtr<PhysicsEngine> m_physicsEngine;
        UniquePtr<ResourceManager> m_resourceManager;
        UniquePtr<InputManager> m_inputManager;
        UniquePtr<UIManager> m_uiManager;
        UniquePtr<EntityManager> m_entityManager;
        
    private:
        UniquePtr<GraphicsEngine> m_graphicsEngine;
        Vector<SharedPtr<PostProcess>> m_effects;
        TimePoint m_previousTime;
        EntityId m_mainCamera;
        bool m_isRunning;
        bool m_vsync;
    };
}

#endif