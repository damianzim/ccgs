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

#ifndef CCGS_TRAIT_IMPL_HPP_
#define CCGS_TRAIT_IMPL_HPP_

#include "trait.hpp"

class Swift : public Trait {
 public:
  Swift* Clone() const override;
  bool Exec(TaskCtx&) const override;
  TaskExecTime ExecTime() const override;
  TaskOwner Owner() const override;
};

class Symbiotic : public Trait {
 public:
  Symbiotic* Clone() const override;
  bool Exec(TaskCtx&) const override;
  TaskExecTime ExecTime() const override;
  TaskOwner Owner() const override;
};

class Poisonous : public Trait {
 public:
  Poisonous* Clone() const override;
  bool Exec(TaskCtx&) const override;
  TaskExecTime ExecTime() const override;
  TaskOwner Owner() const override;
};

class Empowering : public Trait {
 public:
  Empowering* Clone() const override;
  bool Exec(TaskCtx&) const override;
  TaskExecTime ExecTime() const override;
  TaskOwner Owner() const override;
};

class Sabotaging : public Trait {
 public:
  Sabotaging* Clone() const override;
  bool Exec(TaskCtx&) const override;
  TaskExecTime ExecTime() const override;
  TaskOwner Owner() const override;
};

class Supporting : public Trait {
 public:
  Supporting* Clone() const override;
  bool Exec(TaskCtx&) const override;
  TaskExecTime ExecTime() const override;
  TaskOwner Owner() const override;
};

#endif  // CCGS_TRAIT_IMPL_HPP_
