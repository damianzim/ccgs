#include "common.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>

void ConfigureLogger(LogLevel level) {
  auto plain = spdlog::create<spdlog::sinks::stdout_sink_st>("plain");
  plain->set_level(level);
  plain->set_pattern("[%L] %v");

  auto logger = spdlog::stdout_color_st("ccgs");
  logger->set_level(level);
  logger->set_pattern("[%^%L%$] [%Y-%m-%d %H:%M:%S.%f] %v");
  spdlog::set_default_logger(logger);
}

std::shared_ptr<spdlog::logger> LogPlain() { return spdlog::get("plain"); }
