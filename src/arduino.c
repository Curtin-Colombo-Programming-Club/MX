/* 
 * MXEN - Project
 * - Prototye program for 8x8 LED grid
 * 
 * File: arduino.c
 * Author: TodoLodo (Roman)
 * Co-Author(s):
 * 
 * Description: Contains main function along with, runtime configs, ISR(s) and main loop
 */

#include "arduino.h"

volatile uint64_t LED_pattern = 6788975656567;
volatile uint8_t six_segment = 0;

void update_serial()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        USART_transmit((uint8_t)(LED_pattern >> i * 8));
    }
}

void update_seriald(uint64_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        USART_transmit((uint8_t)(data >> i * 8));
    }
}

// Interrupt Service Routine for USART receive complete
ISR(USART0_RX_vect)
{
    static uint64_t received_LED_pattern = 0;

    uint8_t received_data = UDR0;
    USART_transmit(received_data);
    uint8_t begin_end = (received_data >> 6);

    // received_LED_pattern |= (uint64_t)(received_data & (uint8_t)63) << (six_segment * 6);

    // update the LED pattern (LSB -> MSB)
    switch ((int)begin_end)
    {
    case 1: // recieve begin
        six_segment = 0;
        received_LED_pattern = (uint64_t)(received_data & (uint8_t)63);
        break;

    case 0:                                                                                   // recieveing
        received_LED_pattern |= (uint64_t)(received_data & (uint8_t)63) << (six_segment * 6); // updates the segment
        break;

    case 2: // recieve end
        received_LED_pattern |= (uint64_t)(received_data & (uint8_t)63) << (six_segment * 6); // updates the segment
        LED_pattern = received_LED_pattern;
        update_serial();
        break;

    default:
        break;
    }
    six_segment++;
}

int main()
{
    shift_register_init();
    // Initialize USART communication
    USART_init();

    _delay_ms(20);

    // Enable global interrupts
    sei();

    while (1)
    {
        for (uint8_t row = 0; row < 8; row++)
        {
            uint8_t anode_data = (uint8_t)(LED_pattern >> row * 8);
            if ((int)anode_data)
            {
                uint8_t cathode_data = ~(1 << row);
                cathode_bit_shift(cathode_data);
                anode_bit_shift(anode_data);
            }
            /* else
            {
                anode_bit_shift(anode_data);
            } */

            SR_PORT |= 1 << OR_CLK;    // setting output register clock pin to HIGH
            SR_PORT &= ~(1 << OR_CLK); // setting output register clock pin to LOW

            //_delay_ms(1);
        }
    }

    return 1;
}
