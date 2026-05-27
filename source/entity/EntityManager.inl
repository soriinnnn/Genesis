#include <entity/EntityManager.h>

GENESIS_NAMESPACE_BEGIN

template<typename F>
inline void EntityManager::forEach(F&& callback) const
{
	for (const auto& [key, entity] : m_entities) {
		callback(*entity);
	}
}

template<typename F>
inline void EntityManager::forEach(F&& callback)
{
	for (auto& [key, entity] : m_entities) {
		callback(*entity);
	}
}

template<typename T, typename F>
inline void EntityManager::forEachComponent(F&& callback) const
{
	GENESIS_ASSERT((std::is_base_of<Component, T>::value), "T must derive from Component.");

	auto it = m_components.find(typeid(T).hash_code());
	if (it == m_components.end()) {
		return;
	}

	for (const Component* component : it->second) {
		callback(*static_cast<const T*>(component));
	}
}

template<typename T, typename F>
inline void EntityManager::forEachComponent(F&& callback)
{
	GENESIS_ASSERT((std::is_base_of<Component, T>::value), "T must derive from Component.");

	auto it = m_components.find(typeid(T).hash_code());
	if (it == m_components.end()) {
		return;
	}

	for (Component* component : it->second) {
		callback(*static_cast<T*>(component));
	}
}

GENESIS_NAMESPACE_END