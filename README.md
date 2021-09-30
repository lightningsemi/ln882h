# CMake 使用及常见问题

# 环境检查 
检查是否安装**cmke**
检查是否安装**python3**

# 使用方法
### 使用build.sh脚本
- 1 根目录下执行`./build.sh build`,这个命令会建立build文件夹并执行**cmake**命令，生成makefile文件，如果执行失败请检查**build.sh**是否有可执行权限,添加可执行权限。
    ```shell
    chmod +x build.sh && ./build.sh build
    ```
- 2 `./build.sh build`执行成功后执行命令，这个命令会使用上一步生成的makefile文件生成最终的可执行文件**flashimage.bin**。
    ```shell
    ./build.sh all
    ```
  
- 3 执行`./build.sh clean`命令会清除**build**文件夹
    ```shell
    ./build.sh clean
    ```

### 直接使用cmake

- 1 在项目根目录下建立并进入**build**文件夹
    ```shell
    mkdir build && cd build
    ```
- 2 执行**cmake**命令
    ```shell
    cmake -DPROJECT=wifi_mcu_develop ..
    ```
- 3 生成**Makefile**文件之后执行`make`命令生成可执行文件，可执行文件**flashimage.bin**在**build/bin**文件夹下
    ```shell
    make
    ```


# 常见问题

### 1 执行`./build.sh build`后出现CMAKE_CXX_COMPILER not set

检查是否安装**g++**


### 2 tools/Linux/arm-none-eabi-gcc/bin/arm-none-eabi-gcc: Permission denied
出现这个问题很大原因是这个可执行文件没有可执行权限，并且除`tools/Linux/arm-none-eabi-gcc/bin`之外其他文件也可能没有可执行权限，因此可对`tools/Linux/arm-none-eabi-gcc/`文件夹以及相关子目录添加可执行权限。


