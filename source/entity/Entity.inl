#include <entity/Entity.h>
#include <entity/EntityManager.h>
#include <entity/components/Component.h>

namespace genesis
{
	template<typename T>
	inline bool Entity::hasComponent() const
	{
		GENESIS_ASSERT((std::is_base_of<Component, T>::value), "T must derive from Component.");
		return getComponent<T>() != nullptr;
	}

	template<typename T>
	inline T* Entity::createComponent()
	{
		GENESIS_ASSERT((std::is_base_of<Component, T>::value), "T must derive from Component.");
		TypeId id = typeid(T).hash_code();

		T* result = getComponent<T>();
		if (!result) {
			UniquePtr<T> component = std::make_unique<T>(ComponentDesc{m_logger, *this});
			result = component.get();

			m_components.emplace(id, std::move(component));
			m_manager.registerComponent(id, result);
		}
		return result;
	}

	template<typename T>
	inline void Entity::deleteComponent()
	{
		GENESIS_ASSERT((std::is_base_of<Component, T>::value), "T must derive from Component.");
		TypeId id = typeid(T).hash_code();

		auto it = m_components.find(id);
		if (it == m_components.end()) {
			return;
		}

		m_manager.unregisterComponent(id, it->second.get());
		m_components.erase(it);
	}

	template<typename T>
	inline T* Entity::getComponent()
	{
		GENESIS_ASSERT((std::is_base_of<Component, T>::value), "T must derive from Component.");
		TypeId id = typeid(T).hash_code();

		auto it = m_components.find(id);
		if (it == m_components.end()) {
			return nullptr;
		}

		return static_cast<T*>(it->second.get());
	}
}