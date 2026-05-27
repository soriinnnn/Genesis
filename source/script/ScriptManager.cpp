#include <script/ScriptManager.h>
#include <algorithm>

using namespace genesis;
using namespace std;

ScriptManager::ScriptManager(const ScriptManagerDesc& desc): Base(desc.base), m_context{desc.context} {}

ScriptManager::~ScriptManager() {}

void ScriptManager::update(float deltaTime)
{
	processEvents();
	deleteInactive();
	for (auto& instance : m_activeScripts) {
		instance.script->update(deltaTime);
	}
}

void ScriptManager::setActive(Script* script)
{
	GENESIS_ASSERT(script != nullptr, "Attempted to set a null script active.");
	m_events.push_back({EventType::SetActive, script});
}

void ScriptManager::setInactive(Script* script)
{
	GENESIS_ASSERT(script != nullptr, "Attempted to set a null script inactive.");
	m_events.push_back({EventType::SetInactive, script});
}

void ScriptManager::insertActive(ScriptInstance instance)
{
	auto it = lower_bound(m_activeScripts.begin(), m_activeScripts.end(), instance,
		[](const ScriptInstance& a, const ScriptInstance& b) {
			return a.priority < b.priority;
		}
	);
	m_activeScripts.insert(it, move(instance));
}

void ScriptManager::processEvents()
{
	for (auto& event : m_events) {
		switch (event.type) {
			case EventType::SetActive: {
				onSetActive(event);
				break;
			}
			case EventType::SetInactive: {
				onSetInactive(event);
				break;
			}
		}
	}
	m_events.clear();
}

void ScriptManager::deleteInactive()
{
	m_inactiveScripts.clear();
}

void ScriptManager::onSetActive(ScriptEvent event)
{
	Script* script = event.script;

	auto it = find_if(m_inactiveScripts.begin(), m_inactiveScripts.end(),
		[script](const ScriptInstance& instance) {
		return instance.script.get() == script;
	}
	);
	if (it == m_inactiveScripts.end()) {
		return; // already active.
	}

	insertActive(move(*it));
	script->start();

	*it = move(m_inactiveScripts.back());
	m_inactiveScripts.pop_back();
}

void ScriptManager::onSetInactive(ScriptEvent event)
{
	Script* script = event.script;

	auto it = find_if(m_activeScripts.begin(), m_activeScripts.end(),
		[script](const ScriptInstance& instance) {
		return instance.script.get() == script;
	}
	);
	if (it == m_activeScripts.end()) {
		return; // already inactive.
	}

	m_inactiveScripts.push_back(move(*it));
	m_activeScripts.erase(it);
}