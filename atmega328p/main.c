//avr-objcopy -O ihex AVR_PRJ_bare-metal uart_driver.hex     
#include "Drivers/uart.h"
#include "Drivers/i2c.h"

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
    
    struct i2c_config i2c_conf;
    i2c_conf.mode = 0x01;//master mode
    i2c_conf.prescaler = 0x00;//no prescaling
    
    struct i2c* i2c0 = construct_i2c();
    i2c0->init();
    uint8_t saddr = scan_i2c_slave();
    
    uart0->uart_tx(saddr);
    
    free(uart0);
    free(i2c0);
    
    return 0;
}

