#include "Demo.h"
#include <ui/elements/UIPanel.h>

using namespace std;

Demo::Demo(const GameDesc& desc): Game(desc) {}

Demo::~Demo() {}

void Demo::onKeyDown(Key key) {}

void Demo::onKeyUp(Key key)
{
    switch (key) {
        case Key::G:
        {
            bool isMouseLocked = m_inputManager->isMouseLocked();
            m_inputManager->setMouseLock(!isMouseLocked);
            m_inputManager->setMouseVisibility(isMouseLocked);
            break;
        }
        case Key::V:
        {
            bool vsync = getVSync();
            setVSync(!vsync);
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

void Demo::onMouseMove(Point delta, Point pos) {}

void Demo::onMouseDown(MouseButton button, Point pos) {}

void Demo::onMouseUp(MouseButton button, Point pos) {}

void Demo::onCreate()
{
    setVSync(true);
    setMainCamera(m_entityManager->getEntityByName("camera"));
    m_inputManager->addListener(this);

    m_display->onResizeWindow([this](uint32 width, uint32 height) {
        setRenderResolution(width, height);

        m_uiManager->setCanvasSize(Rect{static_cast<int32>(width), static_cast<int32>(height)});
        m_uiManager->getElement<UIPanel>("menuPanel1")->setSize({static_cast<int32>(width), static_cast<int32>(height)});
    });
}

void Demo::onUpdate(float deltaTime) {}