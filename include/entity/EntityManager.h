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

		template<typename T>
		T* createEntity()
		{
			GENESIS_ASSERT((std::is_base_of<Entity, T>::value), "T must derive from Entity.");
			return createEntity<T>(getAvailableId());
		}

		const std::unordered_map<EntityId, UniquePtr<Entity>>& getEntities() const;

		void destroyEntity(EntityId id);
		void destroyPending();

	private:
		EntityId getAvailableId();

		template<typename T>
		T* createEntity(EntityId id)
		{
			T* entity = new T(EntityDesc{m_logger, *this, id});
			UniquePtr<Entity> entityPtr{entity};

			m_entities.emplace(id, std::move(entityPtr));
			return entity;
		}

	private:
		EntityId m_nextId;
		std::vector<EntityId> m_idPool;
		std::unordered_map<EntityId, UniquePtr<Entity>> m_entities;
		std::vector<EntityId> m_entitiesToDestroy;
	};
}

#endif