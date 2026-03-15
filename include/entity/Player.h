#ifndef GENESIS_PLAYER_H
#define GENESIS_PLAYER_H
#include <entity/Entity.h>

namespace genesis
{
	class Player final: public Entity
	{
	public:
		explicit Player(const EntityDesc& desc);
		~Player() override;

	private:

	};
}


#endif