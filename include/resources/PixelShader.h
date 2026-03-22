#ifndef GENESIS_PIXEL_SHADER_H
#define GENESIS_PIXEL_SHADER_H
#include <resources/Resource.h>

namespace genesis
{
	struct PixelShaderDesc
	{
		ResourceDesc resource;
		const char* entry;
	};

	class PixelShader final: public Resource
	{
	public:
		explicit PixelShader(const PixelShaderDesc& desc);
		~PixelShader() override;

		PixelShaderSignature& getSignature() noexcept;

	private:
		SharedPtr<PixelShaderSignature> m_pixelShader;
	};
}

#endif