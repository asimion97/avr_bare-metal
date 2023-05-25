#pragma once

struct timer_config{
	uint8_t select;
	uint8_t mode;
	uint8_t prescaler_frq;
	uint8_t cmp_val;
	uint8_t top_val;
	uint8_t bot_val;
};

struct timer{
        struct timer_config conf;	
	void (*init)(struct timer);
};

void init_timer(struct timer _timer);
void drive_timer(void (*timer_stuff)());

struct timer* create(struct timer_config _conf);
