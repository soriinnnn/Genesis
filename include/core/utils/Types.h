#ifndef GENESIS_TYPES_H
#define GENESIS_TYPES_H
#include <core/Core.h>

GENESIS_NAMESPACE_BEGIN

struct GameContext
{
	EntityManager& entities;
	InputManager& input;
	ResourceManager& resources;
	UIManager& ui;
	ScriptManager& scripts;
	PhysicsEngine& physics;
	Display& display;
	Vector<SharedPtr<PostProcess>>& effects;
};

struct ScriptContext
{
	InputManager& input;
	EntityManager& entities;
	ResourceManager& resources;
	UIManager& ui;
	PhysicsEngine& physics;
};

enum class AntiAliasing
{
	None,
	MSAA_2X,
	MSAA_4X,
	MSAA_8X
};

enum class TextureFiltering
{
	Point,
	Bilinear,
	Trilinear,
	Anisotropic_4X,
	Anisotropic_8X,
	Anisotropic_16X
};

GENESIS_NAMESPACE_END

#endif