#include "timer.h"
#include<avr/io.h>
#include<avr/interrupt.h>

struct timer init_struct_timerstruct timer create_struct_timer(uint8_t sel, uint8_t mode, uint8_t pres_frq, uint8_t cmp_val, uint8_t top_val, uint8_t bot_val){
	struct timer my_timer;
	my_timer.select = sel;
	my_timer.mode = mode;
	my_timer.prescaler_frq = pres_frq;
	my_timer.cmp_val = cmp_val;
	my_timer.top_val = top_val;
	my_timer.bot_val = bot_val;

	return my_timer;
}

void init_timer(struct timer _timer){
       	
       	TOP    = (_timer.top_val > 0)? _timer.top_val : 0xFF;
       	BOTTOM = (_timer.bot_val < 0 && _timer.bot_val >= _timer.top_val )? 0x00 : _timer.bot_val;
       	
       	TCCR0A = 0; //clear 
	TCCR0B = 0; //clear
	TIMSKO = 0; //clear
	TIFR0  = 0; //clear

        // select prescaler
        if(_timer.prescaler_frq == 5)       TCCR0B |= (1 << CS02) | (1 << CS00);
	else if(_timer.prescaler_frq == 4)  TCCR0B |= (1 << CS02);
	else if(_timer.prescaler_frq == 3)  TCCR0B |= (1 << CS01) | (1 << CS00);
	else if(_timer.prescaler_frq == 2)  TCCR0B |= (1 << CS01);
	else if(_timer.prescaler_frq == 1)  TCCR0B |= (1 << CS00);

	      
        if(_timer.sel == 0){
                //first timer/counter 0
       		switch(_timer.mode){
       			case 0: //Normal mode
       				TCCR0A = 0x00; //set Normal Mode
       					
				OCR0A  = TOP; //cmp val is TOP 
				break;
			case 1: //PWM Phase Correct classic mode
			        // PD6 (OC0A): output PWM for Timer0, ch A.
			        // PD5 (OC0B): output PWM for Timer0, ch B.
                                // PB1 (OC1A): output PWM for Timer1, ch A.
                                // PB2 (OC1B): output PWM for Timer1, ch B.
			        TCCR0A |= (1 << WGM00); // set pwm phase correct mode
			        
			        // Clear OC0A on compare match when up-counting. 
			        // Set OC0A on compare match when down-countin
			        TCCR0A |= (1 << COM0A1);
			        
			        // Set  OC0A (PD6) as output
                                DDRD |= (1 << PD6);
                                
                                OCR0A = _timer.cmp_val;//set cmp value
			        break;
			case 2: //Clear Timer Control Mode
			        TCCR0A |= (1 << WGM01); //set CTC Mode  
			        
			        OCR0A = _timer.cmp_val; //set cmp val
			        break;
			case 3: //Fast PWM mode
			        break;
       		}
       		
        	TIMSK0 |= (1 << OCIE0A);// Activ point comp OCR0A
                sie(); // activate global interrupt
        }else if(_timer.sel == 1){
            //16b timer/counter
        
        }else if(_timer.sel == 2){
          //second 8b timer/counter
        }
	
}

void drive_timer(void (*timer_stuff)()){
	timer_stuff();
}

ISR(TIMER0_COMPA_vect) {
	drive_timer(void (*timer_stuff)());
}



