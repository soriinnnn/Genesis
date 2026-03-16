#include <game/Game.h>
#include <display/Display.h>
#include <input/InputManager.h>
#include <graphics/GraphicsEngine.h>
#include <resources/ResourceManager.h>
#include <resources/Texture.h>
#include <resources/Mesh.h>

#include <core/utils/PlatformUtils.h>

using namespace genesis;
using namespace std;
using namespace chrono;

Game::Game(const GameDesc& desc)
{
    m_logger = make_unique<Logger>(desc.logLevel);
    m_graphicsEngine = make_unique<GraphicsEngine>(GraphicsEngineDesc{*m_logger});
    m_resourceManager = make_unique<ResourceManager>(ResourceManagerDesc{*m_logger, m_graphicsEngine->getGraphicsDevice()});
    m_display = make_unique<Display>(DisplayDesc{*m_logger, desc.wndSize, GENESIS_TEXT("Demo"), m_graphicsEngine->getGraphicsDevice()});
    m_inputManager = InputManager::create(InputManagerDesc{*m_logger, m_display->getWindow()});
    m_isRunning = true;

    m_testWorld = make_unique<TestWorld>(WorldDesc{*m_logger, *m_inputManager, *m_resourceManager, *m_graphicsEngine, *m_display});

    GENESIS_LOG_INFO("Game initialized.");
}

Game::~Game() 
{
    GENESIS_LOG_INFO("Game is shutting down...");
}

Logger& Game::getLogger() noexcept
{
    return *m_logger;
}

void Game::onInternalUpdate()
{
    auto currentTime = steady_clock::now();
    duration<float> delta = currentTime - m_previousTime;
    float deltaTime = delta.count();

    m_previousTime = currentTime;
    m_inputManager->update();
  
    m_testWorld->update(deltaTime);
}
