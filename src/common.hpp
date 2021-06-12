#ifndef CCGS_COMMON_HPP_
#define CCGS_COMMON_HPP_

#include <assert.h>
#include <stddef.h>

#include <string_view>

#ifdef NDEBUG
#define COMPILE_TIME_LOG_LEVEL SPDLOG_LEVEL_OFF
#else
#define DDEBUG  // Do debug
#define COMPILE_TIME_LOG_LEVEL SPDLOG_LEVEL_TRACE
#endif  // NDEBUG

#ifndef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL COMPILE_TIME_LOG_LEVEL
#endif  // SPDLOG_ACTIVE_LEVEL

#include "spdlog/spdlog.h"

#define LOGT spdlog::trace
#define LOGD spdlog::debug
#define LOGI spdlog::info
#define LOGW spdlog::warn
#define LOGE spdlog::error
#define LOGC spdlog::critical

using LogLevel = spdlog::level::level_enum;

// Configure run-time logger with given log level and custom formatting.
void ConfigureLogger(LogLevel level = LogLevel::err);

class ScopeTrace {
  const std::string_view name_;

 public:
  // Log trace: [`scope_name`] [Begin], when constructing the object.
  ScopeTrace(const std::string_view& scope_name);

  // Log trace: ['name_`] [End], when destructing the object.
  ~ScopeTrace();
};

template <typename F>
using Func = std::function<F>;

struct Parser {
  // Try to parse int represented by `str` string. If the string represents a
  // valid number of base {0, 2, 3, ..., 36} - return it, otherwise nullopt.
  static std::optional<int> ParseInt(const char* str);
};

#endif  // CCGS_COMMON_HPP_
