#ifndef GENESIS_ENTITY_H
#define GENESIS_ENTITY_H
#include <core/Base.h>
#include <core/Core.h>

GENESIS_NAMESPACE_START

struct EntityDesc
{
	BaseDesc base;
	EntityId id;
	const char* name;
	EntityManager& manager;
};

class Entity final: public Base
{
public:
	explicit Entity(const EntityDesc& desc);
	~Entity() override;

	void update(float deltaTime);

	EntityId getId() const noexcept;
	const char* getName() const noexcept;

	template<typename T>
	bool hasComponent() const;

	template<typename T>
	T* createComponent();

	template<typename T>
	void deleteComponent();

	template<typename T>
	T* getComponent();

private:
	EntityId m_id;
	String m_name;
	EntityManager& m_manager;
	HashMap<TypeId, UniquePtr<Component>> m_components;
};

GENESIS_NAMESPACE_END

#include <entity/Entity.inl>
#endif