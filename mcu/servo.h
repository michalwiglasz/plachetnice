#ifndef _SERVO_H_
#define _SERVO_H_


#define SERVO_HS422_PERIOD 50000  /* 20000 us: 400000 (19 bits) ~ 50000<<3 (16 bits) */
#define SERVO_HS422_LEFT 10000  /* 500 us */
#define SERVO_HS422_CENTER 30000  /*  1500 us */
#define SERVO_HS422_RIGHT 50000  /* 2500 us */


typedef struct {
	char* name;
	uint8_t addr;
	uint16_t period;
	uint16_t left;
	uint16_t right;
	uint16_t center;
	int16_t left_angle;
	int16_t right_angle;
	int16_t center_angle;
	int ctrlreg_bit;
} servo_t;

inline void servo_set_period(servo_t *servo, uint16_t max) {
	FPGA_SPI_RW_A8_D16(SPI_FPGA_ENABLE_WRITE, servo->addr, max);
}

inline uint16_t servo_get_period(servo_t *servo) {
	return FPGA_SPI_RW_A8_D16(SPI_FPGA_ENABLE_READ, servo->addr, 0);
}

inline void servo_set_width(servo_t *servo, uint16_t value) {
	FPGA_SPI_RW_A8_D16(SPI_FPGA_ENABLE_WRITE, servo->addr + 1, value);
}

inline uint16_t servo_get_width(servo_t *servo) {
	return FPGA_SPI_RW_A8_D16(SPI_FPGA_ENABLE_READ, servo->addr + 1, 0);
}

inline uint16_t servo_angle_to_width(servo_t *servo, int16_t angle) {
	if (angle < servo->center_angle) {
		return servo->center + ((servo->center - servo->left) * angle) / (float)(servo->center_angle - servo->left_angle);

	} else if (angle > servo->center_angle) {
		return servo->center + ((servo->right - servo->center) * angle) / (float)(servo->right_angle - servo->center_angle);
	
	} else {
		return servo->center;
	}
}

inline int16_t servo_width_to_angle(servo_t *servo, uint16_t width) {
	if (width < servo->center) {
		return servo->center_angle + ((servo->center_angle - servo->left_angle) * width) / (float)(servo->center - servo->left);

	} else if (width > servo->center) {
		return servo->center_angle + ((servo->right_angle - servo->center_angle) * width) / (float)(servo->right - servo->center);

	} else {
		return servo->center_angle;
	}
}

inline void servo_go_left(servo_t *servo) {
	servo_set_width(servo, servo->left);
}

inline void servo_go_center(servo_t *servo) {
	servo_set_width(servo, servo->center);
}

inline void servo_go_right(servo_t *servo) {
	servo_set_width(servo, servo->right);
}

inline void servo_set_angle(servo_t *servo, float angle) {
	servo_set_width(servo, servo_angle_to_width(servo, angle));
}

inline float servo_get_angle(servo_t *servo) {
	return servo_width_to_angle(servo, servo_get_width(servo));
}

inline void servo_init(servo_t *servo) {
	servo_set_period(servo, servo->period);
	servo_go_center(servo);
}

#endif /* _SERVO_H_ */
