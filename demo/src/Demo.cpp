#include "Demo.h"

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
            m_display->setVSync(!m_display->getVSync());
            break;
        }
        case Key::F11:
        {
            if (m_display->getMode() == DisplayMode::Windowed) {
                m_display->setMode(DisplayMode::Fullscreen);
            }
            else {
                m_display->setMode(DisplayMode::Windowed);
            }
            break;
        }
    }
}

void Demo::onMouseMove(Point delta, Point pos) {}

void Demo::onMouseDown(MouseButton button, Point pos) {}

void Demo::onMouseUp(MouseButton button, Point pos) {}

void Demo::onCreate()
{
    m_display->setVSync(true);
    setMainCamera(m_entityManager->getEntityByName("camera"));
    setAntiAliasing(AntiAliasing::MSAA_8X);
    setTextureFiltering(TextureFiltering::Anisotropic_16X);
    m_inputManager->addListener(this);

    m_display->onResize([this](Rect size) {
        setRenderResolution(size);
        m_uiManager->setCanvasSize(size);
        //m_uiManager->getElement<UIPanel>("menuPanel1")->setSize(size);
    });
}

void Demo::onUpdate(float deltaTime) {}