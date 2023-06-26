#pragma once
#include<avr/io.h>
#include<stdlib.h>
#include<stdint.h>
#include<util/twi.h>
#include<avr/interrupt.h>

#define OSC_FRQ   16000000UL
#define SCL_FRQ     100000UL

#define MASK_TWSR       0xF8
#define TX_BIT             0
#define RX_BIT             1

#define I2C_TOK         0x06 // i2c transmission established OK
#define I2C_DOK         0x07 // i2c transmission data OK

#define MT_MODE         0x01 // master transmitter
#define MR_MODE         0x03 // master receiver
#define SR_MODE         0x02 // slave receiver
#define ST_MODE         0x04 // slave transmitter 

struct i2c_config {
    //Mode: Master Transmitter(0x01), Ma or Slave(0x02)
	uint8_t mode;
	//Prescaler: division (00 ,01 ,10 ,11)
	uint8_t prescaler;
	//Slave Address
	uint8_t slave_addr;
};

struct i2c {
    //Init
	void    (*init)(struct i2c_config*);
	// Master Transmission
	void    (*master_write)(uint8_t, uint8_t);
	void    (*master_write_nbytes)(uint8_t, uint8_t*, uint8_t);
	// Master Reception
	uint8_t (*master_read)(uint8_t);       // read 8bit from slave
	//TODO: Slave Transmission
	//void    (*slave_write)(uint8_t, uint8_t);
	//TODO: Slave Reception
	//uint8_t (*slave_read)(void);
};

//construct i2c interface
struct i2c* construct_i2c(void);
//scan
uint8_t  get_first_addr_i2c_slave();
uint8_t* get_all_addr_i2c_slave();

