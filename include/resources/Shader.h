#ifndef GENESIS_SHADER_H
#define GENESIS_SHADER_H
#include <resources/Resource.h>
#include <graphics/utils/GraphicsTypes.h>

namespace genesis
{
	struct ShaderDesc
	{
		ResourceDesc resource;
		const char* entry;
		ShaderType type;
	};

	class Shader final: public Resource
	{
	public:
		explicit Shader(const ShaderDesc& desc);
		~Shader() override;

		ShaderType getType() const noexcept;

		ShaderBinary& getBinary() noexcept;
		ShaderSignature& getSignature() noexcept;

	private:
		SharedPtr<ShaderBinary> m_binary;
		SharedPtr<ShaderSignature> m_signature;
		ShaderType m_type;
	};
}

#endif