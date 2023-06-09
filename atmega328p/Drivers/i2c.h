#include<avr/io.h>
#include<stdlib.h>
#include<stdint.h>

#define OSC_FRQ   16000000UL
#define SCL_FRQ     100000UL
#define MASK_TWSR       0xF8
#define TX_BIT             0
#define RX_BIT             1

struct i2c_conf {
        //Mode: Master or Slave
	uint8_t mode;
	//Prescaler: division
	uint8_t prescaler;
	//Slave Address
	uint8_t slave_addr;
};

struct i2c {
        //Init
	void    (*init)(struct i2c_conf*);
	// Transmission
	void    (*write)(uint8_t, uint8_t); // transmission 
	void    (*write_nbytes)(uint8_t, uint8_t*, uint8_t);
	// Reception
	uint8_t (*read)(void);       // read data from slave
};

//construct i2c interface
struct i2c* contruct_i2c();
