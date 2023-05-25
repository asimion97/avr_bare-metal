#include "timer.h"
#include<avr/io.h>
#include<avr/interrupt.h>

void init_timer(struct timer _timer){
       	
       	TOP    = (_timer.top_val > 0)? _timer.top_val : 0xFF;
       	BOTTOM = (_timer.bot_val < 0 && _timer.bot_val >= _timer.top_val )? 0x00 : _timer.bot_val;
       	      
        if(_timer.sel == 0){
                //first timer/counter 0
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
       		
        	TIMSK0 |= (1 << OCIE0A); // Activ point comp OCR0A
                sie();                   // activate global interrupt
        }else if(_timer.sel == 1){
                //16b timer/counter
        
        }else if(_timer.sel == 2){
                //second 8b timer/counter 2
	       	TCCR2A = 0; //clear 
		TCCR2B = 0; //clear
		TIMSK2 = 0; //clear
		TIFR2  = 0; //clear

		// select prescaler
		if(_timer.prescaler_frq == 5)       TCCR2B |= (1 << CS22) | (1 << CS20);
		else if(_timer.prescaler_frq == 4)  TCCR2B |= (1 << CS22);
		else if(_timer.prescaler_frq == 3)  TCCR2B |= (1 << CS21) | (1 << CS20);
		else if(_timer.prescaler_frq == 2)  TCCR2B |= (1 << CS21);
		else if(_timer.prescaler_frq == 1)  TCCR2B |= (1 << CS20);
	       		
       		switch(_timer.mode){
       			case 0: //Normal mode
       				TCCR2A = 0x00; //set Normal Mode
       					
				OCR2A  = TOP; //cmp val is TOP 
				break;
			case 1: //PWM Phase Correct classic mode
                                // PB1 (OC2A): output PWM for Timer2, ch A.
                                // PB2 (OC2B): output PWM for Timer2, ch B.
			        TCCR2A |= (1 << WGM20); // set pwm phase correct mode
			        
			        // Clear OC0A on compare match when up-counting. 
			        // Set OC0A on compare match when down-countin
			        TCCR2A |= (1 << COM2A1);
			        
			        // Set  OC0A (PD6) as output
                                DDRD |= (1 << PB1);
                                
                                OCR2A = _timer.cmp_val;//set cmp value
			        break;
			case 2: //Clear Timer Control Mode
			        TCCR2A |= (1 << WGM21); //set CTC Mode  
			        
			        OCR2A = _timer.cmp_val; //set cmp val
			        break;
			case 3: //Fast PWM mode
			        break;
       		}
       		
        	TIMSK2 |= (1 << OCIE2A); // Activ point comp OCR2A
                sie();                   // activate global interrupt                
        }
	
}

void drive_timer(void (*timer_stuff)()){
	timer_stuff();
}

struct timer* create(struct timer_config _conf){
     struct timer timer;
     timer.conf = _conf;
     timer.init = &init_timer;
     
     return &timer;
} 

ISR(TIMER0_COMPA_vect) {
	drive_timer(void (*timer_stuff)());
}


