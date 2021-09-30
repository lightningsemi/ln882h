#!/usr/bin/env python3
# -*- coding:utf-8 -*-

"""
@date: 2021-09-28 15:21:45
@author: shenglin.zhan@lightningsemi.com
@description:
"""

import os
import argparse
import subprocess
from ln_tools import check_python_version
from makeimage import MakeImageTool
from ota_image_generator import OTATOOL
import xml.etree.ElementTree as ET


class AfterBuild:
    __FLASH_IMAGE_VER_MAJOR_STR = "FLASH_IMAGE_VER_MAJOR"
    __FLASH_IMAGE_VER_MINOR_STR = "FLASH_IMAGE_VER_MINOR"
    __SOC_CRP_FLAG_STR          = "SOC_CRP_FLAG"

    def __init__(self) -> None:
        self.keil_proj_path     = None # abcd.uvprojx filepath
        self.cur_working_dir    = None
        self.keil_output_dir    = None # abs dire, in which abcd.axf resides
        self.keil_output_name   = None # no suffix

        # MakeImageTool parameters
        self.ramcode_filepath   = None
        self.app_filepath       = None
        self.output_filepath    = None # default is 'flashimage.bin'
        self.partcfg_filepath   = None
        self.ver_str            = None
        self.crp_flag           = 0

    def prepare(self, keil_proj_filepath, output_filename=None) -> bool:
        if not os.path.exists(keil_proj_filepath):
            print("\nError: not exist: {}".format(keil_proj_filepath))
            return False

        self.keil_proj_path     = keil_proj_filepath
        self.cur_working_dir    = os.path.dirname(os.path.abspath(keil_proj_filepath))

        chip_type = None
        tree = ET.ElementTree(file=self.keil_proj_path)
        root_node = tree.getroot()
        for ele in root_node.iter():
            if "OutputDirectory" == ele.tag:
                self.keil_output_dir = os.path.abspath(os.path.join(self.cur_working_dir, ele.text))
            if "OutputName" == ele.tag:
                self.keil_output_name = ele.text
            if "VariousControls" == ele.tag:
                define_ele = ele.find("Define")
                if define_ele.text:
                    strlist = define_ele.text.strip(" ").split(",")
                    for temp in strlist:
                        if temp.startswith("LN"):
                            chip_type = temp.strip("").lower()
                            break
        if not chip_type:
            print("Error: chip type macro (such as LN882H) has not been found!!!")
            return False

        self.ramcode_filepath = os.path.abspath(os.path.join(self.cur_working_dir, "../../../lib/boot_{}.bin".format(chip_type)))
        self.app_filepath = os.path.join(self.cur_working_dir, self.keil_output_name + ".bin")

        if (output_filename is None) or len(output_filename) == 0:
            output_filename = "flashimage.bin"
        self.output_filepath = os.path.join(self.cur_working_dir, output_filename)

        self.partcfg_filepath = os.path.abspath(os.path.join(self.cur_working_dir, "../cfg/flash_partition_cfg.json"))

        projcfg_filepath = os.path.abspath(os.path.join(self.cur_working_dir, "../cfg/proj_config.h"))
        major = None
        minor = None
        crp   = None
        try:
            with open(projcfg_filepath, "rt") as fObj:
                for line in fObj:
                    if not line.strip(" ").startswith("#define"):
                        continue
                    if line.find(AfterBuild.__FLASH_IMAGE_VER_MAJOR_STR) > 0:
                        strlist = line.strip(" ").split(" ")
                        major = int(strlist[-1])
                    if line.find(AfterBuild.__FLASH_IMAGE_VER_MINOR_STR) > 0:
                        strlist = line.strip(" ").split(" ")
                        minor = int(strlist[-1])
                    if line.find(AfterBuild.__SOC_CRP_FLAG_STR) > 0:
                        strlist = line.strip(" ").split(" ")
                        crp = int(strlist[-1])
        except OSError as err:
            print("Error: read LN SDK version from file: {}".format(str(err)))
            return False

        if (major is None) or (minor is None):
            print("no LN SDK version is found!!!")
            return False

        self.ver_str = '{}.{}'.format(major, minor)

        if isinstance(crp, int):
            self.crp_flag = crp

        if not (self.keil_output_dir and self.keil_output_name and self.ramcode_filepath and self.app_filepath \
                and self.output_filepath and self.partcfg_filepath and self.ver_str):
            return False

        return True

    def build_stage_first(self) -> bool:
        """
        app.axf -> app.bin
        """
        axf_filepath = os.path.join(self.keil_output_dir, self.keil_output_name + ".axf")
        app_filepath = self.app_filepath
        cmd = " ".join(["fromelf", "--bin", "--output=" + app_filepath, axf_filepath])
        try:
            retval = subprocess.check_call(cmd, stdout=subprocess.DEVNULL, shell=True)
        except subprocess.CalledProcessError as err:
            print(str(err))
            retval = err.returncode

        if retval != 0:
            print("Error: produce {} failed!!!".format(os.path.basename(app_filepath)))
            return False

        return True

    def build_stage_second(self) -> bool:
        """
        flashimage.bin
        """
        mkimage = MakeImageTool()
        mkimage.ramcode_filepath = self.ramcode_filepath
        mkimage.app_filepath = self.app_filepath
        mkimage.flashimage_filepath = self.output_filepath
        mkimage.part_cfg_filepath = self.partcfg_filepath
        mkimage.ver_str = self.ver_str
        mkimage.swd_crp = self.crp_flag
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

    def doAllWork(self) -> bool:
        if not self.build_stage_first():
            return False
        if not self.build_stage_second():
            return False
        if not self.build_stage_third():
            return False
        return True


if __name__ == "__main__":

    check_python_version()

    prog = os.path.basename(__file__)
    desc = "Keil After Build Action"
    parser = argparse.ArgumentParser(prog=prog, description=desc)
    parser.add_argument("-k", "--keilproj", help="*.uvprojx filepath", type=str)
    parser.add_argument("-o", "--output",   help="final image name, default is 'flashimage.bin'", type=str)
    args = parser.parse_args()

    if not args.keilproj:
        print("Error: no filepath of *.uvprojx is provided!!!")
        exit(-1)

    buildObj = AfterBuild()

    if not buildObj.prepare(args.keilproj, args.output):
        exit(-2)

    if not buildObj.doAllWork():
        exit(-3)

    exit(0)
