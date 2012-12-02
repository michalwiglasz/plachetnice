#ifndef _CTRL_REG_H_
#define _CTRL_REG_H_


#define CTRL_REG_ADDR 0x0060

inline uint16_t ctrlreg_get_value() {
	return FPGA_SPI_RW_A8_D16(SPI_FPGA_ENABLE_READ, CTRL_REG_ADDR, 0);
}

inline void ctrlreg_set_value(uint16_t value) {
	FPGA_SPI_RW_A8_D16(SPI_FPGA_ENABLE_WRITE, CTRL_REG_ADDR, value);
}

inline int ctrlreg_get_bit(int bit) {
	return (ctrlreg_get_value() & (1 << bit)) >> bit;
}

inline void ctrlreg_set_bit(int bit) {
	uint16_t val = ctrlreg_get_value() | (1 << bit);
	ctrlreg_set_value(val);
}

inline void ctrlreg_clear_bit(int bit) {
	uint16_t val = ctrlreg_get_value() & ~(1 << bit);
	ctrlreg_set_value(val);
}


#endif /* _CTRL_REG_H_ */