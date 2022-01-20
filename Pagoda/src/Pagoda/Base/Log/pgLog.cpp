#include "pgpch.h"
#include "pgLog.h"

namespace Pagoda::Base {
	// Bring into scope from header file.
	std::shared_ptr<spdlog::logger> CLog::s_CoreLogger;
	std::shared_ptr<spdlog::logger> CLog::s_ClientLogger;

	void CLog::Init() {
		// Format logger output.
		spdlog::set_pattern("%^[%T] %n: %v%$");

		// Initialize loggers with names.
		CLog::s_CoreLogger = spdlog::stdout_color_mt("PAGODA");
		// Output every log type.
		s_CoreLogger->set_level(spdlog::level::trace);

		CLog::s_ClientLogger = spdlog::stdout_color_mt("CLIENT");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	void CLog::SetClientName(std::string name) {
		std::transform(name.begin(), name.end(), name.begin(), ::toupper);
		CLog::s_ClientLogger = spdlog::stdout_color_mt(name);
	}
}