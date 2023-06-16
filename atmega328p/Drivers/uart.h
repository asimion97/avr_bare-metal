#pragma once
#include<stdint.h>
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<stdlib.h>

//frame format:
/*    USBS0   UCSZ02   UCSZ01   UCSZ00    
	0       0        0        0          5 + 1bit_stop      
        0       0        0        1          6 + 1bit_stop      
        0       0        1        0          7 + 1bit_stop      
        0       0        1        1          8 + 1bit_stop       
/------------------------------------------------------------------/
	1       0        0        0          5 + 2bit_stop      
        1       0        0        1          6 + 2bit_stop      
        1       0        1        0          7 + 2bit_stop           
        1       0        1        1          8 + 2bit_stop     
*/
//

struct uart_config {
     uint16_t baud_rate;
     uint8_t  data_len; // 0-5b 1-6b 2-7b 3-8b
     uint8_t  stop_bit; //0-1bit 1-2bit
     uint8_t  sync;// 0 or 1
     uint8_t  double_speed;//0 or 1
     uint8_t  multi_proc_comm_mode;// 0 or 1
     uint8_t  parity;//0 no parity, 2 parity  even, 3 parity odd
};

struct uart {
     struct uart_config* conf;
     void     (*uart_init)( struct uart_config* );  
     void     (*uart_tx)( uint8_t) ;
     uint8_t  (*uart_rx)( void );
     void     (*uart_flush)(void);
};

struct uart* create( struct uart_config* _conf );
