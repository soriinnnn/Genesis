#include <game/TestWorld.h>
#include <display/Display.h>
#include <graphics/GraphicsEngine.h>
#include <graphics/GraphicsDevice.h>
#include <input/InputManager.h>
#include <entity/EntityManager.h>
#include <entity/Entity.h>
#include <entity/Player.h>
#include <entity/components/Transform.h>
#include <entity/components/Camera.h>
#include <entity/components/MeshRenderer.h>
#include <resources/ResourceManager.h>
#include <resources/VertexShader.h>
#include <resources/PixelShader.h>
#include <resources/Mesh.h>
#include <resources/Texture.h>

using namespace genesis;

static void showFramerate(float deltaTime, Logger& logger);
static void updatePlayer(float deltaTime, Player* player, InputManager& inputManager);

TestWorld::TestWorld(const WorldDesc& desc): World(desc) 
{
    SharedPtr<Mesh> mesh = m_resourceManager.getResource<Mesh>("demo/assets/meshes/statue.obj");
    SharedPtr<Texture> texture = m_resourceManager.getResource<Texture>("demo/assets/textures/asteroid.jpg");
    SharedPtr<VertexShader> vs = m_resourceManager.getResource<VertexShader>("demo/assets/shaders/basic/vs.hlsl@main");
    SharedPtr<PixelShader> ps = m_resourceManager.getResource<PixelShader>("demo/assets/shaders/basic/ps.hlsl@main");

    Entity* statue = m_entityManager->createEntity<Entity>();
    Transform* statueTransform = statue->createComponent<Transform>();
    MeshRenderer* statueMesh = statue->createComponent<MeshRenderer>();
    statueMesh->setMesh(mesh);
    statueMesh->setTexture(texture);
    statueMesh->setGraphicsPipelineState(
        m_graphicsEngine.getGraphicsDevice().createGraphicsPipelineState({vs->getVertexShaderSignature(), ps->getShaderBinary(), PrimitiveTopology::Triangles})
    );

    Player* player = m_entityManager->createEntity<Player>();
    Transform* playerTransform = player->createComponent<Transform>();
    Camera* playerCamera = player->createComponent<Camera>();
    playerTransform->setPosition(Vec3{0, 0, -1});
    playerCamera->setFov(1.57f);

    // PROVA...
    m_player = player;
    m_statue = statue;

    m_inputManager.setMouseVisibility(false);
    m_resourceManager.unloadUnused();
}

TestWorld::~TestWorld() 
{
    m_resourceManager.unloadUnused();
}

void TestWorld::update(float deltaTime)
{
    // PROVA...
    Rect size = m_display.getSize();
    float aspect = static_cast<float>(size.width())/size.height();

    m_entityManager->destroyPending();

    showFramerate(deltaTime, getLogger());
    updatePlayer(deltaTime, m_player, m_inputManager);
    m_inputManager.setMousePosition({size.width()/2, size.height()/2});
    m_player->getComponent<Camera>()->setAspectRatio(aspect);

    m_graphicsEngine.render(*m_player, m_display.getSwapChain(), deltaTime);
    m_graphicsEngine.render(*m_statue, m_display.getSwapChain(), deltaTime);
}

/* STATIC FUNCTION DEFINITIONS */

static void showFramerate(float deltaTime, Logger& logger) 
{
    static float timer = 0.0f;
    static int frames = 0;

    timer += deltaTime;
    frames++;
    if (timer >= 1.0f) {
        GENESIS_LOG(logger, Logger::LogLevel::Info, "FPS: {} - Delta: {} ms", frames, (1000.0f / frames));
        frames = 0;
        timer -= 1.0f;
    }
}

static void updatePlayer(float deltaTime, Player* player, InputManager& inputManager)
{
    Transform* transform = player->getComponent<Transform>();
    Vec3 position = transform->getPosition();
    Vec3 rotation = transform->getRotation();
    Vec3 forward{
         std::cos(rotation.x) * std::sin(rotation.y),
        -std::sin(rotation.x),
         std::cos(rotation.x) * std::cos(rotation.y)
    };
    Vec3 right = Vec3::normalize(Vec3::cross(Vec3{0, 1, 0}, forward));

    if (inputManager.isKeyDown(Key::W)) {
        position += forward * 1.0f * deltaTime;
    }
    if (inputManager.isKeyDown(Key::S)) {
        position -= forward * 1.0f * deltaTime;
    }
    if (inputManager.isKeyDown(Key::D)) {
        position += right * 1.0f * deltaTime;
    }
    if (inputManager.isKeyDown(Key::A)) {
        position -= right * 1.0f * deltaTime;
    }

    Point mouseDelta = inputManager.getMouseDelta();
    rotation.x += mouseDelta.y * deltaTime;
    rotation.y += mouseDelta.x * deltaTime;

    const float maxPitch = 1.553f;
    if (rotation.x > maxPitch)  rotation.x = maxPitch;
    if (rotation.x < -maxPitch) rotation.x = -maxPitch;

    transform->setPosition(position);
    transform->setRotation(rotation);
}