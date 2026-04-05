#ifndef GENESIS_UI_MANAGER_H
#define GENESIS_UI_MANAGER_H
#include <core/Base.h>
#include <core/Core.h>
#include <ui/elements/UIElement.h>
#include <input/InputListener.h>
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
		T* createElement()
		{	
			GENESIS_ASSERT((std::is_base_of<UIElement, T>::value), "T must derive from UIElement.");
			UniquePtr<T> element = std::make_unique<T>(UIElementDesc{m_logger});
			T* result = element.get();
			m_elements.push_back(std::move(element));
			return result;
		}
		// template<typename T>
		// T* getElement();
		void destroyElement(UIElement* element);

		template<typename F>
		void forEach(F&& callback) const
		{
			for (const auto& element : m_elements) {
				callback(*element);
			}
		}

		template<typename F>
		void forEach(F&& callback)
		{
			for (auto& element : m_elements) {
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
		Vector<UniquePtr<UIElement>> m_elements;
		UIElement* m_pressedElement;
	};
}

#endif