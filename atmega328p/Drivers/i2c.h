#include<avr/io.h>
#include<stdlib.h>
#include<stdint.h>


struct i2c_conf {
	uint8_t mode;
	uint8_t prescaler;
	uint8_t slave_addr;
};

struct i2c {
        //Init
	void    (*i2c_init)(struct i2c_conf*);
	// Transmission
	void    (*i2c_start_tx)(uint8_t); // start transmission 
	void    (*i2c_write)(uint8_t);    // write data to slave
	void    (*i2c_stop_tx)(void);     // stop transmission
	// Reception
	void    (*i2c_start_rx)(uint8_t); // start recv
	void    (*i2c_read)(uint8_t);     // read data from slave
	void    (*i2x_stop_rx)(uint8_t);  // stop recv
};

//construct i2c interface
struct i2c* contruct_i2c(struct i2c_conf* config);
