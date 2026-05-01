#ifndef GENESIS_WORLD_H
#define GENESIS_WORLD_H
#include <core/Base.h>
#include <core/Core.h>
#include <entity/EntityManager.h>

namespace genesis
{
	struct WorldDesc
	{
		BaseDesc base;
	};

	class World final: public Base
	{
	public:
		explicit World(const WorldDesc& desc);
		virtual ~World() override;

		void update(float deltaTime);
		
		Entity* createEntity();
		Entity* getEntity(EntityId id);

		Entity* createEntity(const char* name);
		Entity* getEntity(const char* name);

		Entity* getCamera();
		void setCamera(Entity* camera);

		template<typename F>
		void forEach(F&& callback) const
		{
			m_entityManager->forEach(std::forward<F>(callback));
		}

		template<typename F>
		void forEach(F&& callback)
		{
			m_entityManager->forEach(std::forward<F>(callback));
		}

	protected:
		UniquePtr<EntityManager> m_entityManager;
		HashMap<String, EntityId> m_entityMapping;
		Entity* m_camera;
	};
}

#endif