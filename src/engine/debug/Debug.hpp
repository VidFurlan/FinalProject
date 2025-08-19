#pragma once

#include <functional>
#include <mutex>
#include <string>

namespace engine::debug {
	enum class Level {
		Info,
		Debug,
		Warn,
		Error,
		Fatal
	};

	using Handler = std::function<void(const std::string &)>;

	namespace detail {
		extern std::vector<Handler> handlers;
		extern std::mutex handlersMutex;

		const char *levelToString(Level level);
	} // namespace detail

	void initTerminal();
	void addHandler(Handler handler);
	void log(Level level, const std::string &msg);
	void log(Level level, const char *file, int line, const std::string &msg);

#ifdef NDEBUG
#define ENGINE_LOG_DEBUG(msg) (void)(0)
#else
#define ENGINE_LOG_DEBUG(msg) engine::debug::log(engine::debug::Level::Debug, __FILE__, __LINE__, msg)
#endif
#define ENGINE_LOG_INFO(msg) engine::debug::log(engine::debug::Level::Info, __FILE__, __LINE__, msg)
#define ENGINE_LOG_WARN(msg) engine::debug::log(engine::debug::Level::Warn, __FILE__, __LINE__, msg)
#define ENGINE_LOG_ERROR(msg) engine::debug::log(engine::debug::Level::Error, __FILE__, __LINE__, msg)
#define ENGINE_LOG_FATAL(msg) engine::debug::log(engine::debug::Level::Fatal, __FILE__, __LINE__, msg)
} // namespace engine::debug
