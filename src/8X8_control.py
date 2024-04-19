""" 
MXEN - Project
- Prototype program for 8x8 LED grid

File: 8x8_control.py
Author: TodoLodo (Roman)
Co-Author(s):

Description: Contains program code that enables communication the aurdiono via serial bus and the control GUI
"""

import pygame
import serial
import threading
import numpy as np
import time

BEGIN = 64
END = 128
SENDING = 0


class Main:
    def __init__(self):
        self.LED_pattern = 12297829382473034410
        self.sending_data = False
        
        # test print
        for i in range(11):
            _byte = bin(((self.LED_pattern & (63 << i * 6)) >> 6 * i) + (BEGIN if i == 0 else END if i == 10 else SENDING))[2:].zfill(8)
            print(f"byte: {_byte}, i: {i}")
        print()        
    
        self.ser = serial.Serial('COM17', 9600, timeout=1)
            
        try:
    # Open the serial port
            while not self.ser.is_open:
                self.ser.open()
            
            time.sleep(2)
            
            self.test_send_data()
            
            time.sleep(3)
            
            self.LED_pattern = 0
            
            self.test_send_data()

            # Initialize Pygame
            pygame.init()

            # Set the dimensions of each cell and the number of rows and columns
            CELL_SIZE = 50
            ROWS = 8
            COLS = 8

            # Set the size of the window
            WINDOW_SIZE = (CELL_SIZE * COLS, CELL_SIZE * ROWS)
            screen = pygame.display.set_mode(WINDOW_SIZE)
            pygame.display.set_caption("8x8 Grid")

            # Define colors
            WHITE = (255, 255, 255)
            BLACK = (0, 0, 0)
            BLUE = (0, 0, 255)

            # Initialize cell states (all cells start as white)
            cell_states = [[False for _ in range(COLS)] for _ in range(ROWS)]

            # Main loop
            running = True
            while running:
                for event in pygame.event.get():
                    if event.type == pygame.QUIT:
                        running = False
                    elif event.type == pygame.MOUSEBUTTONDOWN:
                        # Get the position of the mouse click
                        mouse_x, mouse_y = pygame.mouse.get_pos()
                        # Calculate the row and column of the clicked cell
                        row = mouse_y // CELL_SIZE
                        col = mouse_x // CELL_SIZE
                        # Toggle the state of the clicked cell
                        ct = cell_states[row][col] = not cell_states[row][col]
                        
                        # updating LED pattern
                        shift_count = row*8 + col
                        if ct:
                            self.LED_pattern |= (1<<shift_count)
                        else:
                            self.LED_pattern &= ~(1<<shift_count)
                            
                        # pattern print
                        _raw_pattern = bin(self.LED_pattern)[2:].zfill(64)[::-1]
                        _pattern = ''.join([_raw_pattern[i:i+8] + '\n' for i in range(0, len(_raw_pattern), 8)])
                        print(_pattern)
                        print()
                        
                        self.test_send_data()
                        
                        

                # Clear the screen
                screen.fill(WHITE)

                # Draw the grid
                for row in range(ROWS):
                    for col in range(COLS):
                        # Calculate the position of each cell
                        x = col * CELL_SIZE
                        y = row * CELL_SIZE

                        # Determine the color of the cell based on its state
                        color = BLUE if cell_states[row][col] else WHITE

                        # Draw the cell
                        pygame.draw.rect(screen, color, (x, y, CELL_SIZE, CELL_SIZE))
                        pygame.draw.rect(screen, BLACK, (x, y, CELL_SIZE, CELL_SIZE), 1)

                # Update the display
                pygame.display.flip()
                

        except KeyboardInterrupt:
            print("Exiting...")
            pygame.quit()
            self.ser.close()
            
    def send_pattern(self):
        data = self.LED_pattern
        r_b = 0
        print(f"int: {data}\n")
        for i in range(11):
            print(i)
            _byte = ((data & (63 << i * 6)) >> 6 * i) + (BEGIN if i == 0 else END if i == 10 else SENDING)
            _byte_bin = bin(_byte)[2:].zfill(8)
            print(f"Sending: {_byte}")
            print(f"byte to send: {_byte_bin}")
            self.ser.write(_byte.to_bytes(1, byteorder='big'))
            
            
            res = self.ser.read()
            res_int = int.from_bytes(res, byteorder="big")
            res_byte_bin = bin(res_int)[2:].zfill(8)
            print("Received:", res_int)
            print(f"recieved byte: {res_byte_bin}")
            r_b += res_int << i * 6
            
            print()
            
        print(f"recieved total: {r_b}")
        
        print()
        
    def test_send_data(self):
        self.send_pattern()
        for i in range(8):
            res = self.ser.read()
            res_int = int.from_bytes(res, byteorder="big", signed=False)
            _byte_bin = bin(res_int)[2:].zfill(8)
            print("Received:", _byte_bin, "val:", res_int)
        print() 
        

if __name__ == "__main__":
    Main()