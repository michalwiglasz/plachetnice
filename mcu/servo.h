#ifndef _SERVO_H_
#define _SERVO_H_


#define SERVO_DEFAULT_MAX 20000
#define SERVO_DEFAULT_WIDTH 1500

#define SERVO_A_BASE_ADDR 0xE0
#define SERVO_A_MAX_ADDR (SERVO_A_BASE_ADDR)
#define SERVO_A_FLIP_ADDR (SERVO_A_MAX_ADDR + 1)

#define servo_a_init() {do {servo_a_set_max_usec(SERVO_DEFAULT_MAX); servo_a_set_width_usec(SERVO_DEFAULT_WIDTH);} while(0);}

#define servo_a_set_max_usec(usec) FPGA_SPI_RW_A8_D16(SPI_FPGA_ENABLE_WRITE, SERVO_A_MAX_ADDR, ((usec >> 3) * 20))
#define servo_a_get_max_usec() ((FPGA_SPI_RW_A8_D16(SPI_FPGA_ENABLE_READ, SERVO_A_MAX_ADDR, 0) / 20) << 3)

#define servo_a_set_width_usec(usec) FPGA_SPI_RW_A8_D16(SPI_FPGA_ENABLE_WRITE, SERVO_A_FLIP_ADDR, ((usec >> 3) * 20))
#define servo_a_get_width_usec() ((FPGA_SPI_RW_A8_D16(SPI_FPGA_ENABLE_READ, SERVO_A_FLIP_ADDR, 0) / 20) << 3)

#endif /* _SERVO_H_ */
