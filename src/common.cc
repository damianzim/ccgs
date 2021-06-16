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

#include "common.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"

void ConfigureLogger(LogLevel level) {
  auto logger = spdlog::stdout_color_st("ccgs");
  logger->set_level(level);
  logger->set_pattern("[%^%L%$] %v");
  spdlog::set_default_logger(logger);
}

ScopeTrace::ScopeTrace(const std::string_view& name) : name_(name) {
  LOGT("[{}] [Begin]", name_);
}

ScopeTrace::~ScopeTrace() { LOGT("[{}] [End]", name_); }

std::optional<int> Parser::ParseInt(const char* str) {
  if (str == nullptr) return std::nullopt;
  try {
    return std::stoi(str, 0, 0);
  } catch (std::invalid_argument&) {
  } catch (std::out_of_range&) {
  }
  return std::nullopt;
}
