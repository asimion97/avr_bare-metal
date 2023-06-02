//avr-objcopy -O ihex AVR_PRJ_bare-metal uart_driver.hex     
#include "Drivers/uart.h"
#include <stdlib.h>

int main(void) {
    struct uart_config uart_conf;    
    uart_conf.baud_rate = 9600;
    uart_conf.data_len = 3;//bit data
    uart_conf.stop_bit = 0;//1bit stop
    uart_conf.sync = 0;//async
    uart_conf.double_speed = 0;//no double speed
    uart_conf.multi_proc_comm_mode = 0;//no mpcm
    uart_conf.parity  = 0;//parity even = 2, parity odd = 3, no parity = 0
    
    struct uart* uart0 = create(&uart_conf);
    uart0->uart_init(uart0->conf);
    
    while(1){
	    uart0->uart_tx(48);
	    _delay_ms(1000);
	    uart0->uart_tx(49);
	    _delay_ms(1000);
	    uart0->uart_tx(50);
	    _delay_ms(1000);
    }
    free(uart0);
   
    return 0;
}

