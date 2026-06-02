#include <ui/elements/UIImage.h>
#include <resources/Texture.h>
#include <graphics/resources/SpriteBatch.h>
#include <graphics/resources/ImageTexture.h>

using namespace genesis;

UIImage::UIImage(const UIElementDesc& desc): UIElement(desc), m_autoSize{true} {}

UIImage::~UIImage() {}

void UIImage::setTexture(const SharedPtr<Texture>& texture) noexcept
{
	m_texture = texture;
	applyAutoSize();
}

void UIImage::setAutoSize(bool enable) noexcept
{
	m_autoSize = enable;
	applyAutoSize();
}

void UIImage::applyAutoSize()
{
	if (!m_autoSize) {
		return;
	}
	if (!m_texture) {
		m_sourceTexture = Rect{0, 0};
	}
	else {
		m_sourceTexture = m_texture->getImageTexture().getSize();
	}
	updateSize();
}

void UIImage::updateSize()
{
	m_size = Rect{
		static_cast<int32>(round(m_sourceTexture.width() * m_scale.x)), 
		static_cast<int32>(round(m_sourceTexture.height() * m_scale.y))
	};
	updateRelativeLayout();
}

void UIImage::onRender(SpriteBatch& batch)
{
	if (!m_texture) {
		return;
	}
	batch.drawImage(m_texture->getImageTexture(), m_sourceTexture, getGlobalPosition(), m_scale, m_color);
}

void UIImage::onSize()
{
	m_autoSize = false;
	m_sourceTexture = m_size;
	m_size = Rect{
		static_cast<int32>(round(m_size.width() * m_scale.x)), 
		static_cast<int32>(round(m_size.height() * m_scale.y))
	};
}

void UIImage::onScale()
{
	updateSize();
}