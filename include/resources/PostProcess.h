#ifndef GENESIS_POST_PROCESS_H
#define GENESIS_POST_PROCESS_H
#include <resources/Resource.h>
#include <graphics/utils/GraphicsTypes.h>

namespace genesis
{
	struct PostProcessDesc
	{
		ResourceDesc resource;
	};

	class PostProcess final: public Resource
	{
	public:
		explicit PostProcess(const PostProcessDesc& desc);
		~PostProcess() override;

		bool hasProperties() const noexcept;
		bool isDirty() const noexcept;

		void clearDirty() noexcept;
		GraphicsPipelineState& getGraphicsPipelineState() noexcept;
		ConstantBuffer& getProperties();
		uint8* getData();

		void setProperty(const char* name, int value);
		void setProperty(const char* name, float value);
		void setProperty(const char* name, const Vec2& value);
		void setProperty(const char* name, const Vec3& value);
		void setProperty(const char* name, const Vec4& value);

	private:
		template<typename T>
		void setPropertyT(const char* name, const T& value) {
			auto it = m_variables.find(name);
			if (it == m_variables.end()) {
				GENESIS_LOG_WARNING("Post processing effect \"{}\" has no property \"{}\".", getPath(), name);
				return;
			}
			auto& variable = it->second;
			if (sizeof(T) > variable.size) {
				GENESIS_LOG_WARNING("Post processing effect \"{}\" property \"{}\" size mismatch.", getPath(), name);
				return;
			}
			memcpy(m_data.data() + variable.offset, &value, sizeof(T));
			m_isDirty = true;
		}

	private:
		SharedPtr<GraphicsPipelineState> m_pipeline;
		SharedPtr<ConstantBuffer> m_properties;
		Vector<uint8> m_data;
		HashMap<String, ShaderReflectionVariable> m_variables;
		bool m_isDirty;
	};
}

#endif