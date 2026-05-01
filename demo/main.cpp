#include "src/Demo.h"
#include "src/components/PlayerControllerComponent.h"
#include <resources/Font.h>
#include <resources/Mesh.h>
#include <resources/Material.h>
#include <entity/components/LightComponent.h>
#include <entity/components/CameraComponent.h>
#include <entity/components/TransformComponent.h>
#include <entity/components/MeshRendererComponent.h>
#include <entity/components/RigidBodyComponent.h>
#include <physics/RigidBody.h>


#include <ui/elements/UILabel.h>

using namespace genesis;

void createScene(Game& game)
{
	auto context = game.getContext();

	SharedPtr<Material> brickMaterial = context.resources.getMaterial("demo/assets/materials/brick.json");

	SharedPtr<Mesh> floorMesh = context.resources.getMesh("assets/meshes/terrain.obj", GENESIS_VERTEX_PRESET_NORMAL_MAPPED);
	Entity* floor = context.world.createEntity("floor");

	TransformComponent* floorTransform = floor->createComponent<TransformComponent>();
	floorTransform->setScale({0.5f, 0.5f, 0.5f});

	MeshRendererComponent* floorMeshRenderer = floor->createComponent<MeshRendererComponent>();
	floorMeshRenderer->setMesh(floorMesh);
	floorMeshRenderer->setMaterial(brickMaterial);

	RigidBodyComponent* floorRigidBody = floor->createComponent<RigidBodyComponent>();
	floorRigidBody->setBody(context.physics.createBox(floorTransform->getPosition(), {50.0f, 0.0f, 50.0f}, PhysicsEngine::MotionType::Static));

	SharedPtr<Mesh> cubeMesh = context.resources.getMesh("assets/meshes/box.obj", GENESIS_VERTEX_PRESET_NORMAL_MAPPED);
	Entity* cube = context.world.createEntity("cube");

	TransformComponent* cubeTransform = cube->createComponent<TransformComponent>();
	cubeTransform->setPosition({0.0f, 2.5f, 0.0f});

	MeshRendererComponent* cubeMeshRenderer = cube->createComponent<MeshRendererComponent>();
	cubeMeshRenderer->setMesh(cubeMesh);
	cubeMeshRenderer->setMaterial(brickMaterial);

	RigidBodyComponent* cubeRigidBody = cube->createComponent<RigidBodyComponent>();
	cubeRigidBody->setBody(context.physics.createBox({0.0f, 100.0f, 0.0f}, {0.8f, 0.8f, 0.8f}, PhysicsEngine::MotionType::Dynamic));
	cubeRigidBody->getBody()->addForce({0.0f, -2.0f, 0.0f});

	auto* body = cubeRigidBody->getBody();
	body->setOnContactAddedCallback([body](const RigidBody::ContactAddedData& data) {
		body->setPosition({0.0f, 10.0f, 0.0f});
	});
}

void createLights(Game& game)
{
	auto context = game.getContext();

	Entity* sun = context.world.createEntity("sun");

	LightComponent* sunLight = sun->createComponent<LightComponent>();
	sunLight->setType(LightComponent::LightType::Directional);
	sunLight->setColor(Vec3{1.0f, 0.95f, 0.85f});
	sunLight->setIntensity(1.0f);

	TransformComponent* sunTransform = sun->getComponent<TransformComponent>();
	sunTransform->setPosition(Vec3{0.0f, 100.0f, 0.0f});
	sunTransform->setRotation(Vec3{-0.785f, 0.523f, 0.0f});
}

void createCamera(Game& game)
{
	auto context = game.getContext();

	Entity* camera = context.world.createEntity("camera");
	camera->createComponent<CameraComponent>();
	camera->getComponent<TransformComponent>()->setPosition({0, 1, -1});
	camera->createComponent<PlayerControllerComponent>()->setInputManager(context.input);
	context.world.setCamera(camera);
}

void setupWorld(Game& game) 
{
	createCamera(game);
	createLights(game);
	createScene(game);
}

int main()
{
	try {
		Demo game({.windowTitle = "Demo", .logLevel = Logger::LogLevel::Info});
		setupWorld(game);
		game.run();
	}
	catch (const std::runtime_error&) {
		return EXIT_FAILURE;
	}
	catch (const std::invalid_argument&) {
		return EXIT_FAILURE;
	}
	catch (const std::exception&) {
		return EXIT_FAILURE;
	}
	catch (...) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}