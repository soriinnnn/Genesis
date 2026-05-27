#ifndef GENESIS_SCRIPT_COMPONENT_H
#define GENESIS_SCRIPT_COMPONENT_H
#include <entity/components/Component.h>

GENESIS_NAMESPACE_BEGIN

class ScriptComponent final: public Component
{
public:
	explicit ScriptComponent(const ComponentDesc& desc);
	~ScriptComponent() override;

	const Vector<Script*>& getScripts() const;

	void addScript(Script* script);
	void removeScript(Script* script);
	void clearScripts();

private:
	Vector<Script*> m_scripts;
};

GENESIS_NAMESPACE_END

#endif