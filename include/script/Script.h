#ifndef GENESIS_SCRIPT_H
#define GENESIS_SCRIPT_H
#include <core/Core.h>
#include <core/Base.h>
#include <core/utils/Types.h>
#include <entity/Entity.h>

GENESIS_NAMESPACE_BEGIN

struct ScriptDesc
{
	BaseDesc base;
	ScriptManager& manager;
	ScriptContext context;
};

class Script: public Base
{
public:
	virtual ~Script() override;

	void start();
	void update(float deltaTime);

	bool hasStarted() const noexcept;
	Entity* getEntity() noexcept;

protected:
	explicit Script(const ScriptDesc& desc);

	virtual void onAwake();
	virtual void onStart();
	virtual void onUpdate(float deltaTime) = 0;

protected:
	ScriptContext m_context;

private:
	void assignEntity(Entity& entity);
	void unassignEntity();

private:
	ScriptManager& m_manager;
	Entity* m_entity;
	bool m_started;

	friend class ScriptComponent;
};

GENESIS_NAMESPACE_END

#endif