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
	if (!m_texture) {
		if (m_autoSize) {
			m_size = Rect{};
		}
		return;
	}
	if (m_autoSize) {
		m_size = m_texture->getImageTexture().getSize();
	}
}

void UIImage::onRender(SpriteBatch& batch)
{
	if (!m_texture) {
		return;
	}
	batch.drawImage(m_texture->getImageTexture(), m_size, getGlobalPosition(), m_scale, m_color);
}

void UIImage::onSize()
{
	m_autoSize = false;
}