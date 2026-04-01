#ifndef GENESIS_GAME_H
#define GENESIS_GAME_H
#include <core/Core.h>
#include <core/Logger.h>
#include <core/utils/Macros.h>
#include <input/InputListener.h>
#include <math/Rect.h>

#include <game/TestWorld.h>

namespace genesis 
{
    struct GameDesc
    {
        const char* windowTitle;
        Rect windowSize{1280, 720};
        Logger::LogLevel logLevel = Logger::LogLevel::Error;
    };

    class Game: public InputListener
    {
    GENESIS_DISABLE_COPY_AND_MOVE(Game)
    public:
        explicit Game(const GameDesc& desc);
        virtual ~Game();

        Logger& getLogger() noexcept;
        virtual void run();

        void onKeyDown(Key key);
        void onKeyUp(Key key);
        void onMouseMove(Point delta, Point pos);
        void onMouseDown(MouseButton button, Point pos);
        void onMouseUp(MouseButton button, Point pos);

    private:
        void onInternalUpdate();
        float getDeltaTime();

    private:
        UniquePtr<Logger> m_logger;
        UniquePtr<GraphicsEngine> m_graphicsEngine;
        UniquePtr<Display> m_display;
        UniquePtr<ResourceManager> m_resourceManager;
        UniquePtr<InputManager> m_inputManager;

        bool m_isRunning;
        std::chrono::steady_clock::time_point m_previousTime;
        
        /* TEST */
        UniquePtr<TestWorld> m_testWorld;
        SharedPtr<PostProcess> m_effect;
        bool m_centerMouse;
    };
}

#endif