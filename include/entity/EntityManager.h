#ifndef GENESIS_ENTITY_MANAGER_H
#define GENESIS_ENTITY_MANAGER_H
#include <core/Base.h>
#include <core/Core.h>

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

		Entity* createEntity(const char* name = nullptr);
		void destroyEntity(EntityId id);
		Entity* getEntity(EntityId id);

		void destroyEntityByName(const char* name);
		Entity* getEntityByName(const char* name);

		template<typename F> 
		void forEach(F&& callback) const;

		template<typename F>
		void forEach(F&& callback);

		template<typename T, typename F>
		void forEachComponent(F&& callback) const;

		template<typename T, typename F>
		void forEachComponent(F&& callback);

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
		EntityId getAvailableEntityId();
		void registerComponent(TypeId id, Component* component);
		void unregisterComponent(TypeId id, Component* component);

	private:
		EntityId m_nextId;
		Vector<EntityId> m_idPool;
		HashMap<TypeId, Vector<Component*>> m_components;
		HashMap<EntityId, UniquePtr<Entity>> m_entities;
		HashMap<EntityId, UniquePtr<Entity>> m_pendingEntities;
		Vector<EntityEvent> m_events;
		HashMap<String, EntityId> m_nameToId;

		friend class Entity;
	};
}

#include <entity/EntityManager.inl>
#endif