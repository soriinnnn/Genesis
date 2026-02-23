#ifndef GENESIS_GAME_H
#define GENESIS_GAME_H
#include <core/Base.h>
#include <core/Core.h>
#include <math/Rect.h>

#define GENESIS_DEFAULT_WINDOW_WIDTH 1280
#define GENESIS_DEFAULT_WINDOW_HEIGHT 720

namespace genesis 
{
    struct GameDesc
    {
        Rect wndSize{GENESIS_DEFAULT_WINDOW_WIDTH, GENESIS_DEFAULT_WINDOW_HEIGHT};
        Logger::LogLevel logLevel = Logger::LogLevel::Error;
    };

    class Game: public Base 
    {
    public:
        explicit Game(const GameDesc& desc);
        virtual ~Game() override;

        virtual void run() final;

    private:
        void onInternalUpdate();

    private:
        std::unique_ptr<Logger> m_ownedLogger;
        std::unique_ptr<GraphicsEngine> m_graphicsEngine;
        std::unique_ptr<Display> m_display;
        bool m_isRunning;
    };
}

#endif