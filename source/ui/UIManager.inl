#include <ui/UIManager.h>

namespace genesis
{
	template<typename T>
	inline T* UIManager::createElement(const char* name)
	{
		GENESIS_ASSERT((std::is_base_of<UIElement, T>::value), "T must derive from UIElement.");

		T* result = getElement<T>(name);
		if (!result) {
			UniquePtr<T> element = std::make_unique<T>(UIElementDesc{m_logger});
			result = element.get();
			auto [it, inserted] = m_elements.emplace(name, RootElement{std::move(element)});
			m_zOrdered.push_back(&it->second);
			m_isZDirty = true;
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
}