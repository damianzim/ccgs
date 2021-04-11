#ifndef CCGS_COMMON_HPP_
#define CCGS_COMMON_HPP_

#include <assert.h>
#include <stddef.h>

#ifdef NDEBUG
#define COMPILE_TIME_LOG_LEVEL SPDLOG_LEVEL_OFF
#else
#define DDEBUG  // Do debug
#define COMPILE_TIME_LOG_LEVEL SPDLOG_LEVEL_TRACE
#endif  // NDEBUG

#ifndef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL COMPILE_TIME_LOG_LEVEL
#endif  // SPDLOG_ACTIVE_LEVEL

#include <spdlog/spdlog.h>

#define LOGT spdlog::trace
#define LOGD spdlog::debug
#define LOGI spdlog::info
#define LOGW spdlog::warn
#define LOGE spdlog::error
#define LOGC spdlog::critical

using LogLevel = spdlog::level::level_enum;
void ConfigureLogger(LogLevel level = LogLevel::err);
std::shared_ptr<spdlog::logger> LogPlain();

#define ARR_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

struct Parser {
  static std::optional<int> ParseInt(const char* str);
  static std::optional<unsigned long> ParseULong(const char* str);
};

#endif  // CCGS_COMMON_HPP_
