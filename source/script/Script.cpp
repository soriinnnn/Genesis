#include <script/Script.h>
#include <script/ScriptManager.h>
#include <entity/components/ScriptComponent.h>

using namespace genesis;

Script::Script(const ScriptDesc& desc): Base(desc.base), m_manager{desc.manager}, m_context{desc.context}
{
	m_entity = nullptr;
	m_started = false;
}

Script::~Script() 
{
	if (m_entity) {
		ScriptComponent* scripts = m_entity->getComponent<ScriptComponent>();
		scripts->removeScript(this);
	}
}

void Script::start()
{
	if (m_started) {
		GENESIS_LOG_WARNING("Script has already started.");
		return;
	}
	m_started = true;
	onStart();
}

void Script::update(float deltaTime)
{
	onUpdate(deltaTime);
}

bool Script::hasStarted() const noexcept
{
	return m_started;
}

Entity* Script::getEntity() noexcept
{
	return m_entity;
}

void Script::onAwake() {}

void Script::onStart() {}

void Script::assignEntity(Entity& entity)
{
	GENESIS_ASSERT(m_started != true, "Cannot assign entity after script has started.");
	m_entity = &entity;
	m_manager.setActive(this);
}

void Script::unassignEntity()
{
	GENESIS_ASSERT(m_entity != nullptr, "Attempted to unassign entity to an empty script.");
	m_entity = nullptr;
	m_manager.setInactive(this);
}