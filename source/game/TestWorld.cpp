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
#include <entity/components/PlayerController.h>
#include <resources/ResourceManager.h>
#include <resources/VertexShader.h>
#include <resources/PixelShader.h>
#include <resources/Mesh.h>
#include <resources/Texture.h>

using namespace genesis;

static void showFramerate(float deltaTime, Logger& logger);

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

    // MATERIAL RESOURCE
    statueMesh->setGraphicsPipelineState(
        m_graphicsEngine.getGraphicsDevice().createGraphicsPipelineState({vs->getVertexShaderSignature(), ps->getShaderBinary(), PrimitiveTopology::Triangles})
    );

    m_player = m_entityManager->createEntity<Player>();

    // PROVA...
    PlayerController* controller = m_player->createComponent<PlayerController>();
    m_inputManager.addListener(controller);
}

TestWorld::~TestWorld() 
{
    m_resourceManager.unloadUnused();

    // PROVA...
    PlayerController* controller = m_player->getComponent<PlayerController>();
    m_inputManager.removeListener(controller);
}

void TestWorld::onUpdate(float deltaTime)
{
    showFramerate(deltaTime, getLogger());
    m_player->update(deltaTime);
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