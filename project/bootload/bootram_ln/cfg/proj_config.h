#ifndef _PROJ_CONFIG_H_
#define _PROJ_CONFIG_H_

#define DISABLE                                 (0)
#define ENABLE                                  (1)

#define RUN_ON_CHIP                             (0)
#define RUN_ON_FPGA                             (1)
#define RUN_ON_SIMU                             (2)

#define EXEC_ENV                                RUN_ON_FPGA


/*
 * Clock settings section
 * Note:
 *      
 */
#define XTAL_CLOCK                              (40000000)
#define RCO_CLOCK                               (32000)
#define PLL_CLOCK                               (160000000)

#define SOURCE_CLOCK                            XTAL_CLOCK
#define USE_PLL                                 DISABLE

#if (EXEC_ENV == RUN_ON_CHIP)

#define SYSTEM_CLOCK                            (SOURCE_CLOCK)
#define AHB_CLOCK                               (SYSTEM_CLOCK)
#define APB_CLOCK                               (AHB_CLOCK)
#define QSPI_CLK                                 AHB_CLOCK

#define PLL_DIV                                 0
#define AHB_DIV                                 0
#define APB_DIV                                 0

#else

#define SYSTEM_CLOCK                            (40000000)
#define AHB_CLOCK                               (40000000)
#define APB_CLOCK                               (40000000)
#define QSPI_CLK                                 20000000
#endif



/*
 * Module enable/disable control
 */
#define FULL_ASSERT   DISABLE
#define PRINTF_OMIT   DISABLE     // when release software, set 1 to omit all printf logs
#define PRINT_TO_RTT  DISABLE     // 1: print to segger rtt   0: print to uart1

#define FLASH_XIP     DISABLE

/*
 * Hardware config
 */
#define CFG_UART0_TX_BUF_SIZE      256
#define CFG_UART0_RX_BUF_SIZE      512

#define LOG_EN                     ENABLE
#define CFG_UART_BAUDRATE_LOG      115200




#endif /* _PROJ_CONFIG_H_ */


