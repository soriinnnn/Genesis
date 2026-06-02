#ifndef GENESIS_CUBE_TEXTURE_H
#define GENESIS_CUBE_TEXTURE_H
#include <graphics/resources/GraphicsResource.h>
#include <math/Rect.h>

GENESIS_NAMESPACE_BEGIN

struct CubeTextureDesc
{
	const char* cubeFaces[6]; // order: +X, -X, +Y, -Y, +Z, -Z
};

class CubeTexture final: public GraphicsResource
{
public:
	CubeTexture(const CubeTextureDesc& tdesc, const GraphicsResourceDesc& gdesc);
	~CubeTexture() override;

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_resourceView;
	Rect m_size;

	friend class DeviceContext;
};

GENESIS_NAMESPACE_END

#endif