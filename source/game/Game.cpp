#include <game/Game.h>
#include <display/Display.h>
#include <input/InputManager.h>
#include <graphics/GraphicsEngine.h>
#include <resources/ResourceManager.h>
#include <resources/Texture.h>
#include <resources/Mesh.h>
#include <entity/EntityManager.h>

using namespace genesis;
using namespace std;
using namespace chrono;

Game::Game(const GameDesc& desc)
{
    m_logger = make_unique<Logger>(desc.logLevel);
    m_graphicsEngine = make_unique<GraphicsEngine>(GraphicsEngineDesc{*m_logger});
    m_resourceManager = make_unique<ResourceManager>(ResourceManagerDesc{*m_logger, m_graphicsEngine->getGraphicsDevice()});
    m_display = make_unique<Display>(DisplayDesc{*m_logger, desc.wndSize, desc.wndTitle, m_graphicsEngine->getGraphicsDevice()});
    m_inputManager = InputManager::create(InputManagerDesc{*m_logger, m_display->getWindow()});
    m_isRunning = true;

    m_testWorld = make_unique<TestWorld>(WorldDesc{*m_logger, *m_inputManager, *m_resourceManager});
    m_inputManager->setMouseVisibility(false);

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
    float deltaTime = getDeltaTime();

    m_inputManager->update();

    // Mouse center...
    auto size = m_display->getSize();
    m_inputManager->setMousePosition({size.width() / 2, size.height() / 2});

    m_testWorld->update(deltaTime);
    m_graphicsEngine->render(*m_testWorld, m_display->getSwapChain(), deltaTime);
}

float Game::getDeltaTime()
{
    auto currentTime = steady_clock::now();
    duration<float> delta = currentTime - m_previousTime;
    m_previousTime = currentTime;

    return delta.count();
}