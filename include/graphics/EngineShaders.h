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
		struct EngineShader
		{
			SharedPtr<ShaderBinary> vsBinary;
			SharedPtr<ShaderBinary> psBinary;
			SharedPtr<ShaderSignature> vsSignature;
			SharedPtr<ShaderSignature> psSignature;
		};

	public:
		explicit EngineShaders(const EngineShadersDesc& desc);
		~EngineShaders() override;

		const EngineShader& getFullscreenTriangle() const;
		const EngineShader& getDebugLine() const;

	private:
		EngineShader m_fullscreenTriangle;
		EngineShader m_debugLine;
	};
}

#endif