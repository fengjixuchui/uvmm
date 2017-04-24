/*
 * Copyright (C) 2017 Kernkonzept GmbH.
 * Author(s): Alexander Warg <alexander.warg@kernkonzept.com>
 *            Sarah Hoffmann <sarah.hoffmann@kernkonzept.com>
 *
 * This file is distributed under the terms of the GNU General Public
 * License, version 2.  Please see the COPYING-GPL-2 file for details.
 */
#include <l4/re/env>

#include "virtio_proxy.h"
#include "device_factory.h"

namespace {

using namespace Vdev;

struct F : Factory
{
  cxx::Ref_ptr<Device> create(Vmm::Guest *vmm,
                              Vmm::Virt_bus *,
                              Dt_node const &node)
  {
    int cap_name_len;
    char const *cap_name = node.get_prop<char>("l4vmm,virtiocap", &cap_name_len);
    if (!cap_name)
      {
        Dbg(Dbg::Dev, Dbg::Warn, "virtio")
          .printf("'l4vmm,virtiocap' property missing for virtio proxy device.\n");
        return nullptr;
      }

    cap_name_len = strnlen(cap_name, cap_name_len);

    auto cap = L4Re::Env::env()->get_cap<L4virtio::Device>(cap_name, cap_name_len);
    if (!cap)
      {
        Dbg(Dbg::Dev, Dbg::Warn, "virtio")
          .printf("'l4vmm,virtiocap' property: capability %.*s is invalid.\n",
                  cap_name_len, cap_name);
        return nullptr;
      }

    auto c = make_device<Virtio_proxy_mmio>();
    c->register_irq(vmm->registry(), cap);
    vmm->register_mmio_device(c, node);
    return c;
  }
};

static F f;
static Device_type t = { "virtio,mmio", "proxy", &f };

}

