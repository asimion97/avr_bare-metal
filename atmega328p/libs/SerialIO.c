#include "SerialIO.h"

uint8_t  sdispf(struct uart* device, char format, char* data) {
    if(format == 'c') {
       device->uart_tx(*data);
    } else if(format == 'd' || format == 'i' || format == 's') {
       for(char* c = data; *c != '\0'; c++){
           device->uart_tx(*c);
       }   
    } else {
       return NOK;
    }
    return OK;
}


uint8_t  sreadf(struct uart* device, char format, void* buff) {
    uint32_t i = 0;
    if(format == 'c'){
       char c;
       while(c = device->uart_rx() != NEW_LINE) {
        *(char*)buff = c;
       }
       c = device->uart_rx();
       return OK;
    } else if (format == 'd' || format == 'i' ) {
       uint8_t d8;
       while(d8 = device->uart_rx() != NULL_CHAR) {
        *(uint8_t*)buff = d8;      
       }
       d8 = device->uart_rx();
       return OK;
    } else if (format == 's') {
       char c;
       while(c = device->uart_rx() != NEW_LINE) {
        ((char*)buff)[i++] = c;
       }
       c = device->uart_rx();
       ((char*)buff)[i++] = '\0';
       return OK;
    } else if (format == 'D' || format == 'I') {
       uint8_t d8;
       while(d8 = device->uart_rx() != NULL_CHAR) {
        ((uint8_t*)buff)[i++] = d8;      
       }
       d8 = device->uart_rx();
       return OK;
    }
    
    return NOK;
}
