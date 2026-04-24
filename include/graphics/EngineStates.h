#ifndef GENESIS_ENGINE_STATES_H
#define GENESIS_ENGINE_STATES_H
#include <core/Base.h>
#include <core/Core.h>

namespace genesis
{
	struct EngineStatesDesc
	{
		BaseDesc base;
		GraphicsDevice& graphicsDevice;
	};

	class EngineStates final: public Base
	{
	public:
		explicit EngineStates(const EngineStatesDesc& desc);
		~EngineStates() override;

		const SamplerState& getPointClamp() const;
		const SamplerState& getPointWrap() const;
		const SamplerState& getLinearClamp() const;
		const SamplerState& getLinearWrap() const;

	private:
		SharedPtr<SamplerState> m_pointClamp;
		SharedPtr<SamplerState> m_pointWrap;
		SharedPtr<SamplerState> m_linearClamp;
		SharedPtr<SamplerState> m_linearWrap;
	};
}

#endif