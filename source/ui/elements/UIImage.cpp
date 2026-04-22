#include <ui/elements/UIImage.h>
#include <resources/Texture.h>
#include <graphics/resources/SpriteBatch.h>
#include <graphics/resources/ImageTexture.h>

using namespace genesis;

UIImage::UIImage(const UIElementDesc& desc): UIElement(desc) 
{
	m_size = Rect{1, 1};
}

UIImage::~UIImage() {}

void UIImage::render(SpriteBatch& batch) 
{
	ImageTexture* image = (m_texture != nullptr) ? &m_texture->getImageTexture() : nullptr;
	batch.drawImage(image, getGlobalPosition(), m_scale, m_color);
}

void UIImage::setTexture(SharedPtr<Texture> texture) noexcept
{
	m_texture = texture;
	m_size = texture->getImageTexture().getSize();
}