#pragma once

struct timer{
	uint8_t select;
	uint8_t mode;
	uint8_t prescaler_frq;
	uint8_t cmp_val;
	uint8_t top_val;
	uint8_t bot_val;
};

struct timer create_struct_timer(uint8_t sel, uint8_t mode, uint8_t pres_frq, uint8_t cmp_val, uint8_t top_val, uint8_t bot_val);
void init_timer(struct timer _timer);
void drive_timer(void (*timer_stuff)());
