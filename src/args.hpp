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

#ifndef CCGS_ARGS_HPP_
#define CCGS_ARGS_HPP_

#include <map>
#include <set>
#include <string>

class Args {
 public:
  // Construct an object of Args, default flags and options are empty.
  Args(){};

  // Get the value of --<name> argument as a C-string. If such key-value pair
  // does not exist in the  set represented by this object, return nullptr.
  const char* GetValue(const std::string& name) const;

  // Check whenever exist flag with given `name`.
  bool IsFlag(const std::string& name) const;

  // Check whenever exist key-value pair with key equals to given `name`.
  bool IsOption(const std::string& name) const;

  // Clear current flags and options. If the `argv` parameter points to a valid
  // array (the format is the same as the programs main()'s argv parameter has)
  // then the flags and options are completed with appropriate values. The first
  // item in the array should contain a value (typically this is the program
  // name), then the metod returns true, if it is empty or the pointer is
  // invalid at all - return false.
  bool ResolveArgs(const char* const* argv);

 private:
  std::set<std::string> flags_;
  std::map<std::string, const char*> options_;
};

#endif  // CCGS_ARGS_HPP_
