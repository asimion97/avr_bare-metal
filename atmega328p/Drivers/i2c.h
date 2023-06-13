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
#define I2C_OK           200


struct i2c_config {
        //Mode: Master(0x01) or Slave(0x02)
	uint8_t mode;
	//Prescaler: division
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
	uint8_t (*master_read)(void);       // read 8bit from slave
	//TODO: Slave Transmission
	//void    (*slave_write)(uint8_t, uint8_t);
	//TODO: Slave Reception
	//uint8_t (*slave_read)(void);
};

//construct i2c interface
struct i2c* construct_i2c(void);
//scan i2c slave;
uint8_t scan_i2c_slave();
