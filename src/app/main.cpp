#include <iostream>
#include <debug/debug.hpp>

int main() {
	engine::debug::initTerminal();
    engine::debug::addHandler([](const std::string& msg) { std::cout << msg << "\n"; });
    
	ENGINE_LOG_INFO("This is an info message.");
	ENGINE_LOG_DEBUG("This is a debug message.");
	ENGINE_LOG_WARN("This is a warning message.");
	ENGINE_LOG_ERROR("This is an error message.");
	ENGINE_LOG_FATAL("This is a fatal error message.");
	
	return 0;
}