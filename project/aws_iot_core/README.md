# AWS IoT Demo 使用说明

`aws_iot_core` demo 提供了 `demo_http_mutual_auth` 和 `demo_mqtt_mutual_auth` 两个示例程序。

## AWS IoT Core 环境准备

1. 登陆 [亚马逊](https://aws.amazon.com/cn/) ，然后进入 **AWS IoT Core 控制台**
2. 根据官方教程 [创建 AWS IoT 资源](https://docs.aws.amazon.com/zh_cn/iot/latest/developerguide/create-iot-resources.html) 创建 IoT 策略和 Things（官方中文翻译为 物品）

    注意：在 Download certificates and keys（下载证书和密钥）页面，**下载所有证书**！因为设备证书可能仅展示一次，**本窗口关闭后无法获得下载链接**，只能重新创建 Things。

得到以下证书：

- `ceritificate.pem.crt` 结尾的证书，设备证书
- `private.pem.key` 结尾的私钥，设备私钥
- `public.pem.key` 结尾的公钥
- `AmazonRootCA1.pem`，根证书
- `AmazonRootCA3.pem`，备用根证书

> 注意：亚马逊 AWS IoT Core 不同地理位置之间数据不互通，因此在创建 Things 的时候注意你当前使用的地理位置。

## 准备编译环境

1. 准备好上述步骤下载的证书
2. 复制 **根证书** 内容

    使用文本编辑器打开 `AmazonRootCA1.pem`，将文件内容完整地拷贝到 `certificates/ca_cert_def.c` 文件的
    `ca_crt_rsa` 数组里，并在每行的开头增加 `"`，在每行的结尾增加 `\r\n"`。

    注意，数组末尾必须保留 `" "` 。

3. 复制 **设备证书** 内容

    使用文本编辑器打开 `ceritificate.pem.crt` 结尾的证书，将文件内容完整地拷贝到 `certificates/ca_cert_def.c` 文件的
    `client_cert_pem` 数组里，并在每行的开头增加 `"`，在每行的结尾增加 `\r\n"`。

    注意，数组末尾必须保留 `" "` 。

4. 复制 **设备私钥** 内容

    使用文本编辑器打开 `private.pem.key` 结尾的私钥，将文件内容完整地拷贝到 `certificates/ca_cert_def.c` 文件的
    `ca_private_pem_key` 数组里，并在每行的开头增加 `"`，在每行的结尾增加 `\r\n"`。

    注意，数组末尾必须保留 `" "` 。

## MDK 编译

1. `demo_http_mutual_auth` 和 `demo_mqtt_mutual_auth` 分别对应 http demo 和 mqtt demo，选择其中一个 demo
2. 找到 `demo_http_mutual_auth/keil_ln882h_rel/firmware_XIP.uvprojx` 文件

    或者找到 `demo_mqtt_mutual_auth/keil_ln882h_rel/firmware_XIP.uvprojx` 文件。

3. 双击打开
4. 使用 MDK 进行编译、下载

## GCC 编译

0. 在 SDK 根目录下的 CMakeLists.txt 文件内配置 project 为 `aws_iot_core`

    `set(USER_PROJECT  aws_iot_core)`

1. 选择具体的 demo（demo_http_mutual_auth 或者 demo_mqtt_mutual_auth）

    默认配置选择了 demo_mqtt_mutual_auth 。  
    用户可以在当前目录下的 ProjModuleCollect.cmake 文件的头部选择 AWS_DEMO，注释掉不需要的 demo，打开要使用的 demo 。

2. 命令行进入 SDK 根目录（project 同级目录）
3. 命令行输入 `python start_build.py rebuild` 命令开始编译
4. 编译成功后，在 SDK 根目录 `build-aws_iot_core-release` 里生成设备固件

    设备固件位置 `build-aws_iot_core-release/bin/flashimage.bin` 。
