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
		Rect canvasSize;
	};

	class UIManager final: public Base, public InputListener
	{
	public:
		explicit UIManager(const UIManagerDesc& desc);
		~UIManager() override;

		void update(float deltaTime);

		void setCanvasSize(const Rect& size);
		void setZOrder(const char* name, int zOrder);

		template<typename T>
		T* createElement(const char* name);

		template<typename T>
		T* getElement(const char* name);

		void destroyElement(const char* name);
		
		template<typename F>
		void forEach(F&& callback) const;
		
		template<typename F>
		void forEach(F&& callback);

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
		UniquePtr<UIElement> m_canvas;
		HashMap<String, RootElement> m_elements;
		UIElement* m_pressedElement;
		Vector<RootElement*> m_zOrdered;
		bool m_isZDirty;
	};
}

#include <ui/UIManager.inl>
#endif