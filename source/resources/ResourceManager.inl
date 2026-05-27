#include <resources/ResourceManager.h>

GENESIS_NAMESPACE_BEGIN

template<typename T>
inline SharedPtr<T> ResourceManager::getResource(ResourceId id)
{
	auto it = m_resources.find(id);
	if (it == m_resources.end()) {
		return nullptr;
	}
	return dynamic_pointer_cast<T>(it->second);
}

template<typename T, typename D>
inline SharedPtr<T> ResourceManager::createResource(ResourceId id, const D& desc)
{
	SharedPtr<T> resource = std::make_shared<T>(desc);
	m_resources.emplace(id, resource);
	return resource;
}

GENESIS_NAMESPACE_END