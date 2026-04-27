#ifndef GENESIS_TYPES_H
#define GENESIS_TYPES_H
#include <core/Core.h>

namespace genesis 
{
	struct BinaryData
	{
		const void* data{};
		size_t dataSize{};
	};

	struct GameContext
	{
		World& world;
		InputManager& input;
		ResourceManager& resources;
		UIManager& ui;
		PhysicsEngine& physics;
	};
}

#endif