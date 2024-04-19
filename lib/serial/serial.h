/* 
 * MXEN - Project
 * - Prototye program for 8x8 LED grid
 * 
 * File: serial.h
 * Author: TodoLodo (Roman)
 * Co-Author(s):
 * 
 * Description: Contains baude rate configs and serial function declrations
 */

#ifndef SERIAL_H
#define SERIAL_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

// Define baud rate
#define BAUD_RATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

void USART_init();
void USART_transmit(uint8_t data);

#endif