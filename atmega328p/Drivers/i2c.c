#include "i2c.h"

void init(struct i2c_conf *_conf) {
    //clear TWBR -- default value
    TWBR &= 0x00;
    
    //clear TWCR -- default value
    TWCR &= 0x00;
    
    //clear TWSR -- default value
    TWSR &= 0xfc;
    
    //clear TWAR -- default value
    TWAR &= 0xff;
    
    //enble I2C
    TWCR = (1 << TWEN) | (1 << TWIE);
    
	if(_conf->mode == 0x01) { 
		//set prescaler
    	if(_conf->prescaler > 0) TWSR = (_conf->prescaler << TWPS0);
	
		//set I2C bit rate
		TWBR = ((F_CPU / SCL_FREQUENCY) - 16) / 2;
		
	} else if (_conf->mode == 0x02) {
		//set slave addr
		TWAR = (_conf->slave_addr << 1);
	}
	
}

void start_transmission(uint8_t dest_addr) {
	
}

void transmission(uint8_t data) {
	
}

void stop_transmission() {
	
}




struct i2c* contruct_i2c(struct i2c_conf* config) {
	
}




