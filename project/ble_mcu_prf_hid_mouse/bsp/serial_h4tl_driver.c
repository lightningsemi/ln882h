#include "proj_config.h"
#include "ln882h.h"
#include "hal/hal_uart.h"
#include "hal/hal_gpio.h"
//#include "reg_cmp.h"
#if defined(CFG_PATCH)
#include "patch.h"
#endif
#include <stdbool.h>
#define SW_FIFO_DEPTH          (14)
#define HCI_UART_BASE          (UART0_BASE)

/// UART TX RX Channel
struct uart_txchannel
{
    /// call back function pointer
    void (*callback) (void*, uint8_t);
    /// Dummy data pointer returned to callback when operation is over.
    void* dummy;
    uint32_t remain_size;
    const uint8_t* remain_data;
};

struct uart_rxchannel
{
    /// call back function pointer
    void (*callback) (void*, uint8_t);
    /// Dummy data pointer returned to callback when operation is over.
    void* dummy;
    uint32_t remain_size;
    uint8_t* remain_data;
};

/// UART environment structure
struct uart_env_tag
{
    /// tx channel
    struct uart_txchannel tx;
    /// rx channel
    struct uart_rxchannel rx;
};


volatile struct uart_env_tag uart_env;
#define UART_DEBUG 1

#if (UART_DEBUG)
#define UART_BUF_LEN		1024
uint8_t uart_tx_buf[UART_BUF_LEN];
uint8_t uart_rx_buf[UART_BUF_LEN];
uint16_t uart_tx_index;
uint16_t uart_rx_index;
#endif

static void uart0_gpio_init(void)//cfg uart tx,rx gpio
{
    hal_gpio_pin_afio_select(GPIOB_BASE,GPIO_PIN_9,UART0_TX);
    hal_gpio_pin_afio_select(GPIOB_BASE,GPIO_PIN_8,UART0_RX);

    hal_gpio_pin_afio_en(GPIOB_BASE,GPIO_PIN_8,HAL_ENABLE);
    hal_gpio_pin_afio_en(GPIOB_BASE,GPIO_PIN_9,HAL_ENABLE);
}

void hci_uart_init(uint32_t baud)
{
    uart_init_t_def  init_cfg = {0};
    init_cfg.baudrate  = baud;//115200 921600 2000000
    init_cfg.word_len  = UART_WORD_LEN_8;
    init_cfg.parity    = UART_PARITY_NONE;
    init_cfg.stop_bits = UART_STOP_BITS_1;

    hal_uart_init(HCI_UART_BASE, &init_cfg);

	//init uart0 gpio pin
    uart0_gpio_init();

    //hal_uart_hardware_flow_rts_en(HCI_UART_BASE, HAL_ENABLE);
	
    hal_uart_rx_mode_en(HCI_UART_BASE, HAL_ENABLE);
    hal_uart_tx_mode_en(HCI_UART_BASE, HAL_ENABLE);
    hal_uart_en(HCI_UART_BASE, HAL_ENABLE);

	hal_uart_rx_fifo_it_trig_level_set(HCI_UART_BASE, 0);
	hal_uart_it_disable(HCI_UART_BASE, USART_IT_RXNE);//cch add for bootrom
    NVIC_EnableIRQ(UART0_IRQn);  

#if (UART_DEBUG)
	uart_tx_index = 0;
	uart_rx_index = 0;
#endif 
  
}

void hci_uart_flow_on(void)
{
	hal_uart_hardware_flow_rts_en(HCI_UART_BASE, HAL_ENABLE);
}

bool hci_uart_flow_off(void)
{
    bool flow_off = true;
    
    do
    {
        if(hal_uart_rx_fifo_level_get(HCI_UART_BASE))
        {
            flow_off = false;
            break;
        }
        
        
        if(hal_uart_tx_fifo_level_get(HCI_UART_BASE))
        {
            flow_off = false;
            break;
        }
        hal_uart_hardware_flow_rts_en(HCI_UART_BASE, HAL_DISABLE);
    }while(0);

    return flow_off;
}

void hci_uart_read(uint8_t *bufptr, uint32_t size, void (*callback) (void*, uint8_t), void* dummy)
{
    // Sanity check
    if(!bufptr || !size || !callback)
        return;
    
    uart_env.rx.callback = callback;
    uart_env.rx.dummy    = dummy;
    uart_env.rx.remain_size = size;
    uart_env.rx.remain_data = bufptr;

	if (size > SW_FIFO_DEPTH) {
        hal_uart_rx_fifo_it_trig_level_set(HCI_UART_BASE, (SW_FIFO_DEPTH-1));
    } else {
        hal_uart_rx_fifo_it_trig_level_set(HCI_UART_BASE, (size-1));
    }

    hal_uart_it_en(HCI_UART_BASE, USART_IT_RXFIFO_AF);
}

void hci_uart_write(uint8_t *bufptr, uint32_t size, void (*callback) (void*, uint8_t), void* dummy)
{
#if defined(CFG_PATCH)
	typedef int (*Patch_hci_uart_write)(uint8_t *bufptr, uint32_t size, void (*callback) (void*, uint8_t), void* dummy);
	#ifdef PATCH_FUNC_UART0_IRQHandler
		if((Patch_params->patch_flag2&(1<<(PATCH_FUNC_HCI_UART_WRITE-32)))&&(Patch_params->patch_func[PATCH_FUNC_HCI_UART_WRITE]))
			if( ((Patch_hci_uart_write)(Patch_params->patch_func[PATCH_FUNC_HCI_UART_WRITE]))(bufptr,size,callback,dummy) )
				return ;
	#endif
#endif
	int i = 0;
	uint8_t ch;

	if(uart_env.tx.remain_size || uart_env.tx.callback)
		while(1);
    
    // Sanity check
    if(!bufptr || !size || !callback)
        return;
	
    GLOBAL_INT_DISABLE();

    uart_env.tx.callback = callback;
    uart_env.tx.dummy    = dummy;
	uart_env.tx.remain_data    = bufptr;
	uart_env.tx.remain_size    = size;

	if(size <= SW_FIFO_DEPTH )
	{
		for(i = 0; i < size; i++)
		{
			ch = bufptr[i];
			#if (UART_DEBUG)
			uart_tx_buf[uart_tx_index++] = ch;
			if(uart_tx_index == UART_BUF_LEN)
				uart_tx_index = 0;
			#endif
            hal_uart_send_data(HCI_UART_BASE,ch);
		}

		uart_env.tx.remain_data = NULL;
		uart_env.tx.remain_size = 0;
	}
	else
	{
		for(i = 0; i < SW_FIFO_DEPTH; i++)
		{
			ch = bufptr[i];
			#if (UART_DEBUG)
			uart_tx_buf[uart_tx_index++] = ch;
			if(uart_tx_index == UART_BUF_LEN)
				uart_tx_index = 0;
			#endif
            hal_uart_send_data(HCI_UART_BASE,ch);
		}

		uart_env.tx.remain_data += SW_FIFO_DEPTH;
		uart_env.tx.remain_size -= SW_FIFO_DEPTH;
	}
	
	/* Enable TX empty interrupts. */
	hal_uart_it_en(HCI_UART_BASE, USART_IT_TC);
	
	GLOBAL_INT_RESTORE();
}


static uint8_t read_data(uint8_t rd_num)
{
    uint8_t rcvd_len = 0x00;
    uint8_t *bufptr = uart_env.rx.remain_data;
    // Sanity check
    if(!bufptr)
    return 0;
    
    while(rcvd_len < rd_num)
    {
        *bufptr = hal_uart_recv_data(HCI_UART_BASE);
		#if (UART_DEBUG)
		uart_rx_buf[uart_rx_index++] = *bufptr;
		if(uart_rx_index == UART_BUF_LEN)
			uart_rx_index = 0;
		#endif
        rcvd_len++;
        bufptr++;
    }
	uart_env.rx.remain_data = uart_env.rx.remain_data + rcvd_len;
    return rcvd_len;
}

static void uart_rx_isr_new(uint8_t *rx_num)
{
    void (*callback)(void*, uint8_t) = NULL;
    void* data = NULL;
	uint8_t rd_cnt = 0;

    //disable signal of UART interrupt
    hal_uart_it_disable(HCI_UART_BASE, USART_IT_RXFIFO_AF);

	if(*rx_num > uart_env.rx.remain_size) {
		rd_cnt = uart_env.rx.remain_size;
    } else {
		rd_cnt = *rx_num;
    }
    
    read_data(rd_cnt);

	uart_env.rx.remain_size = uart_env.rx.remain_size - rd_cnt; 

    //fix uart rx overflow

	if(0 == uart_env.rx.remain_size)
	{         
	    callback = uart_env.rx.callback;
	    data     = uart_env.rx.dummy;
	    if(callback != NULL)
	    {
	        // Clear callback pointer
	        uart_env.rx.callback = NULL;
	        uart_env.rx.dummy    = NULL;
	        
	        // Call handler
	        callback(data, 0);
	    }
	}
	else
	{
        if (uart_env.rx.remain_size > SW_FIFO_DEPTH) {
            hal_uart_rx_fifo_it_trig_level_set(HCI_UART_BASE, (SW_FIFO_DEPTH-1));
        } else {
            hal_uart_rx_fifo_it_trig_level_set(HCI_UART_BASE, (uart_env.rx.remain_size-1));
        }

        hal_uart_it_en(HCI_UART_BASE, USART_IT_RXFIFO_AF);
	}
}

static void uart_tx_isr_new(uint8_t *cha)
{
    void (*callback)(void*, uint8_t) = NULL;
    void* data = NULL;
	int i = 0;
	uint8_t ch;

	if(uart_env.tx.remain_size == 0)
	{
	     callback = uart_env.tx.callback;
	     data     = uart_env.tx.dummy;
	     if(callback != NULL)
	     {
	         // Clear callback pointer
	         uart_env.tx.callback = NULL;
	         uart_env.tx.dummy    = NULL;

			 /* Disable TX empty interrupts. */
			 hal_uart_it_disable(HCI_UART_BASE, USART_IT_TC);
	     
	         // Call handler
	         callback(data, 0);
	     }
	}
	else// uart_env.tx.remain_size > 0
	{
		if(uart_env.tx.remain_size <= SW_FIFO_DEPTH )
		{
			for(i=0;i<uart_env.tx.remain_size;i++)
			{
				ch = uart_env.tx.remain_data[i];
				#if (UART_DEBUG)
				uart_tx_buf[uart_tx_index++] = ch;
				if(uart_tx_index == UART_BUF_LEN)
					uart_tx_index = 0;
				#endif
                hal_uart_send_data(HCI_UART_BASE, ch);
			}

			uart_env.tx.remain_data = NULL;
			uart_env.tx.remain_size = 0;
		}
		else
		{
			for(i=0;i<SW_FIFO_DEPTH;i++)
			{
				ch = uart_env.tx.remain_data[i];
				#if (UART_DEBUG)
				uart_tx_buf[uart_tx_index++] = ch;
				if(uart_tx_index == UART_BUF_LEN)
					uart_tx_index = 0;
				#endif
                hal_uart_send_data(HCI_UART_BASE, ch);
			}

			uart_env.tx.remain_data += SW_FIFO_DEPTH;
			uart_env.tx.remain_size -= SW_FIFO_DEPTH;
		}
	}
}

void hci_uart0_isr_callback(void)
{
#if defined(CFG_PATCH)
	typedef int (*Patch_uart0_irq)(void);
	#ifdef PATCH_FUNC_UART0_IRQHandler
		if((Patch_params->patch_flag&(1<<(PATCH_FUNC_UART0_IRQHandler)))&&(Patch_params->patch_func[PATCH_FUNC_UART0_IRQHandler]))
			if( ((Patch_uart0_irq)(Patch_params->patch_func[PATCH_FUNC_UART0_IRQHandler]))() )
				return ;
	#endif
#endif
	//tx cmp
    if(hal_uart_flag_get(UART0_BASE, USART_FLAG_TC))
	{
        uint8_t tx_char = 0;
        hal_uart_it_flag_clear(HCI_UART_BASE, USART_IT_FLAG_CLEAR_TC);// ln_uart_tccf_setf(UART0_BASE,1);
        uart_tx_isr_new(&tx_char);
	}
	else if(hal_uart_flag_get(HCI_UART_BASE, USART_FLAG_RX_FIFO_ALMOST_FULL))//rx int
	{
        uint8_t rx_num = hal_uart_rx_fifo_level_get(HCI_UART_BASE);
        uart_rx_isr_new(&rx_num);
	}
	else
		while(1);
}

