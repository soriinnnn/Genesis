#include <script/ScriptManager.h>

GENESIS_NAMESPACE_BEGIN

template<typename T>
inline T* ScriptManager::createScript(uint32 priority)
{
	GENESIS_ASSERT((std::is_base_of<Script, T>::value), "T must derive from Script.");

	UniquePtr<T> script = std::make_unique<T>(ScriptDesc{m_logger, *this, m_context});
	T* result = script.get();

	ScriptInstance instance{};
	instance.script = std::move(script);
	instance.priority = priority;

	m_inactiveScripts.push_back(std::move(instance));

	return result;
}

GENESIS_NAMESPACE_END