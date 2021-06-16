//
// This file is part of the CCGS (https://github.com/damianzim/ccgs)
// distribution.
// Copyright (c) 2021 Piotr Jasik & Damian Zimon.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

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
