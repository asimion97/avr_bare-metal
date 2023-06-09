#include "i2c.h"

/////////////////AUX MASTER FUNCT FOR TRANSMISSION //////////////////////////////////// 
uint8_t _start_transmission(uint8_t _dest_addr, uint8_t direction) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);//init start transmission

    while (!(TWCR & (1 << TWINT)));
    
    if ((TWSR & MASK_TWSR) != TW_START) return TW_BUS_ERROR;
     
    TWDR = (( _dest_addr << 1 ) | direction);
    
    TWCR = (1 << TWINT) | (1 << TWEN);
    
    while (!(TWCR & (1 << TWINT)));
    
    if( !direction ) {
       if ((TWSR & MASK_TWSR) != TW_MT_SLA_ACK) return TW_BUS_ERROR;
     } else { 
       if ((TWSR & MASK_TWSR) != TW_MR_SLA_ACK) return TW_BUS_ERROR;  
     }
       
    return I2C_TOK;
}

uint8_t _transmission(uint8_t _data) {
	TWDR = _data;
	
	TWCR = (1 << TWINT) | (1 << TWEN);
	
	while (!(TWCR & (1 << TWINT)));
	
	if ((TWSR & MASK_TWSR) != TW_MT_DATA_ACK) return TW_BUS_ERROR;
	
	return I2C_DOK;
}

void _stop_transmission() {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

////////////////AUX MASTER FUNC FOR RECEPTION (1byte )///////////////////////////////////////////
uint8_t _reception(uint8_t *data_rx) {
    //uint8_t data_rx;
	DDRC &= ~(1 << DDC4); // set pin SDA in
    
	TWCR = (1 << TWINT) | (1 << TWEN);
    
    while (!(TWCR & (1 << TWINT)));
	
	(*data_rx) = TWDR; // read data after TWSR is Checked

    if ((TWSR & MASK_TWSR) == TW_MR_DATA_NACK) { 
       DDRC |= (1 << DDC4); // set pin SDA out
       return I2C_TSTO;
    }
    
    return TW_BUS_ERROR;
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
    
    //enble I2C
    TWCR = (1 << TWEN);
    
    //clear data reg
    TWDR &= 0x00; 
    
    //compute prescaler value
    switch(_conf->prescaler){
      case 1: psclr_v = 4; break;
      case 2: psclr_v = 16; break;
      case 3: psclr_v = 64; break;
    }    
    
    if( _conf->mode == MT_MODE || _conf->mode == MR_MODE ) { 
        //Master transmiter + Master receptor
	    //set prescaler
    	if( _conf->prescaler > 0 ) { 
    	  TWSR |= ( _conf->prescaler << TWPS0 ); 
	}
	
	    //set I2C bit rate
	    TWBR |= ((OSC_FRQ / SCL_FRQ) - 16) / (2 * psclr_v);
	
	    DDRC |= (1 << DDC4); // set pin SDA out
	    DDRC |= (1 << DDC5); // set pin SCL out
	
    } else if ( _conf->mode == SR_MODE || _conf->mode == ST_MODE ) {
         Serial.println("Slave mode active");
	    //Slave receptor
	    //set slave addr
	    TWAR = (_conf->slave_addr << 1); 
	    TWCR |= (1 << TWEN) | (1 << TWEA);
	
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
    uint8_t data;
    uint8_t e = _start_transmission(slave_addr, RX_BIT);
    
    if(e != I2C_TOK) return TW_BUS_ERROR;
    
    e = _reception(&data);
    
    _stop_transmission();
     
    if(e != I2C_TSTO) return TW_BUS_ERROR; 
    
    return data;
}

//////////// I2C slave reception func ////////////////////////////////////////
uint8_t slave_RX() {
    uint8_t data = 0x00;
    //flush
    //TWDR &= 0x00;
    
    switch(TWSR & MASK_TWSR) {
                               
       case TW_SR_SLA_ACK    : {
                                 //data = TWDR;
                                  //TWCR |= (1 << TWINT) | (1 << TWEA);
                                  //break; //send ACK;
                               }
                               
       case TW_SR_GCALL_ACK  : {
                                 data = TWDR;                                 
                                 TWCR |= (1 << TWINT) | (1 << TWEA); //send ACK
                                 break;
                               }
                             
       case TW_SR_ARB_LOST_SLA_ACK:
       case TW_SR_ARB_LOST_GCALL_ACK: {
                                        TWCR |= (1 << TWINT) | (1 << TWEA);
                                        break;
                                      }
       case TW_SR_STOP: {
                          TWCR |= (1 << TWINT) | (1 << TWEA);
                          break;
                        }
       default: {
                  TWCR |= (1 << TWINT) | (1 << TWEA);
                  break;
                }
    }
    Serial.println(TWSR, HEX);
    Serial.print("TWDR: ");
    Serial.println(data, HEX);
    return data;
}

//////////// I2C slave transmission func ////////////////////////////////////////
void slave_TX(uint8_t data) {
     
    while (!(TWCR & (1 << TWINT))); // wait for complete
    
    switch(TWSR & MASK_TWSR) {
    
       case TW_ST_SLA_ACK  : {
                               TWDR = data; //Data
                               TWCR = (1 << TWINT) | (1 << TWEA); //send ACK
                               break;
                             }
                             
       case TW_ST_DATA_NACK: {
                               TWCR &= ~(1 << TWEN);
                               break;
                             }
                             
       case TW_ST_LAST_DATA: {
                               TWCR = (1 << TWINT); //send NACK
                               break;
                             }
                             
       case TW_ST_ARB_LOST_SLA_ACK: {
                                      TWCR |= (1 << TWSTO) | (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
                                      break;
                                    }
    }
     
}

void slave_TX_Nbytes(uint8_t* data, uint8_t nbytes) {
    
    for(int i = 0; i < nbytes; i++) {
    
       while (!(TWCR & (1 << TWINT))); // wait for complete
    
       switch(TWSR & MASK_TWSR) {
    
          case TW_ST_SLA_ACK  : {
                                  TWDR = data; //Data
                                  TWCR = (1 << TWINT) | (1 << TWEA); //send ACK
                                  break;
                                }
                             
          case TW_ST_DATA_ACK: {
                                  TWDR = data;
                                  TWCR = (1 << TWINT) | (1 << TWEA); //send ACK
                                  break;
                               }                      
                             
          case TW_ST_DATA_NACK: {
                                  TWCR &= ~(1 << TWEN); //disable I2C
                                  break;
                                }
                             
          case TW_ST_LAST_DATA: {
                                  TWCR = (1 << TWINT); //send NACK
                                  break;
                                }
                             
          case TW_ST_ARB_LOST_SLA_ACK: {
                                         TWCR |= (1 << TWSTO) | (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
                                         break;
                                       }
       }
    }
}

///////////////// CONSTRUCT I2C Structure ///////////////////////////////////////
struct i2c* construct_i2c() {
	struct i2c* i2c_device   = (struct i2c*)malloc(sizeof(struct i2c));
	i2c_device->init                = &init;
	i2c_device->master_write        = &master_TX;
	i2c_device->master_write_nbytes = &master_TX_Nbytes;
	i2c_device->master_read         = &master_RX;
	i2c_device->slave_write         = &slave_TX;
	i2c_device->slave_write_nbytes  = &slave_TX_Nbytes;
	i2c_device->slave_read          = &slave_RX;
	return i2c_device;
}

///////////////// SCANNER I2C Slave ///////////////////////////////////////
uint8_t get_first_addr_i2c_slave() {
	uint8_t addr = 0x00;
	
	for(uint8_t addr_i = 0x01 ; addr_i < 0x7f; addr_i++){
	    uint8_t status = _start_transmission(addr_i, TX_BIT);
	    uint16_t delay = 3000;
	    if(status == I2C_TOK) 
	    {
	    	addr = addr_i;
	    	_stop_transmission();
	    	break;
	    }
	    
	    _stop_transmission();
	    while(delay--);
	}
	return addr;
}

uint8_t* get_all_addr_i2c_slave() {
	uint8_t* addr = (uint8_t*)malloc(sizeof(uint8_t) * 0x7f);
	uint8_t cnt = 0;
	
	for(uint8_t addr_i = 0x01 ; addr_i < 0x7f; addr_i++){
	    uint8_t status = _start_transmission(addr_i, TX_BIT);
	    uint16_t delay = 3000;
	    if(status == I2C_TOK) {
	    	addr[cnt++] = addr_i;
	    } else { 
	        addr[cnt++] = 0x00;
	    }
	    
	    _stop_transmission();
	    while(delay--);
	}
	return addr;
}

