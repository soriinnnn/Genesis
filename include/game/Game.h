#ifndef GENESIS_GAME_H
#define GENESIS_GAME_H
#include <core/Core.h>
#include <core/Logger.h>
#include <core/utils/Macros.h>
#include <math/Rect.h>

namespace genesis 
{
    struct GameDesc
    {
        Rect wndSize{1280, 720};
        Logger::LogLevel logLevel = Logger::LogLevel::Error;
    };

    class Game
    {
    GENESIS_DISABLE_COPY_AND_MOVE(Game)
    public:
        explicit Game(const GameDesc& desc);
        virtual ~Game();

        Logger& getLogger() noexcept;
        virtual void run();

    private:
        void onInternalUpdate();

    private:
        UniquePtr<Logger> m_logger;
        UniquePtr<GraphicsEngine> m_graphicsEngine;
        UniquePtr<ResourceManager> m_resourceManager;
        UniquePtr<Display> m_display;
        UniquePtr<InputManager> m_inputManager;
        bool m_isRunning;

        std::chrono::steady_clock::time_point m_previousTime;
        SharedPtr<Texture> m_texture;
        SharedPtr<Mesh> m_mesh;
    };
}

#endif