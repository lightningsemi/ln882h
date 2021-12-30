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

import sys
import os
import shutil
import argparse
import subprocess

# NOTE: first argument of this script.
user_action     = "config"                  # default action.

# NOTE: read from top CMakeLists.txt.
user_project    = "wifi_mcu_basic_example"  # default user project.

# NOTE: read from top CMakeListx.txt
build_type      = "debug"

# NOTE: set up cmake build directory, build-proj-debug.
build_path      = "build"

# NOTE: select generator.
make_generator  = "Ninja"
# make_generator  = "Unix Makefiles"

def read_cmake_config():
    """
    Read configuration from top CMakeLists.txt.
    Return True on success, return False on failure.
    """
    FLAG_USER_PROJECT = "USER_PROJECT"
    FLAG_BUILD_TYPE   = "CMAKE_BUILD_TYPE"

    global user_project
    global build_type
    global build_path

    found_user_project  = False
    found_build_type    = False

    fObj = None
    if sys.version_info.major == 3:
        fObj = open("CMakeLists.txt", "r", encoding="utf-8")
    else:
        fObj = open("CMakeLists.txt", "r")

    if fObj:
        for line in fObj:
                line = line.strip()
                if line.startswith(("set", "SET")):
                    if not found_user_project:
                        index = line.find(FLAG_USER_PROJECT)
                        if index >= 0:
                            left_bracket = line.find("(")
                            right_bracket = line.find(")")
                            if (left_bracket > 0) and (right_bracket > 0):
                                temp_array = [item for item in line[(left_bracket+1):right_bracket].split(" ") if len(item) > 1]
                                user_project = temp_array[-1]
                                print("user_project = {_p}".format(_p=user_project))

                                found_user_project = True
                    if not found_build_type:
                        index = line.find(FLAG_BUILD_TYPE)
                        if index >= 0:
                            left_bracket = line.find("(")
                            right_bracket = line.find(")")
                            if (left_bracket > 0) and (right_bracket > 0):
                                array_temp = [item for item in line[(left_bracket+1):right_bracket].strip().split(" ") if len(item) > 1]
                                build_type = array_temp[1]
                                print("build_type = {_bt}".format(_bt=build_type))
                                found_build_type = True

                    if found_user_project and found_build_type:
                        break
        fObj.close()

    if found_user_project and found_build_type:
        build_path = "build-{_proj}-{_tp}".format(_tp=build_type.lower(), _proj=user_project)
        print("build_path = {_b}".format(_b=build_path))
        return True

    print("Warning: read configuration from top CMakeLists.txt!!!")
    print("     Please check if there are following lines in CMakeLists.txt:")
    print("")
    print("set(USER_PROJECT  wifi_mcu_basic_example)")
    print("")
    return False


def show_usage():
    print("*****************************  usage  *****************************")
    print("argv[1] -- build action, such as clean/config/build/rebuild/jflash.")
    print("Example:")
    print("python3 {}  rebuild".format(os.path.basename(__file__)))
    print("*******************************************************************")


def action_clean():
    print("------  clean directory: {_b}  ------".format(_b=build_path))
    if os.path.exists(build_path):
        shutil.rmtree(build_path, ignore_errors=True)


def action_config():
    global make_generator

    print("------  config: {_p}  ------".format(_p=user_project))
    cmd = "cmake -S . -B {_b}  -G \"{_g}\"".format(_b=build_path, _g=make_generator)
    os.system(cmd)


def action_build():
    global make_generator

    print("------  build: {_p}  ------".format(_p=user_project))
    if not os.path.exists(build_path):
        cmd = "cmake -S . -B {_b}  -G \"{_g}\"".format(_b=build_path, _g=make_generator)
        os.system(cmd)

    cmd = "cmake --build {_b}".format(_b=build_path)
    os.system(cmd)


def action_rebuild():
    global make_generator

    print("------  rebuild: {_p}  ------".format(_p=user_project))
    if os.path.exists(build_path):
        shutil.rmtree(build_path, ignore_errors=True)

    cmd = "cmake -S . -B {_b} -G \"{_g}\"".format(_b=build_path, _g=make_generator)
    os.system(cmd)

    cmd = "cmake --build {_b}".format(_b=build_path)
    os.system(cmd)


def action_jflash():
    print("------  download flashimage.bin via JFlash  ------")

    jflash_exe = None
    if sys.platform == "linux":
        jflash_exe = "JFlash"
    elif sys.platform == "win32":
        jflash_exe = "JFlash.exe"
    else:
        print("Not supported platform!!!")
        exit(-1)

    opts = "-erasesectors -programverify"
    cmd = "{_j} -openprj{_top}/tools/JFlash/ln882h.jflash -open{_b}/bin/flashimage.bin,0x0 {_opt}"\
            .format(_j=jflash_exe, _top=os.path.dirname(os.path.abspath(__file__)) ,_b=build_path, _opt=opts)
    os.system(cmd)

    print("------  reset the chip and the code starts running...  ------")


def call_sdk_env_check():
    script_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "tools", "python_scripts", "sdk_env_check.py")
    try:
        retcode = subprocess.check_call(["python", script_path])
        if retcode != 0:
            return False
    except subprocess.CalledProcessError as err:
        print("returncode: {}".format(err.returncode))
        print("cmd: {}".format(err.cmd))
        print("output: {}".format(err.output))
        return False

    return True


if __name__ == "__main__":

    if not call_sdk_env_check():
        exit(-1)

    prog = os.path.basename(__file__)
    desc = "LN SDK Build Entry"
    usage = "" \
    """
    *****************************  usage  *****************************
    argv[1] -- build action, such as clean/config/build/rebuild/jflash.
    Example:
                python3   {}   rebuild
    *******************************************************************
    """.format(os.path.basename(__file__))

    parser = argparse.ArgumentParser(prog=prog, description=desc, usage=usage)
    parser.add_argument("action", help="build action MUST be provided.", type=str,
                        choices=["clean", "config", "build", "rebuild", "jflash"])
    args = parser.parse_args()
    if not args.action:
        show_usage()
        exit(-1)

    user_action = args.action

    valid_action_collection = {
        "clean":   action_clean,
        "config":  action_config,
        "build":   action_build,
        "rebuild": action_rebuild,
        "jflash":  action_jflash
    }

    if not read_cmake_config():
        exit(-1)

    valid_action_collection.get(user_action)()
    exit(0)
