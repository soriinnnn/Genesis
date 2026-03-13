#ifndef GENESIS_PIXEL_SHADER_H
#define GENESIS_PIXEL_SHADER_H
#include <resources/Resource.h>

namespace genesis
{
	struct PixelShaderDesc
	{
		GraphicsDevice& graphicsDevice;
		const char* entryPoint;
	};

	class PixelShader final: public Resource
	{
	public:
		PixelShader(const PixelShaderDesc& sdesc, const ResourceDesc& rdesc);
		~PixelShader() override;

		ShaderBinary& getShaderBinary();

	private:
		void onLoad() override;
		void onUnload() override;

	private:
		GraphicsDevice& m_graphicsDevice;
		SharedPtr<ShaderBinary> m_pixelShader;
		std::string m_entryPoint;
	};
}

#endif