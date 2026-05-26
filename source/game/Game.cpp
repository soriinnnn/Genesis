#include <game/Game.h>
#include <graphics/GraphicsEngine.h>
#include <input/InputManager.h>
#include <entity/EntityManager.h>
#include <entity/Entity.h>
#include <entity/components/CameraComponent.h>

using namespace genesis;
using namespace std;
using namespace chrono;

Game::Game(const GameDesc& desc)
{
    m_logger = make_unique<Logger>(desc.logLevel);
    m_window = Window::create({getLogger(), desc.windowSize, desc.windowTitle});
    m_graphicsEngine = make_unique<GraphicsEngine>(GraphicsEngineDesc{getLogger(), desc.windowSize});
    m_physicsEngine = make_unique<PhysicsEngine>(PhysicsEngineDesc{getLogger(), m_graphicsEngine->getGraphicsContext()});
    m_resourceManager = make_unique<ResourceManager>(ResourceManagerDesc{getLogger(), m_graphicsEngine->getGraphicsContext()});
    m_display = make_unique<Display>(DisplayDesc{getLogger(), *m_window, m_graphicsEngine->getGraphicsContext()});
    m_uiManager = make_unique<UIManager>(UIManagerDesc{getLogger(), m_display->getSize()});
    m_inputManager = InputManager::create({getLogger(), *m_window});
    m_entityManager = make_unique<EntityManager>(EntityManagerDesc{getLogger()});
    m_isRunning = false;
    m_mainCamera = nullptr;

    m_inputManager->addListener(m_uiManager.get());
    GENESIS_LOG_INFO("Game initialized.");
}

Game::~Game() 
{
    GENESIS_LOG_INFO("Game is shutting down...");
    m_display.reset();
    m_uiManager.reset();
    m_entityManager.reset();
    m_inputManager.reset();
    m_resourceManager.reset();
    m_physicsEngine.reset();
    m_graphicsEngine.reset();
    m_window.reset();
    m_logger.reset();
}

Logger& Game::getLogger() noexcept
{
    return *m_logger;
}

GameContext Game::getContext() noexcept
{
    return {*m_entityManager, *m_inputManager, *m_resourceManager, *m_uiManager, *m_physicsEngine, *m_display};
}

void Game::onInternalUpdate()
{
    float deltaTime = getDeltaTime();

    m_inputManager->update();
    m_uiManager->update(deltaTime);
    onUpdate(deltaTime);
    m_entityManager->update(deltaTime);
    m_physicsEngine->update(*m_entityManager, deltaTime);

    m_graphicsEngine->clear();
    if (m_mainCamera) {
        m_graphicsEngine->render(*m_entityManager, *m_mainCamera, deltaTime);
#ifdef _DEBUG
        m_graphicsEngine->render(m_physicsEngine->getDebugRenderer(), *m_mainCamera);
#endif
        for (auto& effect : m_effects) {
            m_graphicsEngine->postProcess(*effect);
        }
    }
    m_graphicsEngine->render(*m_uiManager);
    m_graphicsEngine->present(*m_display);
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

Entity* Game::getMainCamera() const noexcept
{
    return m_mainCamera;
}

Rect Game::getRenderResolution() const noexcept
{
    return m_graphicsEngine->getRenderResolution();
}

AntiAliasing Game::getAntiAliasing() const noexcept
{
    return m_graphicsEngine->getAntiAliasing();
}

void Game::setMainCamera(Entity* camera)
{
    if (!camera) {
        GENESIS_LOG_WARNING("Failed to set main camera: Entity is null.");
        return;
    }
    if (!m_entityManager->hasEntity(camera->getId())) {
        GENESIS_LOG_WARNING("Failed to set main camera: Entity does not exist.", camera->getId());
        return;
    }
    if (!camera->getComponent<CameraComponent>()) {
        GENESIS_LOG_WARNING("Failed to set main camera: Entity is missing a CameraComponent.");
        return;
    }
    m_mainCamera = camera;
}

void Game::setRenderResolution(const Rect& resolution)
{
    m_graphicsEngine->setRenderResolution(resolution);
}

void Game::setAntiAliasing(AntiAliasing antiAliasing)
{
    m_graphicsEngine->setAntiAliasing(antiAliasing);
}

void Game::addEffect(SharedPtr<PostProcess> effect)
{
    m_effects.push_back(std::move(effect));
}

void Game::clearEffects()
{
    m_effects.clear();
}