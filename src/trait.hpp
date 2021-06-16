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

#ifndef CCGS_TRAIT_HPP_
#define CCGS_TRAIT_HPP_

enum class TaskExecTime;
enum class TaskOwner;
struct TaskCtx;

class Trait {
 public:
  virtual ~Trait(){};

  // Duplicate the trait.
  virtual Trait* Clone() const = 0;

  // Execute the command. If the task has been executed correctly return true,
  // or false if it should be retried.
  virtual bool Exec(TaskCtx&) const = 0;

  // Indicate the time when the `Exec()` method is to be run for the first time.
  virtual TaskExecTime ExecTime() const = 0;

  // Indicate the player for whom this trait should be performed.
  virtual TaskOwner Owner() const = 0;
};

#endif  // CCGS_TRAIT_HPP_
