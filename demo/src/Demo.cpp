#include "Demo.h"
#include <resources/Font.h>

using namespace std;

static void showFramerate(float deltaTime, UILabel& label, Logger& logger);

Demo::Demo(const GameDesc& desc): Game(desc), m_centerMouse{false}, m_label{nullptr} {}

Demo::~Demo() {}

void Demo::onKeyDown(Key key) {}

void Demo::onKeyUp(Key key)
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

void Demo::onMouseMove(Point delta, Point pos) {}

void Demo::onMouseDown(MouseButton button, Point pos) {}

void Demo::onMouseUp(MouseButton button, Point pos) {}

void Demo::onCreate()
{
    m_inputManager->addListener(this);
    m_display->onResizeWindow([&](uint32 width, uint32 height) {
        setImageResolution(width, height);
    });

    m_label = m_uiManager->createElement<UILabel>();
    m_label->setFont(m_resourceManager->getFont("demo/assets/fonts/arial_48.spritefont"));
    m_label->setColor({0.0f, 0.0f, 0.0f, 1.0f});
}

void Demo::onUpdate(float deltaTime)
{
    showFramerate(deltaTime, *m_label, getLogger());
}

void showFramerate(float deltaTime, UILabel& label, Logger& logger)
{
    static float timer = 0.0f;
    static int frames = 0;

    timer += deltaTime;
    frames++;
    if (timer >= 1.0f) {
        String content = "FPS: " + std::to_string(frames);
        label.setContent(content.c_str());  

        frames = 0;
        timer -= 1.0f;
    }
}