/* 
 * MXEN - Project
 * - Prototye program for 8x8 LED grid
 * 
 * File: shift_register.h
 * Author: TodoLodo (Roman)
 * Co-Author(s):
 * 
 * Description: Contains port configurations for shifting bits and function declerations 
 */

#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define DDR(x) (*(&x - 1)) // Address of Input Register of Port X

#define SR_PORT PORTA // to avoid code editer warnings

#ifdef MEGA
#define SR_PORT PORTA
#elif defined(UNO)
#define SR_PORT PORTD
#endif

#define C_DATA_PIN 0
#define C_SR_CLK 1
#define A_DATA_PIN 2
#define A_SR_CLK 3
#define OR_CLK 4

void shift_register_init();
void _bit_shift(uint8_t data, uint8_t DATA_PIN, uint8_t SR_CLK);
void cathode_bit_shift(uint8_t data);
void anode_bit_shift(uint8_t data);

#endif