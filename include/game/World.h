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
	};

	class World: public Base
	{
	public:
		explicit World(const WorldDesc& desc);
		virtual ~World() override;

	private:
		InputManager& m_inputManager;
		ResourceManager& m_resourceManager;
		UniquePtr<EntityManager> m_entityManager;
	};
}


#endif