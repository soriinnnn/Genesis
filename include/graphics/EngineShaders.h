#ifndef GENESIS_ENGINE_SHADERS_H
#define GENESIS_ENGINE_SHADERS_H
#include <core/Base.h>
#include <core/Core.h>

namespace genesis
{
	struct EngineShadersDesc
	{
		BaseDesc base;
		GraphicsDevice& graphicsDevice;
	};

	class EngineShaders final: public Base
	{
	public:
		explicit EngineShaders(const EngineShadersDesc& desc);
		~EngineShaders() override;

		const ShaderBinary& getFullscreenVSBinary() const;
		const ShaderBinary& getFullscreenPSBinary() const;
		const ShaderSignature& getFullscreenVSSignature() const;
		const ShaderSignature& getFullscreenPSSignature() const;

		const ShaderBinary& getDebugLineVSBinary() const;
		const ShaderBinary& getDebugLinePSBinary() const;
		const ShaderSignature& getDebugLineVSSignature() const;
		const ShaderSignature& getDebugLinePSSignature() const;

	private:
		struct EngineShader
		{
			SharedPtr<ShaderBinary> binary;
			SharedPtr<ShaderSignature> signature;
		};

	private:
		EngineShader m_fullscreenVS;
		EngineShader m_fullscreenPS;
		EngineShader m_debugLineVS;
		EngineShader m_debugLinePS;
	};
}

#endif