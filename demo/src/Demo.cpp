#include "Demo.h"
#include "utils/Macros.h"
#include "utils/Utils.h"
#include <entity/components/CameraComponent.h>

using namespace utils;

Demo::Demo(const GameDesc& desc): Game(desc) {}

Demo::~Demo() {}

void Demo::onKeyDown(Key key) {}

void Demo::onKeyUp(Key key)
{
    switch (key) {
        case Key::G: {
            bool mouseLock = m_inputManager->isMouseLocked();
            m_inputManager->setMouseLock(!mouseLock);
            m_inputManager->setMouseVisibility(mouseLock);
            break;
        }
        case Key::F1: {
            UIElement* fps = m_uiManager->getElement<UIElement>(UI_INFO_FPS);
            fps->setVisible(!fps->isVisible());
            break;
        }
        case Key::F2: {
            m_display->setVSync(!m_display->getVSync());
            break;
        }
        case Key::F11: {
            DisplayMode mode = (m_display->getMode() != DisplayMode::Borderless) ? DisplayMode::Borderless : DisplayMode::Windowed;
            m_display->setMode(mode);
            break;
        }
    }
}

void Demo::onMouseMove(Point delta, Point pos) {}

void Demo::onMouseDown(MouseButton button, Point pos) {}

void Demo::onMouseUp(MouseButton button, Point pos) {}

void Demo::onCreate()
{
    m_inputManager->addListener(this);
    m_display->setVSync(true);
    m_display->onResize([this](Rect size) {
        setRenderResolution(size);
        m_uiManager->setCanvasSize(size);
    });

    Entity* camera = m_entityManager->createEntity(ENTITIES_MAIN_CAMERA);
    camera->createComponent<CameraComponent>();
    //camera->createComponent<ScriptComponent>()->addScript(m_scriptManager->createScript<PlayerMovement>());

    setMainCamera(m_entityManager->getEntityByName(ENTITIES_MAIN_CAMERA));
    setSkybox(m_resourceManager->getSkybox(ASSETS_SKYBOX));
}

void Demo::onUpdate(float deltaTime) 
{
    updateFPSLabel(deltaTime, *m_uiManager->getElement<UILabel>(UI_INFO_FPS));

    /*
    Entity* camera = m_entityManager->getEntityByName(ENTITIES_MAIN_CAMERA);

    Vec3 rotation = camera->getComponent<TransformComponent>()->getRotation();
    Vec3 forward = camera->getComponent<TransformComponent>()->getForwardVector();

    GENESIS_LOG_INFO("Camera rot: x={}, y={}, z={}", rotation.x, rotation.y, rotation.z);
    GENESIS_LOG_INFO("Camera forward: x={}, y={}, z={}", forward.x, forward.y, forward.z);
    */
}