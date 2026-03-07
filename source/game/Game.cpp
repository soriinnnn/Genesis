#include <game/Game.h>
#include <game/Display.h>
#include <window/Window.h>
#include <graphics/GraphicsEngine.h>
#include <input/InputManager.h>
#include <misc/PlatformUtils.h>

using namespace genesis;
using namespace std;

Game::Game(const GameDesc& desc)
{
    m_logger = make_unique<Logger>(desc.logLevel);
    m_graphicsEngine = make_unique<GraphicsEngine>(GraphicsEngineDesc{*m_logger});
    m_display = make_unique<Display>(
        DisplayDesc{
            WindowDesc{*m_logger, desc.wndSize, GENESIS_TEXT("Demo")}, 
            m_graphicsEngine->getGraphicsDevice()
        }
    );
    m_inputManager = InputManager::create(InputManagerDesc{*m_logger, m_display->getWindow()});
    m_isRunning = true;

    m_inputManager->addListener(m_graphicsEngine.get());

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
    m_graphicsEngine->render(m_display->getSwapChain(), deltaTime);
}
