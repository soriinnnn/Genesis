#ifndef GENESIS_VERTEX_SHADER_H
#define GENESIS_VERTEX_SHADER_H
#include <resources/Resource.h>

namespace genesis
{
	struct VertexShaderDesc
	{
		GraphicsDevice& graphicsDevice;
		const char* entryPoint;
	};

	class VertexShader final: public Resource
	{
	public:
		VertexShader(const VertexShaderDesc& vdesc, const ResourceDesc& rdesc);
		~VertexShader() override;

		VertexShaderSignature& getVertexShaderSignature();

	private:
		void onLoad() override;
		void onUnload() override;

	private:
		GraphicsDevice& m_graphicsDevice;
		SharedPtr<VertexShaderSignature> m_vertexShader;
		std::string m_entryPoint;
	};
}


#endif