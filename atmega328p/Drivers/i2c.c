#include "i2c.h"

/////////////////AUX FUNCT FOR TRANSMISSION ////////////////////////////////////
void _start_transmission(uint8_t _dest_addr, uint8_t direction) {

    TWCR = (1 << TWSTA) ; // The application writes the TWSTA bit to one when it desires to become a master on the 2-wire serial bus.
    
    while (!(TWCR & (1 << TWINT)));
    
    if ((TWSR & MASK_TWSR) != START) ERROR();
    
    TWDR = ( _dest_addr << 1 ) | direction;
    
    if ((TWSR & MASK_TWSR) != MT_SLA_ACK) ERROR();
    
}

void _transmission(uint8_t _data) {

	TWDR = _data;
	
	while (!(TWCR & (1 << TWINT)));
	
	if ((TWSR & MASK_TWSR)!= MT_DATA_ACK) ERROR();
	
}

void _stop_transmission() {
	TWCR = (1 << TWSTO);
}

////////////////AUX FUNC FOR RECEPTION///////////////////////////////////////////
uint8_t _reception() {
    uint8_t data_rx;
	
	data_rx = TWDR;
	
	while (!(TWCR & (1 << TWINT)));
	
	if ((TWSR & MASK_TWSR)!= MT_DATA_ACK) ERROR();
	
	return data_rx;
}

void _stop_reception() {
	
}

///////////////////// I2C init fct //////////////////////////////////////////////
void init(struct i2c_conf *_conf) {
    //clear TWBR -- default value
    TWBR &= 0x00;
    
    //clear TWCR -- default value
    TWCR &= 0x00;
    
    //clear TWSR -- default value
    TWSR &= 0xfc;
    
    //clear TWAR -- default value
    TWAR &= 0xff;
    
    //enble I2C + interrupts
    TWCR = (1 << TWEN) | (1 << TWIE);
    
	if( _conf->mode == 0x01 ) { 
	//Master
		//set prescaler
    	if( _conf->prescaler > 0 ) TWSR = ( _conf->prescaler << TWPS0 );
	
		//set I2C bit rate
		TWBR = ((OSC_FRQ / SCL_FRQ) - 16) / 2;
		
	} else if ( _conf->mode == 0x02 ) {
	//Slave
		//set slave addr
		TWAR = ( _conf->slave_addr << 1 );
	}
	
}

//////////// I2C master transmission func ////////////////////////////////////////
void master_TX(uint8_t slave_addr, uint8_t data) {
         _start_transmission(slave_addr, TX_BIT);
          
         _transmission(data);
     
         _stop_transmission();
}

void master_TX_Nbytes(uint8_t slave_addr, uint8_t* data, uint8_t n_bytes) {
         _start_transmission(slave_addr, TX_BIT);
          
          for(int i = 0; i < n_bytes; i++) _transmission(*(data + i));
     
         _stop_transmission();
}

//////////// I2C master reception func //////////////////////////////////////////
uint8_t master_RX(uint8_t slave_addr) {
     _start_transmission(slave_addr, RX_BIT);
     
	 uint8_t i2c_rx = _reception();
	 
	 _stop_transmission();
	 
	 return i2c_rx;
}

///////////////// CONSTRUCT I2C Structure ///////////////////////////////////////
struct i2c* contruct_i2c() {
	struct i2c* i2c_device   = (struct i2c*)malloc(struct i2c);
	i2c_device->init         = init;
	i2c_device->write        = master_TX;
	i2c_device->write_nbytes = master_TX_Nbytes;
	i2c_device->read         = master_RX;
	return i2c_device;
}




