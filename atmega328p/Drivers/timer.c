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
       	TCCR0A = 0; //clear 
	TCCR0B = 0; //clear
	TIMSKO = 0; //clear
	TIFR0  = 0; //clear        
        if(_timer.sel == 0){
                //first timer/counter 0
       		switch(_timer.mode){
       			case 0://Normal mode
       				TCCR0A = 0; //set Normal Mode	
       				
	                        if(_timer.prescaler_frq == 5)       TCCR0B |= (1 << CS02) | (1 << CS00);
				else if(_timer.prescaler_frq == 4)  TCCR0B |= (1 << CS02);
				else if(_timer.prescaler_frq == 3)  TCCR0B |= (1 << CS01) | (1 << CS00);
				else if(_timer.prescaler_frq == 2)  TCCR0B |= (1 << CS01);
				else if(_timer.prescaler_frq == 1)  TCCR0B |= (1 << CS00);
				
				TOP = 0xFF;
				OCR0A = 0xFF;
				
				TIMSKO |= (1 << OCIE0A);
				sei(); // activate global interrupt 
				break;
			case 1://PWM Phase Correct classic mode
			       
			       break;
			case 2://CTC
			       TCCR0A |= (1 << WGM01); //set CTC Mode
	                       
	                       if(_timer.prescaler_frq == 5)       TCCR0B |= (1 << CS02) | (1 << CS00);
			       else if(_timer.prescaler_frq == 4)  TCCR0B |= (1 << CS02);
			       else if(_timer.prescaler_frq == 3)  TCCR0B |= (1 << CS01) | (1 << CS00);
			       else if(_timer.prescaler_frq == 2)  TCCR0B |= (1 << CS01);
			       else if(_timer.prescaler_frq == 1)  TCCR0B |= (1 << CS00);
			       
			       OCR0A = _timer.cmp_val;
			       
			       TIMSKO |= (1 << OCIE0A);
			       sie(); //activate global interrupt
			       break;
			case 3://Phase Correct PWM mode
			      break;
       		}
        	
        	
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



