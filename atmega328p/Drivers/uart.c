#include "uart.h"
#include<avr/io.h>
#include<avr/interrupt.h>
#include<stdlib.h>

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

//UART INIT IMPL
void init( struct uart_config _uart ) {
     UCSR0A = 0; //clear 
     UCSR0B = 0; //clear
     UCSR0C = 0; //clear
     
     uint16_t ubrr_value = 0;
     
     //enable Sync Recv Transmit
     if(_uart.sync) { 
        UCSR0C |=  (1 << UMSEL00);
        UCSR0C |= ~(1 << UMSEL01);
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
     UBRR0H = (uint8_t)( ubrr_value >> 8 );
     UBRR0L = (uint8_t) ubrr_value;
     
     // Set RX & TX enable
     UCSR0B |= (1 << TXEN0) | (1 << RXEN0); 
     
     //set frame_format
     switch( _uart.frame_format ) {
        case  0: //5 + 1bit_stop
        	 break;
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
     
     //set parity
     switch(_uart.parity){
     	case 0: //NO PARITY CHECK
    		break;
    	case 1: //ODD PARITY CHECK
    		UCSR0C |= (1 << UPM01);
    		break;
    	case 2: //EVEN PARITY CHECK
    	        UCSR0C |= ( 1 << UPM01) | (1 << UPM00);
    		break;
     }
     
     //enable global interruption
     sei();
}

//UART TRANSMIT IMPL
void transmit( uint8_t _data ) {
   // Wait for empty transmit buffer 
   while (!(UCSR0A & (1 << UDRE0)));
   
   //check if we have frames with 9 data bits 
   if(UCSZ02 && UCSZ01 && UCSZ00 ) {
   	UCSR0B &= ~( 1 << TXB80);
   	if(_data & 0x0100) UCSR0B |= (1 << TXB80);
   }
   
   // Put data into buffer, sends the data 
   UDR0 = _data;
}

//UART RECEIVE IMPL
uint8_t receive( void ) {
    while (!(UCSR0A & (1 << RXC0))); // wait for data to be received
    
    //check if we have frames with 9 data bits 
    if(UCSZ02 && UCSZ01 && UCSZ00 ) {
          uint8_t status;
          uint8_t resh;
          uint8_t resl;
          
	 // Get status and 9th bit, then data from buffer 
	 status = UCSR0A;
	 resh = UCSR0B;
	 resl = UDR0;
	 
	 /* If error, return -1 */
	 if (status & (1 << FE0)|(1 << DOR0)|(1 << UPE0)) return -1;
	 
	 /* Filter the 9th bit, then return */
	 resh = (resh >> 1) & 0x01;
	 return ((resh << 8) | resl);
    } 
    
    return UDR0; //read data 
}

//UART FLUSH IMPL
void flush( void ){

}

struct uart* create( struct uart_config _conf ) {
   struct uart* uart = (struct uart*)malloc(sizeof(struct uart));
   uart->conf       = _conf;
   uart->uart_init  = &init;
   uart->uart_tx    = &transmit;
   uart->uart_rx    = &receive;
   uart->uart_flush = &flush;
   
   return uart;
}



