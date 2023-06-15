#include "i2c.h"

/////////////////AUX FUNCT FOR TRANSMISSION ////////////////////////////////////
uint8_t _start_transmission(uint8_t _dest_addr, uint8_t direction) {
    //TWCR &= ~(1 << TWSTO);
    //TWCR |= (1 << TWSTA) ; // The application writes the TWSTA bit to one when it desires to become a master on the 2-wire serial bus.
   // TWCR |= (1<<TWINT);
     TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    uint8_t twcr = TWCR;
    Serial.println(twcr);
    
    while (!(TWCR & (1 << TWINT)));
    
    Serial.println("Start sent");
    
    if ((TWSR & MASK_TWSR) != TW_START) return TW_BUS_ERROR;
    Serial.println("Start ACK");
     
    TWDR |= ( _dest_addr << 1 ) | direction;
    TWCR = (1 << TWINT) | (1 << TWEN);
    
    while (!(TWCR & (1 << TWINT)));
    Serial.println("ADDR sent");
    
    if ((TWSR & MASK_TWSR) != TW_MT_SLA_ACK) return TW_BUS_ERROR;
    Serial.println("OK");
    return I2C_OK;
}

uint8_t _transmission(uint8_t _data) {

	TWDR = _data;
	
	while (!(TWCR & (1 << TWINT)));
	
	if ((TWSR & MASK_TWSR)!= TW_MT_DATA_ACK) return TW_BUS_ERROR;
	
	return I2C_OK;
	
}

void _stop_transmission() {
        //TWCR |=  (1 << TWINT);
	//TWCR &= ~(1 << TWSTA);
	//TWCR |=  (1 << TWSTO);
	TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO);
}

////////////////AUX FUNC FOR RECEPTION///////////////////////////////////////////
uint8_t _reception() {
    uint8_t data_rx;
	
	data_rx = TWDR;
	
	while (!(TWCR & (1 << TWINT)));
	
	if ((TWSR & MASK_TWSR)!= TW_MT_DATA_ACK) TW_BUS_ERROR;
	
	return data_rx;
}

void _stop_reception() {
	
}

///////////////////// I2C init fct //////////////////////////////////////////////
void init(struct i2c_config *_conf) {
    uint8_t psclr_v = 1;
    //clear TWBR -- default value
    TWBR &= 0x00;
    
    //clear TWCR -- default value
    TWCR &= 0x00;
    
    //clear TWSR -- default value
    TWSR &= 0xfc;
    
    //set TWAR to default value
    TWAR |= 0xff;
    
    //enble I2C + interrupts
    TWCR |= (1 << TWEN);
    
    if( _conf->mode == 0x01 ) { 
        //Master transmiter
	//set prescaler
    	if( _conf->prescaler > 0 ) TWSR |= ( _conf->prescaler << TWPS0 );
	
	//set I2C bit rate
	TWBR |= ((OSC_FRQ / SCL_FRQ) - 16) / (2 * psclr_v);
	
	DDRC |= (1 << DDC4); // set pin SDA out
	DDRC |= (1 << DDC5); // set pin SCL out
	
    } else if ( _conf->mode == 0x02 ) {
	//Slave receptor
	//set slave addr
	//TWAR = ( _conf->slave_addr << 1 );
	TWAR |= _conf->slave_addr;
	TWCR |= (1 << TWEA);
	
	DDRC &= ~(1 << DDC4); // set pin SDA in
	DDRC &= ~(1 << DDC5); // set pin SCL in
    }
	
    // Activ internal pull-up
    PORTC |= (1 << PORTC4); // SDA
    PORTC |= (1 << PORTC5); // SCL
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
struct i2c* construct_i2c() {
	struct i2c* i2c_device   = (struct i2c*)malloc(sizeof(struct i2c));
	i2c_device->init                = &init;
	i2c_device->master_write        = &master_TX;
	i2c_device->master_write_nbytes = &master_TX_Nbytes;
	i2c_device->master_read         = &master_RX;
	return i2c_device;
}

///////////////// SCANNER I2C Slave ///////////////////////////////////////
uint8_t scan_i2c_slave() {
	uint8_t addr = 0xff;
	for(uint8_t addr_i = 0x01 ; addr_i < 0x7f; addr_i++){
	    Serial.print(addr_i);
	    Serial.print(" ");
	    uint8_t status = _start_transmission(addr_i, RX_BIT);
	    
	    if(status == I2C_OK) 
	    {Serial.println(status);
	    	addr = addr_i;
	    	_stop_transmission();
	    	break;
	    }
	    _stop_transmission();
	    delay(1000);	    
	}
	return addr;
}

