#include <game/Game.h>
#include <graphics/GraphicsEngine.h>
#include <input/InputManager.h>

using namespace genesis;
using namespace std;
using namespace chrono;

Game::Game(const GameDesc& desc)
{
    m_logger = make_unique<Logger>(desc.logLevel);
    m_graphicsEngine = make_unique<GraphicsEngine>(GraphicsEngineDesc{*m_logger, desc.windowSize});
    m_resourceManager = make_unique<ResourceManager>(ResourceManagerDesc{*m_logger, m_graphicsEngine->getGraphicsContext()});
    m_display = make_unique<Display>(DisplayDesc{*m_logger, desc.windowSize, desc.windowTitle, WindowStyle::Windowed, m_graphicsEngine->getGraphicsContext()});
    m_inputManager = InputManager::create({*m_logger, m_display->getWindow()});
    m_uiManager = make_unique<UIManager>(UIManagerDesc{*m_logger});
    m_world = make_unique<World>(WorldDesc{*m_logger});
    m_isRunning = true;
    m_vsync = false;
    m_inputManager->addListener(m_uiManager.get());
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

GameContext Game::getContext() noexcept
{
    return {*m_world, *m_inputManager, *m_resourceManager, *m_uiManager};
}

void Game::onInternalUpdate()
{
    float deltaTime = getDeltaTime();
    m_inputManager->update();
    m_uiManager->update(deltaTime);

    onUpdate(deltaTime);
    m_world->update(deltaTime);

    m_graphicsEngine->clear();
    m_graphicsEngine->render(*m_world, deltaTime);
    for (auto& effect : m_effects) {
        m_graphicsEngine->postProcess(*effect);
    }
    m_graphicsEngine->render(*m_uiManager);
    m_graphicsEngine->present(m_display->getSwapChain(), m_vsync);
}

float Game::getDeltaTime()
{
    auto currentTime = steady_clock::now();
    duration<float> delta = currentTime - m_previousTime;
    m_previousTime = currentTime;
    return delta.count();
}

void Game::onCreate() {}

void Game::onUpdate(float deltaTime) {}

void Game::setImageResolution(uint32 width, uint32 height)
{
    m_graphicsEngine->resizeFrameBuffers(width, height);
}

void Game::addEffect(SharedPtr<PostProcess> effect)
{
    m_effects.push_back(effect);
}

void Game::clearEffects()
{
    m_effects.clear();
}
