#pragma once
#include "../Drivers/uart.h"

#define  OK          0
#define NOK          1
#define NULL_CHAR    0
#define NEW_LINE     10

uint8_t  sdispf(struct uart*, char, char*);

uint8_t  sreadf(struct uart*, char, void*);

