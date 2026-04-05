#ifndef GENESIS_GAME_H
#define GENESIS_GAME_H
#include <core/Core.h>
#include <core/Logger.h>
#include <core/utils/Macros.h>
#include <display/Display.h>
#include <ui/UIManager.h>
#include <input/InputManager.h>
#include <resources/ResourceManager.h>
#include <game/World.h>
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
        World& getWorld() noexcept;
        InputManager& getInput() noexcept;
        ResourceManager& getResources() noexcept;
        UIManager& getUI() noexcept;
        void run();

    protected:
        virtual void onCreate();
        virtual void onUpdate(float deltaTime);

        void addEffect(SharedPtr<PostProcess> effect);
        void clearEffects();
        void setImageResolution(uint32 width, uint32 height);

    private:
        void onInternalUpdate();
        float getDeltaTime();

    protected:
        UniquePtr<Logger> m_logger;
        UniquePtr<Display> m_display;
        UniquePtr<InputManager> m_inputManager;
        UniquePtr<ResourceManager> m_resourceManager;
        UniquePtr<UIManager> m_uiManager;
        UniquePtr<World> m_world;
        
    private:
        bool m_isRunning;
        TimePoint m_previousTime;
        UniquePtr<GraphicsEngine> m_graphicsEngine;
        Vector<SharedPtr<PostProcess>> m_effects;
    };
}

#endif