#pragma once

struct uart_config{
     uint16_t baud_rate;
     uint8_t  frame_format;
     uint8_t  sync;
     uint8_t  double_speed;
     uint8_t  multi_proc_comm_mode;
};

struct uart{
     struct uart_config conf;
     void     (*uart_init)( struct uart );  
     void     (*uart_tx)( uint32_t) ;
     uint32_t (*uart_rx)( void );
};

struct uart* create( struct uart_config _conf );
