#ifndef GENESIS_TEST_WORLD_H
#define GENESIS_TEST_WORLD_H
#include <game/World.h>

namespace genesis
{
	class TestWorld final: public World
	{
	public:
		explicit TestWorld(const WorldDesc& desc);
		~TestWorld() override;

		void update(float deltaTime) override;

	private:
		Player* m_player;
		Entity* m_statue;
	};
}

#endif