#ifndef GENESIS_SCRIPT_MANAGER_H
#define GENESIS_SCRIPT_MANAGER_H
#include <core/Core.h>
#include <core/Base.h>
#include <core/utils/Types.h>
#include <script/Script.h>

GENESIS_NAMESPACE_BEGIN

struct ScriptManagerDesc
{
	BaseDesc base;
	ScriptContext context;
};

class ScriptManager final: public Base
{
public:
	explicit ScriptManager(const ScriptManagerDesc& desc);
	~ScriptManager() override;

	void update(float deltaTime);
	void fixedUpdate(float deltaTime);

	template<typename T>
	T* createScript(uint32 priority = 0);

private:
	struct ScriptInstance
	{
		UniquePtr<Script> script;
		uint32 priority;
	};

	enum class EventType
	{
		SetActive,
		SetInactive
	};

	struct ScriptEvent
	{
		EventType type;
		Script* script;
	};

private:
	void setActive(Script* script);
	void setInactive(Script* script);
	void insertActive(ScriptInstance instance);
	void processEvents();
	void deleteInactive();

	void onSetActive(ScriptEvent event);
	void onSetInactive(ScriptEvent event);

private:
	Vector<ScriptInstance> m_activeScripts;
	Vector<ScriptInstance> m_inactiveScripts;
	Vector<ScriptEvent> m_events;
	ScriptContext m_context;

	friend class Script;
};

GENESIS_NAMESPACE_END

#include <script/ScriptManager.inl>
#endif