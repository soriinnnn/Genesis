#include <entity/components/ScriptComponent.h>
#include <script/Script.h>

using namespace genesis;
using namespace std;

ScriptComponent::ScriptComponent(const ComponentDesc& desc): Component(desc) {}

ScriptComponent::~ScriptComponent() 
{
	clearScripts();
}

const Vector<Script*>& ScriptComponent::getScripts() const
{
	return m_scripts;
}

void ScriptComponent::addScript(Script* script)
{
	if (!script) {
		GENESIS_LOG_WARNING("Attempted to add a null script to an entity.");
		return;
	}
	if (find(m_scripts.begin(), m_scripts.end(), script) != m_scripts.end()) {
		GENESIS_LOG_WARNING("Script already added to entity.");
		return;
	}
	if (script->m_entity != nullptr) {
		GENESIS_LOG_WARNING("Script is already assigned to another entity.");
		return;
	}
	script->assignEntity(m_entity);
	m_scripts.push_back(script);
}

void ScriptComponent::removeScript(Script* script)
{
	if (!script) {
		GENESIS_LOG_WARNING("Attempted to remove a null script from an entity.");
		return;
	}
	auto it = find(m_scripts.begin(), m_scripts.end(), script);
	if (it == m_scripts.end()) {
		GENESIS_LOG_WARNING("Attempted to remove a script that was not found on the entity.");
		return;
	}
	script->unassignEntity();
	*it = m_scripts.back();
	m_scripts.pop_back();
}

void ScriptComponent::clearScripts()
{
	for (auto& script : m_scripts) {
		script->unassignEntity();
	}
	m_scripts.clear();
}