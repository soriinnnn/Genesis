#include <resources/PostProcess.h>

GENESIS_NAMESPACE_BEGIN

template<typename T>
inline void PostProcess::setPropertyT(const char* name, const T& value)
{
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

GENESIS_NAMESPACE_END