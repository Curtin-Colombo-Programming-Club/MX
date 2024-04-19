/* 
 * MXEN - Project
 * - Prototye program for 8x8 LED grid
 * 
 * File: serial.c
 * Author: TodoLodo (Roman)
 * Co-Author(s):
 * 
 * Description: Contains function required for serial communications
 */

#include "serial.h"

// Function to initialize USART communication with 1 start bit, 1 stop bit, and no parity
void USART_init()
{
    // Set baud rate
    UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
    UBRR0L = (uint8_t)(BAUD_PRESCALLER);

    // Enable receiver and transmitter, enable RX complete interrupt
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

    // Set frame format: 8 data bits, 1 stop bit, no parity
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Function to transmit a byte via USART
void USART_transmit(uint8_t data)
{
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    // Put data into buffer, sends the data
    UDR0 = data;
}
