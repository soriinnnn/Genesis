#include <core/Logger.h>
#include <game/Game.h>
#include <game/Display.h>
#include <window/Window.h>
#include <graphics/GraphicsEngine.h>
#include <core/PlatformUtils.h>

using namespace genesis;
using namespace std;

Game::Game(const GameDesc& desc): Base({*make_unique<Logger>(desc.logLevel).release()}), m_ownedLogger(&m_logger)
{
    BaseDesc base{m_logger};
    WindowDesc window{base, desc.windowSize, GENESIS_TEXT("Demo")};

    m_graphicsEngine = make_unique<GraphicsEngine>(GraphicsEngineDesc{m_logger});
    m_display = make_unique<Display>(DisplayDesc{window, m_graphicsEngine->getGraphicsDevice()});
    m_isRunning = true;

    GENESIS_LOG_INFO("Game initialized.");
}

Game::~Game() 
{
    GENESIS_LOG_INFO("Game is shutting down...");
}

void Game::onInternalUpdate()
{
    m_graphicsEngine->render(m_display->getSwapChain());
}
