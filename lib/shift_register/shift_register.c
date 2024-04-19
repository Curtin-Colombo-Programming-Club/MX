/* 
 * MXEN - Project
 * - Prototye program for 8x8 LED grid
 * 
 * File: shift_register.c
 * Author: TodoLodo (Roman)
 * Co-Author(s):
 * 
 * Description: Contains functions required to shift bits into shift register
 */

#include "shift_register.h"

void shift_register_init()
{
    DDR(SR_PORT) |= (1 << C_DATA_PIN) | (1 << C_SR_CLK) | (1 << A_DATA_PIN) | (1 << A_SR_CLK) | (1 << OR_CLK);
}

void _bit_shift(uint8_t data, uint8_t DATA_PIN, uint8_t SR_CLK)
{

    for (uint8_t i = 0; i < 8; i++)
    {
        if (data & (1 << (7 - i)))
        {
            SR_PORT |= (1 << DATA_PIN);
        }
        else
        {
            SR_PORT &= ~(1 << DATA_PIN);
        }
        // SR_PORT |= ((data & (1 << i)) >> i) << DATA_PIN; // updating data pin
        SR_PORT |= 1 << SR_CLK;    // setting shift register clock pin to HIGH (bit is shifted in to the register)
        SR_PORT &= ~(1 << SR_CLK); // setting shift register clock pin to LOW
    }
}

void cathode_bit_shift(uint8_t data)
{
    _bit_shift(data, C_DATA_PIN, C_SR_CLK);
}

void anode_bit_shift(uint8_t data)
{
    _bit_shift(data, A_DATA_PIN, A_SR_CLK);
}