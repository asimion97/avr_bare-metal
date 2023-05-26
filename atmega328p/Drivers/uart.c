#include "uart.h"
#include<avr/io.h>

//frame format:
/*    USBS0   UCSZ02   UCSZ01   UCSZ00    
	0       0        0        0          5 + 1bit_stop      0 
        0       0        0        1          6 + 1bit_stop      1
        0       0        1        0          7 + 1bit_stop      2
        0       0        1        1          8 + 1bit_stop      3
        0       1        1        1          9 + 1bit_stop      7
/------------------------------------------------------------------/
	1       0        0        0          5 + 2bit_stop      8
        1       0        0        1          6 + 2bit_stop      9
        1       0        1        0          7 + 2bit_stop     10      
        1       0        1        1          8 + 2bit_stop     11
        1       1        1        1          9 + 2bit_stop     15   
*/
//

void init( struct uart _uart ) {
     UCSR0A = 0; //clear 
     UCSR0B = 0; //clear
     UCSR0C = 0; //clear
     
     uint16_t ubrr_value = 0;
     
     //enable Sync Recv Transmit
     if(_uart.sync) { 
        UCSR0C |=  (1 << UMSELn0);
        UCSR0C |= ~(1 << UMSELn1);
        //enable double speed 
        if(_uart.double_speed) UCSR0A |= (1 << U2X0);
     }
     
     //enable  Multi-processor Communication Mode
     if(_uart.multi_proc_comm_mode){
     	UCSR0A |= (1 << MPCM0);
     }
     
     //compute baudrate 
     if ( ~(U2X0) )  ubrr_value = F_CPU / 16 / _uart.baud_rate  - 1;
     else ubrr_value = F_CPU / 8 / _uart.baud_rate - 1;
     
     //Set baud rate 
     UBRR0H = (uint8_t)(ubrr >> 8);
     UBRR0L = (uint8_t) ubrr;
     
     // Set RX & TX enable
     UCSR0B |= (1 << TXEN0) | (1 << RXEN0); 
     
     //set frame_format
     switch( _uart.frame_format ) {
        case  1: UCSR0C |= ~(1 << USBS0) | ~(1 << UCSZ02)  | ~(1 << UCSZ01) | (1 << UCSZ00);
        	 break;
        case  2: UCSR0C |= ~(1 << USBS0) | ~(1 << UCSZ02)  | (1 << UCSZ01) | ~(1 << UCSZ00);
        	 break;
        case  3: UCSR0C |= ~(1 << USBS0) | ~(1 << UCSZ02)  | (1 << UCSZ01) |  (1 << UCSZ00);
        	 break;
        case  7: UCSR0C |= ~(1 << USBS0) |  (1 << UCSZ02)  | (1 << UCSZ01) |  (1 << UCSZ00);
        	 break;
        case  8: UCSR0C |=  (1 << USBS0) | ~(1 << UCSZ02)  | ~(1 << UCSZ01) | (1 << UCSZ00);
        	 break;
        case  9: UCSR0C |=  (1 << USBS0) | ~(1 << UCSZ02)  | ~(1 << UCSZ01) | (1 << UCSZ00);
        	 break;
        case 10: UCSR0C |=  (1 << USBS0) | ~(1 << UCSZ02)  |  (1 << UCSZ01) | ~(1 << UCSZ00);
        	 break;
        case 11: UCSR0C |=  (1 << USBS0) | ~(1 << UCSZ02)  |  (1 << UCSZ01) |  (1 << UCSZ00);
      	         break;
        case 15: UCSR0C |=  (1 << USBS0) |  (1 << UCSZ02)  |  (1 << UCSZ01) |  (1 << UCSZ00);
       		 break;
     }
     
}

void transmit( uint32_t _data ) {

}

uint32_t receive( void ) {

}

struct uart* create( struct uart_config _conf ) {

}
