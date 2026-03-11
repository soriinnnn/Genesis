#include <game/Game.h>
#include <display/Display.h>
#include <input/InputManager.h>
#include <resources/ResourceManager.h>
#include <graphics/GraphicsEngine.h>
#include <resources/Texture.h>

#include <misc/PlatformUtils.h>

using namespace genesis;
using namespace std;

Game::Game(const GameDesc& desc)
{
    m_logger = make_unique<Logger>(desc.logLevel);
    m_graphicsEngine = make_unique<GraphicsEngine>(GraphicsEngineDesc{*m_logger});
    m_resourceManager = make_unique<ResourceManager>(ResourceManagerDesc{*m_logger, m_graphicsEngine->getGraphicsDevice()});
    m_display = make_unique<Display>(DisplayDesc{*m_logger, desc.wndSize, GENESIS_TEXT("Demo"), m_graphicsEngine->getGraphicsDevice()});
    m_inputManager = InputManager::create(InputManagerDesc{*m_logger, m_display->getWindow()});
    m_isRunning = true;

    m_graphicsEngine->setPipeline(*m_resourceManager);
    m_inputManager->addListener(m_graphicsEngine.get());
    m_inputManager->setMouseVisibility(false);
    m_tex = m_resourceManager->createTexture("demo/assets/textures/wood.jpg");
    m_resourceManager->unloadUnused();

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
    auto currentTime = chrono::steady_clock::now();
    chrono::duration<float> delta = currentTime - m_previousTime;
    m_previousTime = currentTime;
    float deltaTime = delta.count();

    // FPS temporal... ---
    static float timer = 0.0f;
    static int frames = 0;
    timer += deltaTime;
    frames++;
    if (timer >= 1.0f) {
        GENESIS_LOG_INFO("FPS: {} - Delta: {} ms", frames, (1000.0f / frames));
        frames = 0;
        timer -= 1.0f;
    }

    m_inputManager->update();

    Rect wndSize = m_display->getSize();
    m_inputManager->setMousePosition({wndSize.width() / 2, wndSize.height() / 2});

    m_graphicsEngine->render(*m_tex, m_display->getSwapChain(), deltaTime);
}
