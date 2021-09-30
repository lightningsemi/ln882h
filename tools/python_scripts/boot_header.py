#!/usr/bin/env python3
# -*- coding:utf-8 -*-

"""
@date: 2021-09-26 19:25:39
@author: shenglin.zhan@lightningsemi.com
@description:
"""

import zlib
import struct
from ln_tools import *


class BootHeader:

    BOOT_HEADER_SIZE = (4 + 2 + 2 + 4 * 4)

    CRP_VALID_FLAG = 0x46505243

    def __init__(self, other_buf) -> None:
        self.__bootram_target_addr  = 0
        self.__bootram_bin_length   = 0 # 2bytes
        self.__bootram_crc_offset   = 0 # 2bytes
        self.__bootram_crc_value    = 0
        self.__bootram_vector_addr  = 0
        self.__crp_flag             = 0
        self.__boot_header_crc      = 0

        if not (isinstance(other_buf, bytearray) or isinstance(other_buf, bytes)):
            raise TypeError("Error: other_buf MUST be a bytearray or bytes!!!")

        if len(other_buf) < BootHeader.BOOT_HEADER_SIZE:
            raise ValueError("Error: other_buf MUST have at least {} bytes!!!".format(BootHeader.BOOT_HEADER_SIZE))

        self.__buffer = bytearray(BootHeader.BOOT_HEADER_SIZE)
        self.__buffer[:] = other_buf[0:BootHeader.BOOT_HEADER_SIZE]

        items = struct.unpack("<I2H4I", self.__buffer)
        self.__bootram_target_addr  = items[0]
        self.__bootram_bin_length   = items[1]
        self.__bootram_crc_offset   = items[2]
        self.__bootram_crc_value    = items[3]
        self.__bootram_vector_addr  = items[4]
        self.__crp_flag             = items[5]
        self.__boot_header_crc      = items[6]

    def toByteArray(self) -> bytearray:
        self.__boot_header_crc = zlib.crc32(self.__buffer[0:(BootHeader.BOOT_HEADER_SIZE-4)])
        struct.pack_into("<I2H4I", self.__buffer, 0,
                        self.bootram_target_addr,
                        self.bootram_bin_length, self.bootram_crc_offset,
                        self.bootram_crc_value, self.bootram_vector_addr, self.crp_flag, self.boot_header_crc)
        return self.__buffer

    @property
    def bootram_target_addr(self):
        return self.__bootram_target_addr

    @property
    def bootram_bin_length(self):
        return self.__bootram_bin_length

    @property
    def bootram_crc_offset(self):
        return self.__bootram_crc_offset

    @property
    def bootram_crc_value(self):
        return self.__bootram_crc_value

    @property
    def bootram_vector_addr(self):
        return self.__bootram_vector_addr

    @property
    def crp_flag(self):
        return self.__crp_flag

    @crp_flag.setter
    def crp_flag(self, crp):
        if isinstance(crp, int):
            if (crp == 0) or (crp == 1):
                self.__crp_flag = crp
            else:
                raise ValueError("Error: crp MUST be 0 or 1!!!")
        else:
            raise TypeError("Error: crp MUST be int type!!!")

    @property
    def boot_header_crc(self):
        return self.__boot_header_crc