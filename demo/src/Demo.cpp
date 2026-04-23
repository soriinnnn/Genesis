#include "Demo.h"
#include <resources/Font.h>
#include <entity/Entity.h>
#include <entity/components/Transform.h>
#include <ui/elements/UILabel.h>
#include <ui/elements/UIImage.h>
#include <ui/elements/UIPanel.h>
#include <ui/elements/UIButton.h>

using namespace std;

static void updateFPS(float deltaTime, UILabel& label);
static void updateScreenSize(Rect size, UILabel& label);
static void updatePlayerPosition(Vec3 position, UILabel& label);
static void updatePlayerRotation(Vec3 rotation, UILabel& label);

Demo::Demo(const GameDesc& desc): Game(desc), m_centerMouse{false} {}

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
        case Key::V:
        {
            // VSYNC
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

            auto* screenSize = m_uiManager->getElement<UILabel>("screenSize");
            updateScreenSize(m_display->getImageResolution(), *screenSize);
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

    auto* fps = m_uiManager->createElement<UILabel>("fps");
    fps->setFont(m_resourceManager->getFont("demo/assets/fonts/calibri_16.spritefont"));
    fps->setColor({0.0f, 0.0f, 0.0f, 1.0f});
    fps->setContent("FPS: ");

    auto* screenSize = m_uiManager->createElement<UILabel>("screenSize");
    screenSize->setFont(m_resourceManager->getFont("demo/assets/fonts/calibri_16.spritefont"));
    screenSize->setColor({0.0f, 0.0f, 0.0f, 1.0f});
    screenSize->setPosition({0, 26});
    updateScreenSize(m_display->getImageResolution(), *screenSize);

    auto* playerPosition = m_uiManager->createElement<UILabel>("playerPosition");
    playerPosition->setFont(m_resourceManager->getFont("demo/assets/fonts/calibri_16.spritefont"));
    playerPosition->setColor({0.0f, 0.0f, 0.0f, 1.0f});
    playerPosition->setPosition({0, 52});
    updatePlayerPosition({}, *playerPosition);

    auto* playerRotation = m_uiManager->createElement<UILabel>("playerRotation");
    playerRotation->setFont(m_resourceManager->getFont("demo/assets/fonts/calibri_16.spritefont"));
    playerRotation->setColor({0.0f, 0.0f, 0.0f, 1.0f});
    playerRotation->setPosition({0, 78});
    updatePlayerRotation({}, *playerRotation);

    auto* ammo = m_uiManager->createElement<UIImage>("ammo");
    ammo->setTexture(m_resourceManager->getTexture("assets/textures/UI/ammo.png"));
    ammo->setPosition({0, 120});
    ammo->setScale({1.0f, 1.0f});
    ammo->setColor({0.0f, 0.0f, 0.0f, 1.0f});

    auto* button = m_uiManager->createElement<UIButton>("button");
    button->setPosition({5, 250});
    button->setColor({0.4f, 0.4f, 0.4f, 1.0f});
    button->setSize({100, 50});

    auto* buttonLabel = button->getLabel();
    buttonLabel->setFont(m_resourceManager->getFont("demo/assets/fonts/calibri_16.spritefont"));
    buttonLabel->setContent("Malo");
    
    button->centerLabel();
    button->setOnMouseEnterCallback([button]() {
        button->setColor({0.3f, 0.3f, 0.3f, 1.0f});
    });
    button->setOnMouseOutCallback([button]() {
        button->setColor({0.4f, 0.4f, 0.4f, 1.0f});
    });
    button->setOnMouseUpCallback([&](MouseButton button) {
        GENESIS_LOG(getLogger(), Logger::LogLevel::Info, "Malo");
    });
}

void Demo::onUpdate(float deltaTime)
{
    auto* fps = m_uiManager->getElement<UILabel>("fps");
    auto* playerPosition = m_uiManager->getElement<UILabel>("playerPosition");
    auto* playerRotation = m_uiManager->getElement<UILabel>("playerRotation");

    updateFPS(deltaTime, *fps);

    auto* camera = m_world->getCamera();
    if (!camera) {
        return;
    }
    
    updatePlayerPosition(camera->getComponent<Transform>()->getPosition(), *playerPosition);
    updatePlayerRotation(camera->getComponent<Transform>()->getRotation(), *playerRotation);
}

void updateFPS(float deltaTime, UILabel& label)
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

void updateScreenSize(Rect size, UILabel& label)
{
    String content = "Screen size {";
    content += "width: " + std::to_string(size.width()) + ", ";
    content += "height: " + std::to_string(size.height()) + "}";
    label.setContent(content.c_str());
}

void updatePlayerPosition(Vec3 position, UILabel& label)
{
    String content = "Player position {";
    content += "x: " + std::to_string(position.x) + ", ";
    content += "y: " + std::to_string(position.y) + ", ";
    content += "z: " + std::to_string(position.z) + "}";
    label.setContent(content.c_str());
}

void updatePlayerRotation(Vec3 rotation, UILabel& label)
{
    const float radToDeg = 180.0f / 3.14159265359f;
    float rotX = rotation.x * radToDeg;
    float rotY = rotation.y * radToDeg;
    float rotZ = rotation.z * radToDeg;

    String content = "Player rotation {";
    content += "x: " + std::to_string(rotX) + ", ";
    content += "y: " + std::to_string(rotY) + ", ";
    content += "z: " + std::to_string(rotZ) + "}";
    label.setContent(content.c_str());
}