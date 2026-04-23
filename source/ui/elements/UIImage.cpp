#include <ui/elements/UIImage.h>
#include <resources/Texture.h>
#include <graphics/resources/SpriteBatch.h>
#include <graphics/resources/ImageTexture.h>

using namespace genesis;

UIImage::UIImage(const UIElementDesc& desc): UIElement(desc) {}

UIImage::~UIImage() {}

void UIImage::render(SpriteBatch& batch) 
{
	if (!m_texture) {
		return;
	}
	batch.drawImage(m_texture->getImageTexture(), m_size, getGlobalPosition(), m_scale, m_color);
}

void UIImage::setTexture(SharedPtr<Texture> texture) noexcept
{
	m_texture = texture;
	if (!texture) {
		m_size = Rect{};
		return;
	}
	m_size = texture->getImageTexture().getSize();
}