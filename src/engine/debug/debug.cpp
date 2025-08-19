#include "debug.hpp"

namespace engine::debug {
	namespace detail {
		std::vector<Handler> handlers;
		std::mutex handlersMutex;

		const char* levelToString(Level level)
		{
#ifdef _WIN32
			switch (level) {
				case Level::Info:  return "[ Info  ]";
				case Level::Debug: return "[ Debug ]";
				case Level::Warn:  return "[ Warn  ]";
				case Level::Error: return "[ Error ]";
				case Level::Fatal: return "[ Fatal ]";
			}
			return "[\e[1;37m   ?   \e[0m]";
#else
			switch (level) {
				case Level::Info:  return "[\e[1;37m Info  \e[0m]";
				case Level::Debug: return "[\e[1;32m Debug \e[0m]";
				case Level::Warn:  return "[\e[1;33m Warn  \e[0m]";
				case Level::Error: return "[\e[1;31m Error \e[0m]";
				case Level::Fatal: return "[\e[1;91m Fatal \e[0m]";
			}
			return "[   ?   ]";
#endif
		}
	}

	void addHandler(Handler handler) {
		std::lock_guard<std::mutex> lock(detail::handlersMutex);
		detail::handlers.push_back(std::move(handler));
	}

	void log(Level level, const std::string& msg) {
		std::string formatted = std::string(detail::levelToString(level)) + " " + msg;
		std::lock_guard<std::mutex> lock(detail::handlersMutex);
		for (auto& handler : detail::handlers) {
			handler(formatted);
		}
	}

	void log(Level level, const char* file, int line, const std::string& msg) {
		std::string formatted = "(" + std::string(file) + ":" + std::to_string(line) + ") " + msg;
		debug::log(level, formatted);
	}
}
