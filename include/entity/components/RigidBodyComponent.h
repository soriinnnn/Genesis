#ifndef GENESIS_RIGID_BODY_COMPONENT_H
#define GENESIS_RIGID_BODY_COMPONENT_H
#include <entity/components/Component.h>

namespace genesis
{
	class RigidBodyComponent final: public Component
	{
	public:
		explicit RigidBodyComponent(const ComponentDesc& desc);
		~RigidBodyComponent() override;

		RigidBody* getBody();
		void setBody(const SharedPtr<RigidBody>& body);

	private:
		SharedPtr<RigidBody> m_body;
	};
}

#endif