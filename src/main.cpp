#include "Engine.hpp"

int main() {
	KQ::Engine engine;

	try {
		engine.Run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}