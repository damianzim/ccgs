#include "common.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>

void ConfigureLogger(LogLevel level) {
  auto logger = spdlog::stdout_color_st("ccgs");
  logger->set_level(level);
  logger->set_pattern("[%^%L%$] [%Y-%m-%d %H:%M:%S.%f] %v");
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

std::optional<unsigned long> Parser::ParseULong(const char* str) {
  if (str == nullptr) return std::nullopt;
  try {
    return std::stoul(str, 0, 0);
  } catch (std::invalid_argument&) {
  } catch (std::out_of_range&) {
  }
  return std::nullopt;
}
