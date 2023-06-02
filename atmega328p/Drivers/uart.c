#include "uart.h"

#define OSC_CPU 16000000UL

//UART INIT IMPL
void init( struct uart_config* _uart ) {
     UCSR0A &= 0x00; //clear 
     UCSR0B &= 0x00; //clear
     UCSR0C &= 0x00; //clear
     
     uint16_t ubrr_value = 0;
     
     //enable Sync Recv Transmit
     if(_uart->sync && !(_uart->double_speed)) { 
        UCSR0C |=  (1 << UMSEL00);
        UCSR0C &= ~(1 << UMSEL01);
        ubrr_value = OSC_CPU / 2 / _uart->baud_rate - 1;
     } else if ( !(_uart->sync) &&  _uart->double_speed ) {
        UCSR0A |= (1 << U2X0);
        ubrr_value = OSC_CPU / 8 / _uart->baud_rate - 1;
     } else if (!(_uart->sync) && !(_uart->double_speed)) {
     	ubrr_value = OSC_CPU / 16 / _uart->baud_rate  - 1;
     } else { //default
     	ubrr_value = OSC_CPU / 16 / _uart->baud_rate  - 1;
     }
     
     //enable  Multi-processor Communication Mode
     if(_uart->multi_proc_comm_mode) UCSR0A |= (1 << MPCM0);
     
     //Set baud rate 
     UBRR0H = (uint8_t)( ubrr_value >> 8 );
     UBRR0L = (uint8_t) ubrr_value;
     
     // Set RX & TX enable
     UCSR0B = (1 << TXEN0) | (1 << RXEN0); 
     
     //set frame_format
     if( _uart->data_len > 0 && _uart->data_len <= 3 ) UCSR0C = (_uart->data_len << UCSZ00);

     
     //set usbs0 
     if( _uart->stop_bit ) UCSR0C = (_uart->stop_bit << USBS0);
     
     //set parity
     if( _uart->parity > 1 && _uart->parity <= 3 ) UCSR0C = (_uart->parity << UPM00);
     
     //enable global interruption
     sei();
}

//UART TRANSMIT IMPL
void transmit( uint8_t _data ) {
   // Wait for empty transmit buffer 
   while (!(UCSR0A & (1 << UDRE0)));
   
   // Put data into buffer, sends the data 
   UDR0 = _data;
}

//UART RECEIVE IMPL
uint8_t receive( void ) {
    while (!(UCSR0A & (1 << RXC0))); // wait for data to be received
        
    return UDR0; //read data 
}

//UART FLUSH IMPL
void flush( void ){

}

struct uart* create( struct uart_config* _conf ) {
   struct uart* uart = (struct uart*)malloc(sizeof(struct uart));
   uart->conf       = _conf;
   uart->uart_init  = &init;
   uart->uart_tx    = &transmit;
   uart->uart_rx    = &receive;
   uart->uart_flush = &flush;
   
   return uart;
}

