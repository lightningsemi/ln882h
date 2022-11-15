#!/usr/bin/env python3
# -*- coding:utf-8 -*-
#
# Copyright 2021 Shanghai Lightning Semiconductor Technology Co., LTD

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#     http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import imp
import os
from ln_tools import check_python_version
from makeimage import MakeImageTool
from ota_image_generator import OTATOOL
import xml.etree.ElementTree as ET
from bin_to_hex import bin_to_hex
import time
 

class AfterBuildBase:
    FLASH_IMAGE_VER_MAJOR_STR = "FLASH_IMAGE_VER_MAJOR"
    FLASH_IMAGE_VER_MINOR_STR = "FLASH_IMAGE_VER_MINOR"
    SOC_CRP_FLAG_STR          = "SOC_CRP_FLAG"
    HEX_START_ADDR            = 0x10000000
    #HEX_START_ADDR            = 0x00000000
    def __init__(self) -> None:
        self.sdkroot_dir        = None # LN SDK root dir.
        self.buildout_dir       = None # absolute directory, in which app.axf/app.elf resides.
        self.buildout_name      = None # no suffix (such as app.axf/app.elf -> app)

        # MakeImageTool parameters
        self.boot_filepath      = None # /path/to/boot_ln88xx.bin
        self.app_filepath       = None # /path/to/app.bin
        self.output_filepath    = None # default is 'flashimage.bin'
        self.partcfg_filepath   = None # /path/to/flash_partition_cfg.json
        self.ver_str            = None # ???
        self.crp_flag           = 0    # 0 -- SWD enabled; 1 -- SWD disabled.

    def prepare(self, *args) -> bool:
        return True

    def build_stage_first(self) -> bool:
        # Depends on the IDE behavior.
        return True

    def build_stage_second(self) -> bool:
        """
        app.bin --> flashimage.bin
        """
        mkimage = MakeImageTool()
        mkimage.boot_filepath       = self.boot_filepath
        mkimage.app_filepath        = self.app_filepath
        mkimage.flashimage_filepath = self.output_filepath
        mkimage.part_cfg_filepath   = self.partcfg_filepath
        mkimage.ver_str             = self.ver_str
        mkimage.swd_crp             = self.crp_flag
        if not mkimage.doAllWork():
            print("Failed to build: {}".format(self.output_filepath))
            return False
        print("Succeed to build: {}".format(self.output_filepath))
        return True

    def build_stage_third(self) -> bool:
        """
        OTA image
        """

        flashimage_filepath = self.output_filepath
        ota_save_dir = os.path.dirname(flashimage_filepath)

        ota_tool = OTATOOL()
        ota_tool.input_filepath = flashimage_filepath
        ota_tool.output_dir     = ota_save_dir

        if not ota_tool.doAllWork():
            print("Failed to build {}".format(ota_tool.output_filepath))
            return False

        print("Succeed to build: {}".format(ota_tool.output_filepath))
        return True

    def build_stage_fourth(self) -> bool:
        """
        flashimage.bin --> flashimage.hex
        """
        if not bin_to_hex(self.output_filepath,self.output_filepath[0:-4]+'.hex',self.HEX_START_ADDR):
            print("Failed to build {}".format(self.output_filepath[0:-4]+'.hex'))
            return False
        now_time = time.localtime()
        time_string = "{yyyy}-{MM:02d}-{DD:02d} {hh:02d}:{mm:02d}:{ss:02d}".format(yyyy=now_time.tm_year,
        MM=now_time.tm_mon, DD=now_time.tm_mday,
        hh=now_time.tm_hour, mm=now_time.tm_min, ss=now_time.tm_sec)
        print("----------" * 6)
        print("\t\t\thex done @ {t}".format(t=time_string))
        print("----------" * 6)
        return True

    def doAllWork(self) -> bool:
        if not self.build_stage_first():
            return False
        if not self.build_stage_second():
            return False
        if not self.build_stage_third():
            return False 
        return True
