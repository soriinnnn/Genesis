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
		virtual ~World() override;

		void update(float deltaTime);
		EntityManager& getEntityManager();

		// PROVA...
		Player* getPlayer();

	protected:
		explicit World(const WorldDesc& desc);

		virtual void onUpdate(float deltaTime) = 0;

	protected:
		InputManager& m_inputManager;
		ResourceManager& m_resourceManager;
		UniquePtr<EntityManager> m_entityManager;
		
		// PROVA...
		Player* m_player;
	};
}

#endif