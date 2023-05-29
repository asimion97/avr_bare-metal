#pragma once
#include<stdint.h>
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<stdlib.h>
struct uart_config{
     uint16_t baud_rate;
     uint8_t  frame_format;
     uint8_t  sync;// 0 or 1
     uint8_t  double_speed;//0 or 1
     uint8_t  multi_proc_comm_mode;// 0 or 1
     uint8_t  parity;//0 no parity, 1 parity  odd, 2 parity even
};

struct uart{
     struct uart_config conf;
     void     (*uart_init)( struct uart_config );  
     void     (*uart_tx)( unsigned char) ;
     uint8_t  (*uart_rx)( void );
     void     (*uart_flush)(void);
};

struct uart* create( struct uart_config _conf );
