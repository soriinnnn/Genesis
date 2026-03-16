#ifndef GENESIS_WORLD_H
#define GENESIS_WORLD_H
#include <core/Base.h>
#include <core/Core.h>

namespace genesis
{
	struct WorldDesc
	{
		BaseDesc base;
		InputManager& inputManager;
		ResourceManager& resourceManager;
		GraphicsEngine& graphicsEngine;
		Display& display;
	};

	class World: public Base
	{
	public:
		virtual ~World() override;

		virtual void update(float deltaTime) = 0;

	protected:
		explicit World(const WorldDesc& desc);

	protected:
		InputManager& m_inputManager;
		ResourceManager& m_resourceManager;
		GraphicsEngine& m_graphicsEngine;
		Display& m_display;
		UniquePtr<EntityManager> m_entityManager;
	};
}

#endif