#include <game/Game.h>
#include <math/Mat4.h>

using namespace genesis;

int main() 
{
	try {
		Game game(GameDesc{.logLevel = Logger::LogLevel::Info});
		game.run();
	}
	catch (const std::runtime_error&) {
		return EXIT_FAILURE;
	}
	catch (const std::invalid_argument&) {
		return EXIT_FAILURE;
	}
	catch (const std::exception&) {
		return EXIT_FAILURE;
	}
	catch (...) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}