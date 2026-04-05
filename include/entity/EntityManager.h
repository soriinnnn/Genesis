#ifndef GENESIS_ENTITY_MANAGER_H
#define GENESIS_ENTITY_MANAGER_H
#include <core/Base.h>
#include <core/Core.h>
#include <entity/Entity.h>

namespace genesis
{
	struct EntityManagerDesc
	{
		BaseDesc base;
	};

	class EntityManager final: public Base
	{
	public:
		explicit EntityManager(const EntityManagerDesc& desc);
		~EntityManager() override;

		void update(float deltaTime);

		Entity* createEntity();
		Entity* getEntity(EntityId id);
		void destroyEntity(EntityId id);

		template<typename F>
		void forEach(F&& callback) const
		{
			for (const auto& [key, entity] : m_entities) {
				callback(*entity);
			}
		}

		template<typename F>
		void forEach(F&& callback)
		{
			for (auto& [key, entity] : m_entities) {
				callback(*entity);
			}
		}

	private:
		EntityId getAvailableId();

	private:
		enum class EventType
		{
			Create,
			Destroy
		};

		struct EntityEvent
		{
			EventType type;
			EntityId id;
		};

	private:
		EntityId m_nextId;
		Vector<EntityId> m_idPool;
		Vector<EntityEvent> m_events;
		HashMap<EntityId, UniquePtr<Entity>> m_entities;
		HashMap<EntityId, UniquePtr<Entity>> m_pendingEntities;
	};
}

#endif