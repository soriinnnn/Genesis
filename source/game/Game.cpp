#include <game/Game.h>
#include <display/Display.h>
#include <input/InputManager.h>
#include <graphics/GraphicsEngine.h>
#include <resources/ResourceManager.h>

using namespace genesis;
using namespace std;
using namespace chrono;

Game::Game(const GameDesc& desc)
{
    m_logger = make_unique<Logger>(desc.logLevel);
    m_graphicsEngine = make_unique<GraphicsEngine>(GraphicsEngineDesc{*m_logger});
    m_resourceManager = make_unique<ResourceManager>(ResourceManagerDesc{*m_logger, m_graphicsEngine->getGraphicsDevice()});
    m_display = make_unique<Display>(DisplayDesc{*m_logger, desc.wndSize, desc.wndTitle, WindowStyle::Windowed, m_graphicsEngine->getGraphicsDevice()});
    m_inputManager = InputManager::create(InputManagerDesc{*m_logger, m_display->getWindow()});
    m_isRunning = true;

    m_testWorld = make_unique<TestWorld>(WorldDesc{*m_logger, *m_inputManager, *m_resourceManager});
    m_centerMouse = false;
    m_inputManager->addListener(this);
    GENESIS_LOG_INFO("Game initialized.");
}

Game::~Game() 
{
    GENESIS_LOG_INFO("Game is shutting down...");
    m_inputManager->removeListener(this);
}

Logger& Game::getLogger() noexcept
{
    return *m_logger;
}

void Game::onInternalUpdate()
{
    float deltaTime = getDeltaTime();

    m_inputManager->update();
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

void Game::onKeyDown(Key key) {}

void Game::onKeyUp(Key key) 
{
    switch (key) {
    case Key::G:
    {
        m_centerMouse = !m_centerMouse;
        m_inputManager->setMouseLock(m_centerMouse);
        m_inputManager->setMouseVisibility(!m_centerMouse);
        break;
    }
    case Key::F11:
    {
        if (!m_display->isBorderless()) {
            m_display->toggleBorderless(1920, 1080);
        }
        else {
            m_display->toggleBorderless(1280, 720);
        }
        m_inputManager->ignoreNextMouseMove();
        break;
    }
    }
}

void Game::onMouseMove(Point delta, Point pos) {}

void Game::onMouseDown(MouseButton button, Point pos) {}

void Game::onMouseUp(MouseButton button, Point pos) {}