#ifndef GENESIS_TYPES_H
#define GENESIS_TYPES_H
#include <core/Core.h>
#include <graphics/utils/GraphicsTypes.h>

namespace genesis 
{
	struct BinaryData
	{
		const void* data{};
		size_t size{};
	};

    struct GameContext
    {
        EntityManager& entities;
        InputManager& input;
        ResourceManager& resources;
        UIManager& ui;
        PhysicsEngine& physics;
        Display& display;
    };

	struct GraphicsContext
	{
		GraphicsDevice& graphicsDevice;
		EngineShaders& engineShaders;
		EngineStates& engineStates;
	};
}

#endif