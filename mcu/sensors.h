#ifndef _SENSORS_H_
#define _SENSORS_H_

#include <fitkitlib.h>

#define set_mag_clk(on) {\
  P4DIR |= BIT3; /* nastavit bit jako vystup */ \
  P4OUT = (on) ? (P4OUT | BIT3) : (P4OUT & ~BIT3); /* 1 - sviti, 0 - nesviti */ \
 }

 #define set_mag_cs(on) {\
  P4DIR |= BIT2; /* nastavit bit jako vystup */ \
  P4OUT = (on) ?  (P4OUT | BIT2) : (P4OUT & ~BIT2); /* 1 - sviti, 0 - nesviti */ \
 }
 
 /*
inline void sensors_init() {
	P4DIR &= 0x0F;
	P4OUT = 0xFF;
	set_led_3(1);
}*/



inline int sensors_get_bit(int bit) {
	return (P4IN & (1 << bit)) >> bit;
}

inline void sensors_set_bit(int bit) {
	P4OUT |= (1 << bit);
}

inline void sensors_clear_bit(int bit) {
	P4OUT &= ~(1 << bit);
}

inline void sensors_put_bit(int bit, int value) {
	if (value)
		sensors_set_bit(bit);
	else 
		sensors_clear_bit(bit);
}

inline uint8_t sensors_get_optical() {
	return sensors_get_bit(5);
}


#endif /* _SENSORS_H_ */
