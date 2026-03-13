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

		uint32 createEntity();
		void destroyEntity(uint32 entityId);

		template<typename T, typename... Args>
		T& addComponent(uint32 entityId, Args&&... args)
		{

		}

		template<typename T>
		void removeComponent(uint32 entityId)
		{

		}

		template<typename T>
		T& getComponent(uint32 entityId)
		{

		}

		template<typename T>
		bool hasComponent(uint32 entityId)
		{

		}

	private:
		uint32 m_nextEntityId;
		std::queue<uint32> m_availableEntityId;
		std::unordered_map<uint32, UniquePtr<Entity>> m_entities;
	};
}

#endif