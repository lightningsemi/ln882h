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

import os
import shutil

remove_list = [
    ######  BLE  ######
    "components/ble/CMakeLists.txt",
    "components/ble/mac/*.c",
    "components/ble/modules/*.c",

    "components/ble/mac/ble/ll/import",
    "components/ble/mac/ble/hl/config",
    "components/ble/mac/sch/import",
    "components/ble/modules/rwip/import",
    "components/ble/modules/rwip/api/rwip_config_1conn_scan_adv_noprf.h",

    ######  WIFI  ######
    "components/wifi/CMakeLists.txt",
    "components/wifi/wifi_cfg",
    "components/wifi/wifi_if",
    "components/wifi/wifi_mac",

    ###### PROJECT ######
    "project/armcc_lib_build",
    "project/ble_mcu_data_trans/keil_ln882h_dev",
    "project/ble_mcu_scan/keil_ln882h_dev",
    "project/combo_mcu_basic_example/keil_ln882h_dev",
    "project/wifi_mcu_basic_example/keil_ln882h_dev",

    ###### others #######
    ".gitlab-ci.yml",
    "ln_sdk_build_check.py",
    "tools/python_scripts/release_auto_remove.py",
]

def delete_pattern_files(pattern_path):
    """
    /path/to/abc/*.c
    """
    dir_start = os.path.dirname(pattern_path)
    pattern_str = pattern_path.split("/")[-1]

    if pattern_str is None:
        return

    if isinstance(pattern_str, str):
        if not pattern_str.startswith("*"):
            return

        patter_xxx = pattern_str.split(".")[-1]
        for parent, dirnames, filenames in os.walk(dir_start):
            for f in filenames:
                if isinstance(f, str) and f.endswith(patter_xxx):
                    path_delete = os.path.normpath(os.path.abspath(os.path.join(parent, f)))
                    print("[DEL]--> {}".format(path_delete))
                    os.remove(path_delete)

def delete_proj_files():
    """
    NOTE: 必须从源码顶层目录运行这个脚本;
    没有注释的行会被删除,
    被注释的行会被保留。
    """
    root_dir = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

    for path in remove_list:
        temp_path = os.path.join(root_dir, path)
        temp_path = temp_path.replace("\\", "/")

        if not os.path.exists(temp_path):
            delete_pattern_files(temp_path)
            continue

        if os.path.isdir(temp_path):
            if os.listdir(temp_path):
                print("delete dir --> {}".format(temp_path))
                shutil.rmtree(temp_path)
            else:
                print("delete dir --> {}".format(temp_path))
                os.removedirs(temp_path)
        elif os.path.isfile(temp_path):
            print("delete file --> {}".format(temp_path))
            os.remove(temp_path)
        else:
            pass


if __name__ == "__main__":
    delete_proj_files()
