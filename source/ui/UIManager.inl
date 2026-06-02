#include <ui/UIManager.h>

GENESIS_NAMESPACE_BEGIN

template<typename T>
inline T* UIManager::createElement(const char* name)
{
	GENESIS_ASSERT((std::is_base_of<UIElement, T>::value), "T must derive from UIElement.");

	T* result = getElement<T>(name);
	if (!result) {
		UniquePtr<T> element = std::make_unique<T>(UIElementDesc{m_logger, m_canvas.get()});
		result = element.get();
		auto [it, inserted] = m_elements.emplace(name, RootElement{std::move(element)});
		m_zOrdered.push_back(&it->second);
		m_isZDirty = true;
	}
	else {
		GENESIS_LOG_WARNING("User interface element with name \"{}\" already exists. Returning existing element.", name);
	}

	return result;
}

template<typename T>
inline T* UIManager::getElement(const char* name)
{
	auto it = m_elements.find(name);
	if (it == m_elements.end()) {
		return nullptr;
	}

	auto& root = it->second;
	return dynamic_cast<T*>(root.element.get());
}

template<typename F>
inline void UIManager::forEach(F&& callback) const
{
	for (const auto* root : m_zOrdered) {
		callback(*root->element);
	}
}

template<typename F>
inline void UIManager::forEach(F&& callback)
{
	for (auto* root : m_zOrdered) {
		callback(*root->element);
	}
}

GENESIS_NAMESPACE_END