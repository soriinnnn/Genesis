#ifndef GENESIS_ENTITY_H
#define GENESIS_ENTITY_H
#include <core/Base.h>
#include <core/Core.h>
#include <entity/components/Component.h>

namespace genesis
{
	struct EntityDesc
	{
		BaseDesc base;
		EntityId id;
		EntityManager& entityManager;
	};

	class Entity: public Base
	{
	public:
		explicit Entity(const EntityDesc& desc);
		virtual ~Entity() override;

		EntityId getId() const noexcept;
		virtual void update(float deltaTime);

		template<typename T>
		T* createComponent()
		{
			GENESIS_ASSERT((std::is_base_of<Component, T>::value), "T must derive from Component.");
			ComponentId id = typeid(T).hash_code();

			T* result = getComponent<T>(id);
			if (!result) {
				result = createComponent<T>(id);
			}
			return result;
		}

		template<typename T>
		void deleteComponent()
		{
			GENESIS_ASSERT((std::is_base_of<Component, T>::value), "T must derive from Component.");
			deleteComponent(typeid(T).hash_code());
		}

		template<typename T>
		T* getComponent()
		{
			GENESIS_ASSERT((std::is_base_of<Component, T>::value), "T must derive from Component.");
			return getComponent<T>(typeid(T).hash_code());
		}

	private:
		using ComponentId = size_t;
		
		template<typename T>
		T* createComponent(ComponentId id)
		{
			T* component = new T{ComponentDesc{m_logger, *this}};
			UniquePtr<Component> componentPtr{component};

			m_components.emplace(id, std::move(componentPtr));
			return component;
		}

		void deleteComponent(ComponentId id);

		template<typename T>
		T* getComponent(ComponentId id)
		{
			auto it = m_components.find(id);
			if (it == m_components.end()) {
				return nullptr;
			}
			return static_cast<T*>(it->second.get());
		}
	
	protected:
		EntityId m_id;
		HashMap<ComponentId, UniquePtr<Component>> m_components;
		EntityManager& m_entityManager;
	};
}

#endif