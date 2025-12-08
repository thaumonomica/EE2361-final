/*
 * File:   meher_lab5_main_v001.c
 * Author: chand
 *
 * Created on September 4, 2025, 2:38 PM
 */


#include "xc.h"
#include <math.h>
#include <stdio.h>
#include "meher_final_asmLib_v001.h"
#include <stdlib.h> // Required for malloc

// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)
// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Fail-Safe Monitor Enabled
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))
     
// Define your specific PIN mappings here
// LAT registers are used for writing to Output pins on PIC24
#define R1_LAT   _LATB6  // Example: Red on Port B, Pin 0
#define G1_LAT   _LATB7   // Example: Green on Port B, Pin 1
#define B1_LAT   _LATB8   // Example: Blue on Port B, Pin 2
#define CLK_LAT  _LATB5  // Clock pin
#define LAT_LAT  _LATB3  // Latch pin
#define OE_LAT   _LATB4  // Output Enable pin
#define BUTTON   _RB2

// Helper to make the code readable
#define PIN_HIGH 1
#define PIN_LOW  0
#define red 0x01
#define green 0x02
#define blue 0x04
#define purple 0x05
#define yellow 0x03
#define cyan 0x06
#define black 0x00
#define white 0x07

volatile int row0[32] = {purple,black,yellow,black,black,black,black,black,
                yellow,yellow,yellow,yellow,yellow,yellow,yellow,black,
                black,black,black,black,black,yellow,yellow,yellow,
                yellow,black,black,black,yellow,yellow,yellow,yellow};

volatile int row1[32] = {yellow,black,black,black,yellow,black,yellow,black,
                black,black,yellow,black,black,black,black,black,
                yellow,yellow,yellow,yellow,black,black,black,black,
                black,black,yellow,black,black,black,black,yellow};

volatile int row2[32] = {black,black,yellow,black,yellow,black,black,black,
                yellow,black,yellow,black,black,yellow,black,yellow,
                black,black,black,yellow,black,black,yellow,black,
                black,yellow,yellow,black,yellow,yellow,black,yellow};

volatile int row3[32] = {black,yellow,black,black,black,black,yellow,black,
                black,black,yellow,black,yellow,black,black,yellow,
                black,yellow,black,black,yellow,black,yellow,black,
                yellow,yellow,yellow,black,yellow,yellow,black,yellow};

volatile int row4[32] = {black,black,yellow,black,yellow,black,black,black,
                yellow,black,black,black,black,black,black,yellow,
                black,yellow,yellow,black,black,black,yellow,black,
                black,black,yellow,black,black,black,black,yellow};

volatile int row5[32] = {yellow,black,black,black,yellow,black,yellow,black,
                yellow,yellow,yellow,yellow,yellow,yellow,black,yellow,
                black,black,black,yellow,black,yellow,yellow,black,
                yellow,yellow,yellow,yellow,yellow,black,black,yellow};

volatile int row6[32] = {yellow,black,yellow,yellow,yellow,black,black,black,
                black,black,black,black,black,black,black,yellow,
                yellow,yellow,black,yellow,black,black,black,black,
                black,black,black,black,black,black,yellow,yellow};

volatile int row7[32] = {black,black,black,black,black,black,yellow,yellow,
                yellow,yellow,yellow,yellow,yellow,yellow,yellow,green,
                black,black,black,black,black,yellow,yellow,black,
                yellow,yellow,black,black,yellow,yellow,black,black};

volatile int row8[32] = {black,black,yellow,yellow,yellow,black,black,black,
                black,black,black,yellow,black,black,black,yellow,
                yellow,black,yellow,yellow,yellow,black,black,black,
                yellow,black,black,yellow,black,black,black,yellow};

volatile int row9[32] = {yellow,black,yellow,black,black,black,yellow,yellow,
                black,yellow,yellow,yellow,black,yellow,black,black,
                black,black,yellow,black,black,black,yellow,black,
                black,yellow,black,black,black,yellow,black,yellow};

volatile int row10[32] = {yellow,black,black,black,yellow,black,black,black,
                black,black,black,black,black,yellow,yellow,yellow,
                yellow,black,yellow,black,yellow,yellow,yellow,yellow,
                black,black,black,black,black,black,black,yellow};

volatile int row11[32] = {yellow,black,yellow,yellow,yellow,black,yellow,black,
                yellow,black,black,yellow,black,black,yellow,black,
                yellow,black,black,black,black,black,black,black,
                black,yellow,black,yellow,black,yellow,black,yellow};

volatile int row12[32] = {black,black,yellow,black,black,black,black,black,
                black,yellow,black,yellow,yellow,black,black,black,
                yellow,yellow,yellow,yellow,black,yellow,black,yellow,
                black,black,black,black,black,black,black,yellow};

volatile int row13[32] = {black,yellow,yellow,yellow,black,yellow,yellow,black,
                yellow,black,black,black,yellow,yellow,yellow,black,
                yellow,black,black,black,black,yellow,black,yellow,
                black,yellow,black,yellow,black,yellow,black,yellow};

volatile int row14[32] = {black,black,black,black,black,yellow,black,black,
                black,black,yellow,black,yellow,black,black,black,
                black,black,yellow,black,black,black,black,black,
                black,black,black,black,black,black,black,yellow};

volatile int row15[32] = {yellow,yellow,yellow,yellow,black,black,black,yellow,
                yellow,black,black,black,black,black,yellow,black,
                yellow,black,black,black,yellow,yellow,yellow,yellow,
                yellow,yellow,yellow,yellow,yellow,yellow,black,red};

volatile int *matrix[16] = {row0,row1,row2,row3,row4,row5,row6,row7,row8,row9,row10,row11,row12,row13,row14,row15};


// Game state
volatile int pacman_x = 5;
volatile int pacman_y = 5;
volatile int direction = 0; // 0 = right, 1 = down, 2 = left, 3 = up
volatile int lastButtonState = 1; 
volatile int moveCounter = 0;
volatile int buttonDebounce = 0;

void setup(void) {
    CLKDIVbits.RCDIV = 0; //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0xffff; //sets all pins to digital I/O
    TRISB = 0b0000000000000111; //Set RB2 as input (button), rest as outputs
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

    // Wait for the I2C module to stabilize
    delay(20);
}

void delay(long n) {
    for (; n > 0; n--) {
        asm("nop");
    }
}

void setcolor(int  r, int  g, int  b){
    LATBbits.LATB6 = 0;
    LATBbits.LATB7 = 0;
    LATBbits.LATB8 = 0;
    if (r == 1){
        LATBbits.LATB6 = 1;
    }
    if (g == 2){
        LATBbits.LATB7 = 1;
    }
    if (b == 4){
        LATBbits.LATB8 = 1;
    }
}

#include <xc.h> // Standard PIC include

void blast_row_pic24(uint8_t *data, uint16_t length) {
    delay(10000);
    LATBbits.LATB4 = 1;
    delay(1000);
    
    int *ptr = data;
    
    do {
        uint8_t pixel = *ptr++;
        uint8_t r = pixel & 0x01;
        setcolor(pixel & 0x01 ,pixel & 0x02,pixel & 0x04);

        CLK_LAT = PIN_HIGH;
        CLK_LAT = PIN_LOW;
        
    } while(length--);
    
    LATBbits.LATB4 = 0;
}

void reset(){
    LATBbits.LATB6 = 0;
    LATBbits.LATB7 = 0;
    LATBbits.LATB8 = 0;
    LATBbits.LATB9 = 0;
    LATBbits.LATB10 = 0;
    LATBbits.LATB11 = 0;
    LATBbits.LATB12 = 0;
}

void updateMatrix(int startrow, int FOV) {
    for (int i = startrow; i< startrow + FOV; i++){
        blast_row_pic24(matrix[i],31);
        
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
    
    // Button pressed (active low) with debounce counter
    if (currentButtonState == 0 && lastButtonState == 1 && buttonDebounce == 0) {
        direction = (direction + 1) % 4; // Rotate clockwise: right->down->left->up
        buttonDebounce = 1000; // Set debounce counter
    }
    
    // Decrement debounce counter
    if (buttonDebounce > 0) {
        buttonDebounce--;
    }
    
    lastButtonState = currentButtonState;
}

int checkWall(int x, int y) {
    if (y < 0 || y >= 16 || x < 0 || x >= 32) return 1;
    return (matrix[y][x] == yellow);
}

void drawPacman() {
    // Draw pacman as cyan pixel at current position
    if (pacman_y >= 0 && pacman_y < 16 && pacman_x >= 0 && pacman_x < 32) {
        matrix[pacman_y][pacman_x] = cyan;
    }
}

void clearPacman() {
    // Clear pacman's old position (set to black)
    if (pacman_y >= 0 && pacman_y < 16 && pacman_x >= 0 && pacman_x < 32) {
        if (matrix[pacman_y][pacman_x] == cyan) {
            matrix[pacman_y][pacman_x] = black;
        }
    }
}

void movePacman() {
    int new_x = pacman_x;
    int new_y = pacman_y;
    
    // Calculate new position based on direction
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
    // If it hits a wall, pacman stays in place (no movement)
}

int main(void) {
    setup();
    reset();
    
    // Draw pacman at starting position
    drawPacman();

    while(1){
        // Check button state
        checkButton();
        
        // Move pacman every N iterations (adjust speed here)
        moveCounter++;
        if (moveCounter >= 100) { // Increase this number to slow down, decrease to speed up
            movePacman();
            moveCounter = 0;
        }
        
        // Update display
        updateMatrix(0, 16);
    }
}
