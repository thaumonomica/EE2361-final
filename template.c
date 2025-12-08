/*
 * Pac-Man Game for 16x32 LED Matrix
 * Button on RB2 rotates direction clockwise
 * Uses your existing blast_row_pic24 and updateMatrix functions
 */

#include "xc.h"
#include <math.h>
#include <stdio.h>
#include "meher_final_asmLib_v001.h"
#include <stdlib.h>

// Configuration bits
#pragma config ICS = PGx1
#pragma config FWDTEN = OFF
#pragma config GWRP = OFF
#pragma config GCP = OFF
#pragma config JTAGEN = OFF
#pragma config I2C1SEL = PRI
#pragma config IOL1WAY = OFF
#pragma config OSCIOFNC = ON
#pragma config FCKSM = CSECME
#pragma config FNOSC = FRCPLL

// Pin definitions
#define R1_LAT   _LATB6
#define G1_LAT   _LATB7
#define B1_LAT   _LATB8
#define CLK_LAT  _LATB5
#define LAT_LAT  _LATB3
#define OE_LAT   _LATB4
#define BUTTON   _RB2

#define PIN_HIGH 1
#define PIN_LOW  0

// Colors
#define red 0x01
#define green 0x02
#define blue 0x04
#define purple 0x05
#define yellow 0x03
#define cyan 0x06
#define black 0x00
#define white 0x07

// Game state
int pacman_x = 5;
int pacman_y = 5;
int direction = 0; // 0=right, 1=down, 2=left, 3=up
int lastButtonState = 1;
int moveCounter = 0;

// Initialize rows with walls (red) and open space (black)
int row1[32] = {red,red,red,red,red,red,red,red,red,red,red,red,red,red,black,black,black,black,red,red,red,red,red,red,red,red,red,red,red,red,red,red};
int row2[32] = {red,black,black,black,black,black,black,black,black,black,black,black,black,red,black,black,black,black,red,black,black,black,black,black,black,black,black,black,black,black,black,red};
int row3[32] = {red,black,black,black,black,black,black,black,black,black,black,black,black,red,black,black,black,black,red,black,black,black,black,black,black,black,black,black,black,black,black,red};
int row4[32] = {red,black,black,black,black,black,black,black,black,black,black,black,black,red,black,black,black,black,red,black,black,black,black,black,black,black,black,black,black,black,black,red};
int row5[32] = {red,black,black,black,black,black,black,black,black,black,black,black,black,red,black,black,black,black,red,black,black,black,black,black,black,black,black,black,black,black,black,red};
int row6[32] = {red,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,red};
int row7[32] = {red,black,black,black,black,black,black,black,black,black,black,black,black,red,black,black,black,black,red,black,black,black,black,black,black,black,black,black,black,black,black,red};
int row8[32] = {red,black,black,black,black,black,black,black,black,black,black,black,black,red,black,black,black,black,red,black,black,black,black,black,black,black,black,black,black,black,black,red};
int row9[32] = {red,black,black,black,black,black,black,black,black,black,black,black,black,red,black,black,black,black,red,black,black,black,black,black,black,black,black,black,black,black,black,red};
int row10[32] = {red,black,black,black,black,black,black,black,black,black,black,black,black,red,black,black,black,black,red,black,black,black,black,black,black,black,black,black,black,black,black,red};
int row11[32] = {red,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,red};
int row12[32] = {red,black,black,black,black,black,black,black,black,black,black,black,black,red,black,black,black,black,red,black,black,black,black,black,black,black,black,black,black,black,black,red};
int row13[32] = {red,black,black,black,black,black,black,black,black,black,black,black,black,red,black,black,black,black,red,black,black,black,black,black,black,black,black,black,black,black,black,red};
int row14[32] = {red,black,black,black,black,black,black,black,black,black,black,black,black,red,black,black,black,black,red,black,black,black,black,black,black,black,black,black,black,black,black,red};
int row15[32] = {red,black,black,black,black,black,black,black,black,black,black,black,black,red,black,black,black,black,red,black,black,black,black,black,black,black,black,black,black,black,black,red};
int row16[32] = {red,red,red,red,red,red,red,red,red,red,red,red,red,red,black,black,black,black,red,red,red,red,red,red,red,red,red,red,red,red,red,red};

int *matrix[16] = {row1,row2,row3,row4,row5,row6,row7,row8,row9,row10,row11,row12,row13,row14,row15,row16};

void setup(void) {
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0xffff;
    TRISB = 0b0000000000000111; // RB0, RB1, RB2 inputs
    TRISBbits.TRISB2 = 1; // Button input
    TRISBbits.TRISB5 = 0;
    
    LATBbits.LATB5 = 0;
    LATBbits.LATB4 = 0;
    LATBbits.LATB3 = 0;
    LATBbits.LATB6 = 0;
    LATBbits.LATB7 = 0;
    LATBbits.LATB8 = 0;
    LATBbits.LATB9 = 0;
    LATBbits.LATB10 = 0;
    LATBbits.LATB11 = 0;
    LATBbits.LATB12 = 0;
    LATBbits.LATB13 = 0;
    
    delay(20);
}

void delay(long n) {
    for (; n > 0; n--) {
        asm("nop");
    }
}

void setcolor(int r, int g, int b) {
    LATBbits.LATB6 = 0;
    LATBbits.LATB7 = 0;
    LATBbits.LATB8 = 0;
    if (r == 1) {
        LATBbits.LATB6 = 1;
    }
    if (g == 2) {
        LATBbits.LATB7 = 1;
    }
    if (b == 4) {
        LATBbits.LATB8 = 1;
    }
}

void blast_row_pic24(uint8_t *data, uint16_t length) {
    delay(10000);
    LATBbits.LATB4 = 1;
    delay(1000);
    
    int *ptr = data;
    do {
        uint8_t pixel = *ptr++;
        setcolor(pixel & 0x01, pixel & 0x02, pixel & 0x04);
        CLK_LAT = PIN_HIGH;
        CLK_LAT = PIN_LOW;
    } while(length--);
    
    LATBbits.LATB4 = 0;
}

void reset() {
    LATBbits.LATB6 = 0;
    LATBbits.LATB7 = 0;
    LATBbits.LATB8 = 0;
    LATBbits.LATB9 = 0;
    LATBbits.LATB10 = 0;
    LATBbits.LATB11 = 0;
    LATBbits.LATB12 = 0;
}

void updateMatrix(int startrow, int FOV) {
    for (int i = startrow; i < startrow + FOV; i++) {
        blast_row_pic24(matrix[i], 31);
        
        int state9 = (i & 0x01) == 1;
        int state10 = (i & 0x02) == 2;
        int state11 = (i & 0x04) == 4;
        int state12 = (i & 0x08) == 8;
        LATBbits.LATB4 = 1;
        
        LAT_LAT = PIN_HIGH;
        delay(1);
        LAT_LAT = PIN_LOW;
        LATBbits.LATB9 = state9;
        LATBbits.LATB10 = state10;
        LATBbits.LATB11 = state11;
        LATBbits.LATB12 = state12;
        LATBbits.LATB4 = 0;
    }
}

void checkButton() {
    int currentButtonState = BUTTON;
    
    // Button pressed (active low)
    if (currentButtonState == 0 && lastButtonState == 1) {
        direction = (direction + 1) % 4; // Rotate clockwise
        delay(50000); // Debounce
    }
    
    lastButtonState = currentButtonState;
}

int checkWall(int x, int y) {
    if (y < 0 || y >= 16 || x < 0 || x >= 32) return 1;
    return (matrix[y][x] == red);
}

void drawPacman() {
    // Draw pacman as yellow pixel at current position
    if (pacman_y >= 0 && pacman_y < 16 && pacman_x >= 0 && pacman_x < 32) {
        matrix[pacman_y][pacman_x] = yellow;
    }
}

void clearPacman() {
    // Clear pacman's old position
    if (pacman_y >= 0 && pacman_y < 16 && pacman_x >= 0 && pacman_x < 32) {
        if (matrix[pacman_y][pacman_x] == yellow) {
            matrix[pacman_y][pacman_x] = black;
        }
    }
}

void movePacman() {
    int new_x = pacman_x;
    int new_y = pacman_y;
    
    // Calculate new position
    switch(direction) {
        case 0: // Right
            new_x++;
            break;
        case 1: // Down
            new_y++;
            break;
        case 2: // Left
            new_x--;
            break;
        case 3: // Up
            new_y--;
            break;
    }
    
    // Check if new position is valid (not a wall)
    if (!checkWall(new_x, new_y)) {
        clearPacman();
        pacman_x = new_x;
        pacman_y = new_y;
        drawPacman();
    }
}

int main(void) {
    setup();
    reset();
    
    // Draw initial pacman
    drawPacman();
    
    while(1) {
        // Check button for direction change
        checkButton();
        
        // Move pacman every N iterations
        moveCounter++;
        if (moveCounter >= 1000) {
            movePacman();
            moveCounter = 0;
        }
        
        // Display the matrix
        updateMatrix(0, 16);
    }
    
    return 0;
}
