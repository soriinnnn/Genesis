#ifndef GENESIS_VERTEX_SHADER_H
#define GENESIS_VERTEX_SHADER_H
#include <resources/Resource.h>

namespace genesis
{
	struct VertexShaderDesc
	{
		ResourceDesc resource;
		const char* entry;
	};

	class VertexShader final: public Resource
	{
	public:
		explicit VertexShader(const VertexShaderDesc& desc);
		~VertexShader() override;

		VertexShaderSignature& getSignature() noexcept;

	private:
		SharedPtr<VertexShaderSignature> m_vertexShader;
	};
}


#endif