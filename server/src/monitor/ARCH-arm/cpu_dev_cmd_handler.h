/*
 * Copyright (C) 2019 Kernkonzept GmbH.
 * Author(s): Sarah Hoffmann <sarah.hoffmann@kernkonzept.com>
 *            Alexander Warg <alexander.warg@kernkonzept.com>
 *            Timo Nicolai <timo.nicolai@kernkonzept.com>
 *
 * This file is distributed under the terms of the GNU General Public
 * License, version 2.  Please see the COPYING-GPL-2 file for details.
 */
#pragma once

#include <cstdio>
#include <cstring>

#include "monitor.h"
#include "monitor_util.h"
#include "vcpu_ptr.h"

namespace Monitor {

template<bool, typename T>
class Cpu_dev_cmd_handler {};

template<typename T>
class Cpu_dev_cmd_handler<true, T> : public Cmd
{
public:
  char const *help() const override { return "CPU state"; }

  void complete(FILE *f, char const *args) const override
  { simple_complete(f, args, {"regs"}); }

  void exec(FILE *f, char const *args) override
  {
    if (strcmp(args, "regs") == 0)
      show_regs(f);
    else
      print_help(f);
  }

  void show_regs(FILE *f) const
  {
    auto regs = get_vcpu()->r;

    fprintf(f, "pc=%08lx lr=%08lx sp=%08lx flags=%08lx\n",
            regs.ip, regs.lr, regs.sp, regs.flags);
    fprintf(f, " r0=%08lx  r1=%08lx  r2=%08lx  r3=%08lx\n",
            regs.r[0], regs.r[1], regs.r[2], regs.r[3]);
    fprintf(f, " r4=%08lx  r5=%08lx  r6=%08lx  r7=%08lx\n",
            regs.r[4], regs.r[5], regs.r[6], regs.r[7]);
    fprintf(f, " r8=%08lx  r9=%08lx r10=%08lx r11=%08lx\n",
            regs.r[8], regs.r[9], regs.r[10], regs.r[11]);
    fprintf(f, "r12=%08lx\n", regs.r[12]);
  }

private:
  void print_help(FILE *f) const
  {
    fprintf(f, "%s\n"
               "* 'cpu <i> regs': dump CPU registers\n",
            help());
  }

  Vmm::Vcpu_ptr get_vcpu() const
  { return static_cast<T const *>(this)->vcpu(); }
};

}
