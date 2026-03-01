#ifndef GENESIS_GAME_H
#define GENESIS_GAME_H
#include <core/Core.h>
#include <core/Logger.h>
#include <math/Rect.h>
#include <chrono>

#define GENESIS_DEFAULT_WINDOW_WIDTH 1280
#define GENESIS_DEFAULT_WINDOW_HEIGHT 720

namespace genesis 
{
    struct GameDesc
    {
        Rect wndSize{GENESIS_DEFAULT_WINDOW_WIDTH, GENESIS_DEFAULT_WINDOW_HEIGHT};
        Logger::LogLevel logLevel = Logger::LogLevel::Error;
    };

    class Game
    {
    GENESIS_DISABLE_COPY_AND_MOVE(Game)
    public:
        explicit Game(const GameDesc& desc);
        virtual ~Game();

        virtual Logger& getLogger() noexcept final;
        virtual void run() final;

    private:
        void onInternalUpdate();

    private:
        UniquePtr<Logger> m_logger;
        UniquePtr<GraphicsEngine> m_graphicsEngine;
        UniquePtr<Display> m_display;
        UniquePtr<InputManager> m_inputManager;
        bool m_isRunning;

        std::chrono::steady_clock::time_point m_previousTime;
    };
}

#endif