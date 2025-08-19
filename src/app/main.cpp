#include "Application.hpp"
#include <debug/Debug.hpp>
#include <iostream>

int main() {
	engine::debug::addHandler([](const std::string &msg) { std::cout << msg << "\n"; });

	Application app;
	app.run();
	app.cleanup();

	return 0;
}
