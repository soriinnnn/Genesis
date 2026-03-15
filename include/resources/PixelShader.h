#ifndef GENESIS_PIXEL_SHADER_H
#define GENESIS_PIXEL_SHADER_H
#include <resources/Resource.h>

namespace genesis
{
	class PixelShader final: public Resource
	{
	public:
		PixelShader(const ResourceDesc& desc);
		~PixelShader() override;

		ShaderBinary& getShaderBinary();

	private:
		void onLoad() override;
		void onUnload() override;

	private:
		SharedPtr<ShaderBinary> m_pixelShader;
		std::string m_entryPoint;
	};
}

#endif