//avr-objcopy -O ihex AVR_PRJ_bare-metal uart_driver.hex     
#include "Drivers/uart.h"

int main(void) {
    struct uart_config uart_conf;
    uart_conf.baud_rate = 9600;
    uart_conf.frame_format = 3;
    uart_conf.sync = 0;
    uart_conf.double_speed = 0;
    uart_conf.multi_proc_comm_mode = 0;
    uart_conf.parity  = 1;
    
    struct uart* uart0 = create(uart_conf);
    uart0->uart_init(uart_conf);
    
    while(1){
    	uart0->uart_tx('A');
    	
    }
    
    return 0;
}

