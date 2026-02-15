#ifndef GENESIS_GAME_H
#define GENESIS_GAME_H
#include <core/Base.h>
#include <core/Core.h>

namespace genesis 
{
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