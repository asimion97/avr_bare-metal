//avr-objcopy -O ihex AVR_PRJ_bare-metal uart_driver.hex     
#include "Drivers/uart.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    struct uart_config uart_conf;
    /* //9600 8N1
    uart_conf.baud_rate = 9600;
    uart_conf.frame_format = 3;
    uart_conf.sync = 0;
    uart_conf.double_speed = 0;
    uart_conf.multi_proc_comm_mode = 0;
    uart_conf.parity  = 0;
    
    struct uart* uart0 = create(uart_conf);
    uart0->uart_init(uart0->conf);
    
    while(1){
    	uart0->uart_tx('A');
    	
        _delay_ms(1000);
        
        uart0->uart_tx(10);
        
        _delay_ms(500);
        
        uart0->uart_tx('B');
        
        _delay_ms(1000);
    }
    */
    
    
    uart_conf.baud_rate = 9600;
    uart_conf.frame_format = 1;
    uart_conf.sync = 0;
    uart_conf.double_speed = 0;
    uart_conf.multi_proc_comm_mode = 0;
    uart_conf.parity  = 0; 
    
    struct uart* uart0 = create(uart_conf);
    uart0->uart_init(uart_conf);
    while(1){
	    uart0->uart_tx(48);
	    _delay_ms(1000);
	    uart0->uart_tx(49);
	    _delay_ms(1000);
	    uart0->uart_tx(50);
    }
    free(uart0);
   
    return 0;
}

