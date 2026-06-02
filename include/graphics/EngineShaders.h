#ifndef GENESIS_ENGINE_SHADERS_H
#define GENESIS_ENGINE_SHADERS_H
#include <core/Base.h>
#include <core/Core.h>

GENESIS_NAMESPACE_BEGIN

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

	const EngineShader& fullscreenTriangle() const;
	const EngineShader& debugLine() const;
	const EngineShader& sphereSkybox() const;
	const EngineShader& cubeSkybox() const;

private:
	EngineShader m_fullscreenTriangle;
	EngineShader m_debugLine;
	EngineShader m_sphereSkybox;
	EngineShader m_cubeSkybox;
};

GENESIS_NAMESPACE_END

#endif