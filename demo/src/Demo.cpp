#include "Demo.h"
#include "utils/Macros.h"
#include "scripts/MainMenu.h"
#include <entity/components/CameraComponent.h>
#include <entity/components/ScriptComponent.h>

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
        case Key::F9: {
            m_display->setVSync(!m_display->getVSync());
            break;
        }
        case Key::F11: {
            DisplayMode mode = (m_display->getMode() != DisplayMode::Fullscreen) ? DisplayMode::Fullscreen : DisplayMode::Windowed;
            m_display->setMode(mode);
            break;
        }
        case Key::F12: {
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

    setMainCamera(m_entityManager->getEntityByName(ENTITIES_MAIN_CAMERA));
    setSkybox(m_resourceManager->getSkybox(ASSETS_SKYBOX));
    setTextureFiltering(TextureFiltering::Anisotropic_16X);
    setAntiAliasing(AntiAliasing::MSAA_8X);

    Entity* scripts = m_entityManager->createEntity(ENTITIES_GLOBAL_SCRIPTS);
    ScriptComponent* scriptComponent = scripts->createComponent<ScriptComponent>();
    scriptComponent->addScript(m_scriptManager->createScript<MainMenu>());
}

void Demo::onUpdate(float deltaTime) {}