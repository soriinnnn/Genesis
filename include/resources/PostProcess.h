#ifndef GENESIS_POST_PROCESS_H
#define GENESIS_POST_PROCESS_H
#include <resources/Resource.h>
#include <graphics/utils/GraphicsTypes.h>
#include <core/utils/Types.h>

GENESIS_NAMESPACE_BEGIN

struct PostProcessDesc
{
	ResourceDesc resource;
};

class PostProcess final: public Resource
{
public:
	explicit PostProcess(const PostProcessDesc& desc);
	~PostProcess() override;

	bool isDirty() const noexcept;
	bool hasProperties() const noexcept;
	const GraphicsPipelineState& getGraphicsPipelineState() const noexcept;
	const ConstantBuffer& getProperties() const;
	BinaryData getData() const noexcept;

	void clearDirty() noexcept;
	void setProperty(const char* name, int value);
	void setProperty(const char* name, float value);
	void setProperty(const char* name, const Vec2& value);
	void setProperty(const char* name, const Vec3& value);
	void setProperty(const char* name, const Vec4& value);

private:
	template<typename T>
	void setPropertyT(const char* name, const T& value);

private:
	SharedPtr<GraphicsPipelineState> m_pipeline;
	SharedPtr<ConstantBuffer> m_properties;
	Vector<uint8> m_data;
	HashMap<String, ShaderReflectionVariable> m_variables;
	bool m_isDirty;
};

GENESIS_NAMESPACE_END

#include <resources/PostProcess.inl>
#endif