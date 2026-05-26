#ifndef GENESIS_TYPES_H
#define GENESIS_TYPES_H
#include <core/Core.h>

GENESIS_NAMESPACE_START

struct GameContext
{
	EntityManager& entities;
	InputManager& input;
	ResourceManager& resources;
	UIManager& ui;
	PhysicsEngine& physics;
	Display& display;
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
	Bilinear,
	Trilinear,
	Anisotropic_4X,
	Anisotropic_8X,
	Anisotropic_16X
};

GENESIS_NAMESPACE_END

#endif