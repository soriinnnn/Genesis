#ifndef GENESIS_IMAGE_TEXTURE_H
#define GENESIS_IMAGE_TEXTURE_H
#include <graphics/resources/GraphicsResource.h>
#include <math/Rect.h>
#include <math/Vec4.h>

GENESIS_NAMESPACE_BEGIN

struct ImageTextureFileDesc
{
	const char* path{};
	bool generateMipMaps{true};
};

struct ImageTextureSolidDesc
{
	Vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	Rect size = {1, 1};
};

class ImageTexture final: public GraphicsResource
{
public:
	ImageTexture(const ImageTextureFileDesc& tdesc, const GraphicsResourceDesc& gdesc);
	ImageTexture(const ImageTextureSolidDesc& tdesc, const GraphicsResourceDesc& gdesc);
	~ImageTexture() override;

	Rect getSize() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_resourceView;
	Rect m_size;

	friend class DeviceContext;
	friend class SpriteBatch;
};

GENESIS_NAMESPACE_END

#endif
