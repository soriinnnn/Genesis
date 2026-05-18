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
    m_graphicsEngine = make_unique<GraphicsEngine>(GraphicsEngineDesc{*m_logger, desc.windowSize});
    m_resourceManager = make_unique<ResourceManager>(ResourceManagerDesc{*m_logger, m_graphicsEngine->getGraphicsContext()});
    m_display = make_unique<Display>(DisplayDesc{*m_logger, desc.windowSize, desc.windowTitle, WindowStyle::Windowed, m_graphicsEngine->getGraphicsContext()});
    m_inputManager = InputManager::create({*m_logger, m_display->getWindow()});
    m_entityManager = make_unique<EntityManager>(EntityManagerDesc{*m_logger});
    m_uiManager = make_unique<UIManager>(UIManagerDesc{*m_logger});
    m_physicsEngine = make_unique<PhysicsEngine>(PhysicsEngineDesc{*m_logger, m_graphicsEngine->getGraphicsContext()});
    m_isRunning = false;
    m_vsync = false;
    m_mainCamera = GENESIS_INVALID_ENTITY;
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

    Entity* camera = m_entityManager->getEntity(m_mainCamera);
    if (camera) {
        m_graphicsEngine->render(*m_entityManager, *camera, deltaTime);
#ifdef _DEBUG
        m_graphicsEngine->render(m_physicsEngine->getDebugRenderer(), *camera);
#endif
        for (auto& effect : m_effects) {
            m_graphicsEngine->postProcess(*effect);
        }
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

EntityId Game::getMainCamera() const noexcept
{
    return m_mainCamera;
}

Rect Game::getRenderResolution() const noexcept
{
    return m_graphicsEngine->getRenderResolution();
}

bool Game::getVSync() const noexcept
{
    return m_vsync;
}

void Game::setMainCamera(EntityId camera)
{
    Entity* entity = m_entityManager->getEntity(camera);
    if (!entity) {
        GENESIS_LOG_WARNING("Trying to set main camera with nonexistent entity ID.");
        return;
    }
    if (!entity->getComponent<CameraComponent>()) {
        GENESIS_LOG_WARNING("Entity has no camera component.");
    }
    m_mainCamera = camera;
}

void Game::setRenderResolution(uint32 width, uint32 height)
{
    m_graphicsEngine->setRenderResolution(width, height);
}

void Game::setVSync(bool enabled)
{
    m_vsync = enabled;
}

void Game::addEffect(SharedPtr<PostProcess> effect)
{
    m_effects.push_back(std::move(effect));
}

void Game::clearEffects()
{
    m_effects.clear();
}