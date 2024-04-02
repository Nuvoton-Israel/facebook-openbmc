#!/usr/bin/env python3
#
# Copyright (c) Meta Platforms, Inc. and affiliates. (http://www.meta.com)
#
# This program file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program in a file named COPYING; if not, write to the
# Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA
#
import unittest

from common.base_interface_test import CommonInterfaceTest
from utils.cit_logger import Logger
from utils.test_utils import qemu_check


"""
Tests eth0 & usb0 v6 interface
"""


class InterfaceTest(CommonInterfaceTest, unittest.TestCase):
    def test_eth0_v4_interface(self):
        """
        Tests eth0 v4 interface
        """
        self.set_ifname("eth0")
        Logger.log_testname(name=self._testMethodName)
        self.assertEqual(self.ping_v4(), 0, "Ping test for eth0 v4 failed")

    @unittest.skipIf(qemu_check(), "test env is QEMU, skipped")
    def test_usb0_v6_interface(self):
        """
        Tests usb0 v6 interface
        """
        self.set_ifname("usb0")
        Logger.log_testname(self._testMethodName)
        self.assertEqual(self.ping_v6(), 0, "Ping test for usb0 v6 failed")

    def test_usb0_v6_x86_interface(self):
        """
        Tests usb0 v6 interface ping to x86 side
        """
        self.set_ifname("usb0")
        ip = "fe80::ff:fe00:2"
        cmd = "ping6 -c 1 -q -w 1 " + ip + "%" + self.ifname
        self.assertEqual(self.run_ping(cmd), 0, "Ping test for x86 failed")
