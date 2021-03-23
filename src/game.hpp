#ifndef CCGS_GAME_HPP_
#define CCGS_GAME_HPP_

#include <optional>

#include "args.hpp"
#include "common.hpp"

std::optional<GameParams> ParseGameParams(const Args& args);

#endif  // CCGS_GAME_HPP_
