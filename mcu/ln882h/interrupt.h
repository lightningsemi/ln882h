#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_


//------------------------------------------------------------------------------
//
//   Weak attribute allows to replace default handler with the user's one
//
#define WEAK __attribute__ ((weak))
//------------------------------------------------------------------------------

extern unsigned long __top_of_stack[];
//------------------------------------------------------------------------------
//
//   Vector table item. Can be pointer to function or plain address value
//
typedef void (*intfun_t)(void);
typedef struct
{
    unsigned long *tos;
    intfun_t      vectors[101];
}__vector_table_t;

//------------------------------------------------------------------------------
//
//   Startup handler
//
void Reset_Handler(void);

//------------------------------------------------------------------------------
//
//   Cortex-M internal exceptions
//
WEAK void NMI_Handler(void);
WEAK void HardFault_Handler(void);
WEAK void MemManage_Handler(void);
WEAK void BusFault_Handler(void);
WEAK void UsageFault_Handler(void);
WEAK void SVC_Handler(void);
WEAK void DebugMon_Handler(void);
WEAK void PendSV_Handler(void);
WEAK void SysTick_Handler(void);

//------------------------------------------------------------------------------
//
//   Controller specific peripheral interrupts
//
WEAK void WDT_IRQHandler(void);
WEAK void EXT_IRQHandler(void);
WEAK void RTC_IRQHandler(void);
WEAK void RFSLP_IRQHandler(void);
WEAK void MAC_IRQHandler(void);

WEAK void BLE_WAKE_IRQHandler(void);
WEAK void BLE_ERR_IRQHandler(void);
WEAK void BLE_MAC_IRQHandler(void);

WEAK void DMA_IRQHandler(void);
WEAK void QSPI_IRQHandler(void);
WEAK void SDIO_F0_IRQHandler(void);
WEAK void SDIO_F1_IRQHandler(void);
WEAK void SDIO_F2_IRQHandler(void);

WEAK void FPIXC_IRQHandler(void);
WEAK void FPOFC_IRQHandler(void);
WEAK void FPUFC_IRQHandler(void);
WEAK void FPIOC_IRQHandler(void);
WEAK void FPDZC_IRQHandler(void);
WEAK void FPIDC_IRQHandler(void);

WEAK void I2C_IRQHandler(void);
WEAK void SPI0_IRQHandler(void);
WEAK void SPI1_IRQHandler(void);
WEAK void UART0_IRQHandler(void);
WEAK void UART1_IRQHandler(void);
WEAK void UART2_IRQHandler(void);

WEAK void ADC_IRQHandler(void);
WEAK void WS_IRQHandler(void);
WEAK void I2S_IRQHandler(void);
WEAK void GPIOA_IRQHandler(void);
WEAK void GPIOB_IRQHandler(void);

WEAK void TIMER0_IRQHandler(void);
WEAK void TIMER1_IRQHandler(void);
WEAK void TIMER2_IRQHandler(void);
WEAK void TIMER3_IRQHandler(void);

WEAK void ADV_TIMER_IRQHandler(void);
WEAK void AES_IRQHandler(void);
WEAK void TRNG_IRQHandler(void);
WEAK void PAOTD_IRQHandler(void);


#endif /* _INTERRUPT_H_ */
