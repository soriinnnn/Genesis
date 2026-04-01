#include <game/TestWorld.h>
#include <input/InputManager.h>
#include <entity/Entity.h>
#include <entity/Player.h>
#include <entity/EntityManager.h>
#include <entity/components/Transform.h>
#include <entity/components/Camera.h>
#include <entity/components/MeshRenderer.h>
#include <entity/components/PlayerController.h>
#include <resources/ResourceManager.h>

using namespace genesis;

static void showFramerate(float deltaTime, Logger& logger);

TestWorld::TestWorld(const WorldDesc& desc): World(desc) 
{
    SharedPtr<Mesh> mesh = m_resourceManager.getMesh("demo/assets/meshes/sphere.obj", GENESIS_VERTEX_PRESET_NORMAL_MAPPED);
    SharedPtr<Material> material = m_resourceManager.getMaterial("demo/assets/materials/statue.json");

    Entity* statue = m_entityManager->createEntity<Entity>();
    Transform* statueTransform = statue->createComponent<Transform>();
    statueTransform->setScale({0.5, 0.5, 0.5});
    MeshRenderer* statueMesh = statue->createComponent<MeshRenderer>();
    statueMesh->setMesh(mesh);
    statueMesh->setMaterial(material);

    m_player = m_entityManager->createEntity<Player>();
    m_player->getComponent<PlayerController>()->setInputManager(m_inputManager);
}

TestWorld::~TestWorld() 
{
    m_resourceManager.unloadUnused();
}

void TestWorld::onUpdate(float deltaTime)
{
    showFramerate(deltaTime, getLogger());
    for (auto& [id, entity] : m_entityManager->getEntities()) {
        entity->update(deltaTime);
    }
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