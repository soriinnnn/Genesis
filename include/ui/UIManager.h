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
		void render(SpriteBatch& batch);

		template<typename T>
		T* createElement(const char* name)
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
		T* getElement(const char* name)
		{
			auto it = m_elements.find(name);
			if (it == m_elements.end()) {
				return nullptr;
			}
			auto& root = it->second;
			return dynamic_cast<T*>(root.element.get());
		}

		void destroyElement(const char* name);
		void setZOrder(const char* name, int zOrder);

	public:
		void onKeyDown(Key key) override;
		void onKeyUp(Key key) override;
		void onMouseMove(Point delta, Point pos) override;
		void onMouseDown(MouseButton button, Point pos) override;
		void onMouseUp(MouseButton button, Point pos) override;

	private:
		struct RootElement
		{
			UniquePtr<UIElement> element;
			int zOrder{0};
		};

	private:
		HashMap<String, RootElement> m_elements;
		UIElement* m_pressedElement;
		Vector<RootElement*> m_zOrdered;
		bool m_isZDirty;
	};
}

#endif