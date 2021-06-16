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

#include <iostream>
#include <string>

#include "args.hpp"
#include "common.hpp"
#include "game.hpp"
#include "game_params.hpp"

static const char* prog;

// Print help to the stderr, end exit the program with `exit_status` code.
[[noreturn]] void Usage(int exit_status = EXIT_FAILURE);

// Initalise a logger with the level `default_level` or if `args` contains
// 'level' option, with it. Return false if the option is invalid or the logger
// itself could not been initialised, otherwise true.
bool InitialiseLogger(const Args& args, LogLevel default_level);

int main([[maybe_unused]] int argc, char* argv[]) {
  prog = argv[0];

  Args args;
  if (!args.ResolveArgs(argv)) {
    std::cerr << "Could not resolve arguments." << std::endl;
    return EXIT_FAILURE;
  }

  if (args.IsFlag("help")) Usage(EXIT_SUCCESS);
  if (!InitialiseLogger(args, LogLevel::info)) return EXIT_FAILURE;

  const char* output_dir = args.GetValue("output");
  if (output_dir == nullptr) output_dir = ".";

  auto params = GameParams::Parse(args);
  if (!params) return EXIT_FAILURE;

  Game game{*params};
  if (!game.InitGame(output_dir)) return EXIT_FAILURE;

  auto result = game.Run();
  spdlog::log(result.Ok() ? LogLevel::info : LogLevel::err,
              "Finished with status {} ({})", result.StatusStringify(),
              result.Status());

  return EXIT_SUCCESS;
}

void Usage(int exit_status) {
  // clang-format off
  std::cerr << prog
            << " [--help] [--level LEVEL] [--output DIR] [--balance N]\n\
  [--deck-size N] [--pool-size N] [--cards N] [--turn-limit N] [--leading N]\n\
\n\
optional arguments:\n\
  --help          This help message.\n\
  --level LEVEL   Set log level. Available levels: trace, debug, info, warning,\
\n\
                  error, critical, off.\n\
  --output DIR    Directory where the output files will be stored, default:\n\
                  current dir.\n\
  --balance N     This argument determines upper and lower bound of Power Level\
\n\
                  which is +/- N. Must be an integer in [0;5], default: 0.\n\
  --deck-size N   Number of cards for each player. Must be an integer in\n\
                  [15;50], default: 30.\n\
  --pool-size N   Number of cards in in the cards pool from wich players will \
be\n\
                  drawing their decks. Must be an integer in\n\
                  [(deck size)+1;(deck size)^2], default: 100.\n\
  --cards N       Number of cards each player has in his hand at the start of\n\
                  the game. Must be an integer in [3;10], default: 5.\n\
  --turn-limit N  Maximum number of turns the game must end after. Must be an\n\
                  integer in [3;20], default: 10.\n\
  --leading N     Minimum number of points a player must lead to be considered\
\n\
                  a winner or when 0 - the game cannot end with such a case.\n\
                  Must be an integer in [20;100] or 0, deafult: 50."
            << std::endl;
  // clang-format on
  exit(exit_status);
}

bool InitialiseLogger(const Args& args, LogLevel level) {
  auto arg_value = args.GetValue("level");
  if (arg_value != nullptr) {
    const std::string name{arg_value};
    // Check log level 'off' as a separate case, because spdlog::from_str
    // actually may return this level when it is present or when level could not
    // been parsed.
    if (name != "off") {
      level = spdlog::level::from_str(name);
      if (level == LogLevel::off) {
        std::cerr << "Could not parse log level." << std::endl;
        return false;
      }
    } else {
      level = LogLevel::off;
    }
  }

  try {
    ConfigureLogger(level);
  } catch (const spdlog::spdlog_ex& e) {
    std::cerr << "Could not initialise logger: " << e.what() << std::endl;
    return false;
  }
  return true;
}
