#ifndef GENESIS_UI_MANAGER_H
#define GENESIS_UI_MANAGER_H
#include <core/Base.h>
#include <core/Core.h>
#include <input/InputListener.h>
#include <ui/elements/UIElement.h>
#include <graphics/utils/GraphicsTypes.h>

namespace genesis
{
	struct UIManagerDesc
	{
		BaseDesc base;
	};

	class UIManager final: public Base, public InputListener
	{
	public:
		explicit UIManager(const UIManagerDesc& desc);
		~UIManager() override;

		void update(float deltaTime);

		template<typename T>
		T* createElement(const char* name)
		{	
			GENESIS_ASSERT((std::is_base_of<UIElement, T>::value), "T must derive from UIElement.");
			T* result = getElement<T>(name);
			if (!result) {
				UniquePtr<T> element = std::make_unique<T>(UIElementDesc{m_logger, *this});
				result = element.get();
				m_elements.emplace(name, std::move(element));
				m_zOrdered.push_back(result);
				m_isZDirty = true;
			}
			return result;
		}

		template<typename T>
		T* getElement(const char* name)
		{
			auto it = m_elements.find(name);
			if (it == m_elements.end()) {
				return nullptr;
			}
			return dynamic_cast<T*>(it->second.get());
		}

		void destroyElement(const char* name);

		template<typename F>
		void forEach(F&& callback) const
		{
			for (const auto* element : m_zOrdered) {
				callback(*element);
			}
		}

		template<typename F>
		void forEach(F&& callback)
		{
			for (auto* element : m_zOrdered) {
				callback(*element);
			}
		}

	public:
		void onKeyDown(Key key) override;
		void onKeyUp(Key key) override;
		void onMouseMove(Point delta, Point pos) override;
		void onMouseDown(MouseButton button, Point pos) override;
		void onMouseUp(MouseButton button, Point pos) override;

	private:
		HashMap<String, UniquePtr<UIElement>> m_elements;
		UIElement* m_pressedElement;
		Vector<UIElement*> m_zOrdered;
		bool m_isZDirty;

		friend class UIElement;
	};
}

#endif