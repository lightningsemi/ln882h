
<table>
    <tr>
        <td>日期</td>
        <td>当前SDK版本</td>
        <td>上游版本</td>
        <td>摘要</td>
    </tr>
    <tr>
        <td>
            2021-12-30 14:30:00
        </td>
        <td>
            v1.1
        </td>
        <td>
            223160a1
        </td>
        <td>
            <b>SDK changelog:</b><br />
            1. 优化 boot 启动时间长的问题，增加 boot 支持降级升级的功能<br />
            2. 完善 newlib 接口<br />
            3. 调整各个工程的 flash 分区结构，降低 boot 分区大小为 24KB<br />
            4. 修复 ap 模式无法使用 iperf 的问题<br />
            5. 增加低功耗组件，完善低功耗功能<br />
            6. 修复 awo 寄存器错误<br />
            7. 更新 adc 校准参数<br />
            8. 增加 SHA1,AES,CRC ROM 代码使用，默认不适用 ROM 代码，可选择性启用<br />
            9. 更新 ota 组件，移除对 root 的耦合<br />
            10. 移除 reboot_trace 组件对 wdt 的耦合<br />
            11. 修复 WiFi 断开连接的时候依旧打印 ip 地址的问题<br />
            <b>wifi changelog:</b><br />
            1. 优化内存，降低 WiFi 1600 内存池数量，降低 sniffer 内存使用数量<br />
            2. 优化 RF 性能，提升 RF 在高低温情况下的稳定性和性能<br />
            3. 优化 RF 校准时间，降低 temp 和 img 校准时间，加快启动速度<br />
            4. 优化 WiFi 发射滤波器和发射动态功率数据，提升 WiFi 发射性能<br />
            5. 优化 WiFi 低功耗机制，支持 40M 到 32K 时钟切换<br />
            8. 修复路由器一直发 moredata 导致 MCU 无法休眠的问题<br />
            9. 修复 WiFi SCAN 过程中记录路由器信息的时候会遗漏进而导致内存泄漏的问题<br />
            10. 修复不支持 WEP40 和 WEP104 的问题<br />
            11. 修复处理 CFG_MSG_INFO 消息的时候，在处理各种状态 cb 的时候可能出现的在逻辑死锁问题<br />
            13. 修复 RF/PHY 寄存器、LUT 访问不可靠的问题，防止编译器优化<br />
            14. 修复 ble rssi 不准确问题，涉及 PHYRXLNAGAIN7TABLE_VALUE 寄存器<br />
            16. 增加 LUT_HW_ERROR 错误类型<br />
            17. 增加 WID_ALLOW_CPU_SLEEP WID，用于低功耗控制<br />
            18. 增加 11n 使能接口 wifi_set_11n_enable<br />
            19. 移除 WID_DHCP_STATUS WID，弃用<br />
            <b>ble changelog:</b><br />
            1. 移除 common 目录<br />
            2. 使用指针替代结构体里的零数组<br />
            3. 增加 ble_arch 目录，用于平台相关的处理<br />
            6. 更新 mac 地址配置接口 ln_ble_mac_set ln_ble_mac_get<br />
            7. 修复 rssi 不准确、不稳定的问题<br />
            8. 禁用内部 NVDS 功能，禁用 AOA 和 AOD 功能
        </td>
    </tr>
    <tr>
        <td>
            2021-10-24 18:19:00
        </td>
        <td>
            v1.0
        </td>
        <td>
            3037bbcc
        </td>
        <td>
            2.1. 优化温度补偿校准.<br/>
            2.2. 修复隐藏连接隐藏AP的问题.<br/>
            2.3. 优化改善WiFi BLE共存稳定性.<br/>
            2.4. 修复暗室测试过程中非标速率集导致的死循环问题.<br/>
            2.5. WiFi发射功率依据eFuse标定值.<br/>
            2.6. 更新WiFi tx power VS time. <br/>
            2.7. 优化 11b EVM.<br/>
            2.8. 增加在不同速率集下使用不同的发射功率的功能.<br/>
            2.9. 优化 ram/codesize.<br/>
            3. 实现WiFi BLE共存，优化改善共存稳定性。<br/>
            4. wifi/ble stack (gcc/armcc)lib 使用O1编译.<br/>
            5. 更新peripher hal driver.<br/>
            6. 更新peripher driver test.<br/>
            7. 修复部分编译警告.<br/>
            8. 新增keil MDK(win平台)，gcc(win/linux平台)编译自动化检查功能.<br/>
            9. 新增ble_mcu_data_trans, combo_mcu_basic_example示例工程，支持keil/gcc编译.<br/>
            10. 新增Tuya OS的GCC编译配置入口（已作为submodule适配到Tuya OS SDK）<br/>
            11. 更新部分示例工程的分区表配置.<br/>
            12. 部分代码使用Apache License V2.0<br/>
            13. 更新CMake list.
        </td>
    </tr>
    <tr>
        <td>
            2021-10-16 17:58:00
        </td>
        <td>
            v0.2
        </td>
        <td>
            eddb49be
        </td>
        <td>
            1. 正式发布ln882h wifi mac stack，完全解除与platform（soc、os、net...）的耦合，全面深度重构和优化，精简api。<br />
            2. 全新的cmake构建体系 。<br />
            3. 更新keil MDK(win平台)，gcc(win/linux平台)开发环境指导文档。<br />
            4. 增加LN882H分区地址辅助计算工具。<br />
            5. 全新的编译预处理、后处理脚本工具体系。<br />
            6. 新版AT命令组件。<br />
            7. 修复armcc5，gcc编译警告若干。<br />
            8. 更新peripher hal driver.<br />
            9. 更新Jflash烧录配置参数。
        </td>
    </tr>
    <tr>
        <td>
            2021-09-30 16:10:00
        </td>
        <td>
            v0.1
        </td>
        <td>
            38a947c8
        </td>
        <td>
            LN882H SDK 初始版本
        </td>
    </tr>
</table>
