/*
 * print_func.h
 *
 * Created: 1/3/2012 4:39:36 PM
 *  Author: mpoppe
 */ 


#ifndef PRINT_FUNC_H_
#define PRINT_FUNC_H_


void print_char_hex(uint8_t num, uint8_t *buffer);

typedef struct {
	uint8_t cnt;
	uint8_t *strings[];
}itemizedString_t;

#endif /* PRINT_FUNC_H_ */