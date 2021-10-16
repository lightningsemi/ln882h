#!/usr/bin/env python3
# -*- coding:utf-8 -*-

"""
@date: 2021-09-28 15:21:45
@author: shenglin.zhan@lightningsemi.com
@description:
"""

import sys
import os
import argparse
import subprocess
from ln_tools import check_python_version, realpath
from after_build_base import AfterBuildBase


class AfterBuildGCC(AfterBuildBase):
    def __init__(self) -> None:
        super().__init__()

    def prepare(self, *args) -> bool:

        if len(args) < 4:
            print("sdkroot_dir MUST be provided!!!")
            print("userproj_dir MUST be provided!!!")
            print("buildout_dir MUST be provided!!!")
            print("buildout_name MUST be provided!!!")
            return False

        self.sdkroot_dir    = os.path.abspath(args[0])
        userproj_dir        = realpath(args[1])
        if not os.path.exists(os.path.abspath(userproj_dir)):
            print("Error: userproj_dir not exist: {}".format(userproj_dir))
            return False

        self.buildout_dir   = args[2]
        self.buildout_name  = args[3]
        if not os.path.exists(self.buildout_dir):
            print("\nError: buildout_dir not exist: {}".format(self.buildout_dir))
            return False

        output_filename = None  # flashimage.bin (optional)
        if len(args) >= 5:
            output_filename = args[4]

        # TODO: how to get chip_type?
        chip_type = "ln882h"
        self.boot_filepath = os.path.abspath(os.path.join(self.sdkroot_dir, "lib/boot_{}.bin".format(chip_type)))
        self.app_filepath = os.path.join(self.buildout_dir, self.buildout_name + ".bin")

        if (output_filename is None) or len(output_filename) == 0:
            output_filename = "flashimage.bin"
        self.output_filepath = os.path.join(self.buildout_dir, output_filename)

        self.partcfg_filepath = os.path.abspath(os.path.join(userproj_dir, "cfg/flash_partition_cfg.json"))
        projcfg_filepath = os.path.abspath(os.path.join(userproj_dir, "cfg/proj_config.h"))
        major = None
        minor = None
        crp   = None
        try:
            with open(projcfg_filepath, "rt") as fObj:
                for line in fObj:
                    if not line.strip(" ").startswith("#define"):
                        continue
                    if line.find(AfterBuildBase.FLASH_IMAGE_VER_MAJOR_STR) > 0:
                        strlist = line.strip(" ").split(" ")
                        major = int(strlist[-1])
                    if line.find(AfterBuildBase.FLASH_IMAGE_VER_MINOR_STR) > 0:
                        strlist = line.strip(" ").split(" ")
                        minor = int(strlist[-1])
                    if line.find(AfterBuildBase.SOC_CRP_FLAG_STR) > 0:
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

        if not (self.buildout_dir and self.buildout_name and self.boot_filepath and self.app_filepath \
                and self.output_filepath and self.partcfg_filepath and self.ver_str):
            return False

        return True

    def build_stage_first(self) -> bool:
        """
        app.elf -> app.bin
        """
        axf_filepath = os.path.join(self.buildout_dir, self.buildout_name + ".elf")
        app_filepath = self.app_filepath

        if not os.environ["CROSS_TOOLCHAIN_ROOT"]:
            print("Error: CROSS_TOOLCHAIN_ROOT must be set for LN SDK!!!")
            return False

        TOOL_SUFFIX = ""
        if "win32" == sys.platform:
            TOOL_SUFFIX = ".exe"
        CROSS_COMPILE_PREFIX = os.environ["CROSS_TOOLCHAIN_ROOT"] + "/bin/arm-none-eabi-"
        objcopy_tool = CROSS_COMPILE_PREFIX + "objcopy" + TOOL_SUFFIX
        size_tool    = CROSS_COMPILE_PREFIX + "size" + TOOL_SUFFIX

        cmd = " ".join([objcopy_tool, "-O binary", axf_filepath, app_filepath])
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
        return super().build_stage_second()

    def build_stage_third(self) -> bool:
        """
        OTA image
        """
        return super().build_stage_third()

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
    desc = "GCC After Build Action"

    parser = argparse.ArgumentParser(prog=prog, description=desc)
    parser.add_argument("--sdkroot_dir",    help="LN SDK root dir", type=str)
    parser.add_argument("--userproj_dir",   help="user project dir, such as /path/to/wifi_mcu_basic_example", type=str)
    parser.add_argument("--buildout_dir",   help="GCC build output/bin dir, which has the FOO.elf(*.elf) file.", type=str)
    parser.add_argument("--buildout_name",  help="GCC build ouput name without 'elf', that is 'foo'.", type=str)
    parser.add_argument("-o", "--output",   help="final image name, default is 'flashimage.bin'", type=str)
    args = parser.parse_args()

    if not args.sdkroot_dir:
        print("LN SDK root dir MUST be provided!!!")
        exit(-1)

    if not args.userproj_dir:
        print("user project dir MUST be provided, such as /path/to/wifi_mcu_basic_example")
        exit(-2)

    if not args.buildout_dir:
        print("GCC build output/bin dir Must be provided!!!")
        exit(-3)

    if not args.buildout_name:
        print("GCC build output name MUST be provided!!!")
        exit(-4)

    buildObj = AfterBuildGCC()

    if not buildObj.prepare(args.sdkroot_dir,
                            args.userproj_dir,
                            args.buildout_dir,
                            args.buildout_name,
                            args.output):
        exit(-11)

    if not buildObj.doAllWork():
        exit(-12)
