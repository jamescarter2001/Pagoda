#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Pagoda/pgCore.h"

#ifdef PG_DEBUG

#define PG_CORE_INFO(...)::Pagoda::Base::CLog::GetCoreLogger()->info(__VA_ARGS__)
#define PG_CORE_TRACE(...)::Pagoda::Base::CLog::GetCoreLogger()->trace(__VA_ARGS__)
#define PG_CORE_WARNING(...)::Pagoda::Base::CLog::GetCoreLogger()->warn(__VA_ARGS__)
#define PG_CORE_ERROR(...)::Pagoda::Base::CLog::GetCoreLogger()->error(__VA_ARGS__)
#define PG_CORE_CRITICAL(...)::Pagoda::Base::CLog::GetCoreLogger()->critical(__VA_ARGS__)
#define PG_CORE_DEBUG(...)::Pagoda::Base::CLog::GetCoreLogger()->debug(__VA_ARGS__)

#define PG_INFO(...)::Pagoda::Base::CLog::GetClientLogger()->info(__VA_ARGS__)
#define PG_TRACE(...)::Pagoda::Base::CLog::GetClientLogger()->trace(__VA_ARGS__)
#define PG_WARNING(...)::Pagoda::Base::CLog::GetClientLogger()->warn(__VA_ARGS__)
#define PG_ERROR(...)::Pagoda::Base::CLog::GetClientLogger()->error(__VA_ARGS__)
#define PG_CRITICAL(...)::Pagoda::Base::CLog::GetClientLogger()->critical(__VA_ARGS__)

#else

#define PG_CORE_INFO
#define PG_CORE_TRACE
#define PG_CORE_WARNING
#define PG_CORE_ERROR
#define PG_CORE_CRITICAL
#define PG_CORE_DEBUG


#define PG_INFO
#define PG_TRACE
#define PG_WARNING
#define PG_ERROR
#define PG_CRITICAL

#endif

namespace Pagoda::Base {
	class PAGODA_API CLog {
	public:
		static void Init();
		static void SetClientName(std::string name);

		// Paste function at call, speeds up execution time.
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
			return s_CoreLogger;
		}

		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
			return s_ClientLogger;
		}

	private:
		// Loggers for both core and client.
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}