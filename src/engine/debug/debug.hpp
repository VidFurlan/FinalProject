#include <functional>
#include <string>
#include <mutex>

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

		const char* levelToString(Level level);
	}

	void initTerminal();
	void addHandler(Handler handler);
	void log(Level level, const std::string& msg);
	void log(Level level, const char* file, int line, const std::string &msg);

	#define ENGINE_LOG_DEBUG(msg) engine::debug::log(engine::debug::Level::Debug, __FILE__, __LINE__, msg)
	#define ENGINE_LOG_INFO(msg)  engine::debug::log(engine::debug::Level::Info,  __FILE__, __LINE__, msg)
	#define ENGINE_LOG_WARN(msg)  engine::debug::log(engine::debug::Level::Warn,  __FILE__, __LINE__, msg)
	#define ENGINE_LOG_ERROR(msg) engine::debug::log(engine::debug::Level::Error, __FILE__, __LINE__, msg)
	#define ENGINE_LOG_FATAL(msg) engine::debug::log(engine::debug::Level::Fatal, __FILE__, __LINE__, msg) 
}