#ifndef GENESIS_VERTEX_SHADER_H
#define GENESIS_VERTEX_SHADER_H
#include <resources/Resource.h>

namespace genesis
{
	class VertexShader final: public Resource
	{
	public:
		VertexShader(const ResourceDesc& desc);
		~VertexShader() override;

		VertexShaderSignature& getVertexShaderSignature();

	private:
		void onLoad() override;
		void onUnload() override;

	private:
		SharedPtr<VertexShaderSignature> m_vertexShader;
		std::string m_entryPoint;
	};
}


#endif