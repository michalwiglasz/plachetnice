#ifndef _SERVO_H_
#define _SERVO_H_


#define SERVO_HS422_PERIOD 20000
#define SERVO_HS422_LEFT 500
#define SERVO_HS422_CENTER 1470
#define SERVO_HS422_RIGHT 2500


typedef struct {
	uint8_t addr;
	uint16_t period;
	uint16_t left;
	uint16_t right;
	uint16_t center;
} servo_t;

void servo_set_period(servo_t *servo, uint16_t max) {
	FPGA_SPI_RW_A8_D16(SPI_FPGA_ENABLE_WRITE, servo->addr, ((max >> 3) * 20));
}

uint16_t servo_get_period(servo_t *servo) {
	return (FPGA_SPI_RW_A8_D16(SPI_FPGA_ENABLE_READ, servo->addr, 0) / 20) << 3;
}

void servo_set_width(servo_t *servo, uint16_t value) {
	FPGA_SPI_RW_A8_D16(SPI_FPGA_ENABLE_WRITE, servo->addr + 1, ((value >> 3) * 20));
}

uint16_t servo_get_width(servo_t *servo) {
	return (FPGA_SPI_RW_A8_D16(SPI_FPGA_ENABLE_READ, servo->addr + 1, 0) / 20) << 3;
}

void servo_go_left(servo_t *servo) {
	servo_set_width(servo, servo->left);
}

void servo_go_center(servo_t *servo) {
	servo_set_width(servo, servo->center);
}

void servo_go_right(servo_t *servo) {
	servo_set_width(servo, servo->right);
}

void servo_init(servo_t *servo) {
	servo_set_period(servo, servo->period);
	servo_go_center(servo);
}

#endif /* _SERVO_H_ */
