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

volatile int row0[32] = {blue,black,yellow,black,black,black,black,black,
                yellow,yellow,yellow,yellow,yellow,yellow,yellow,purple,
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
                yellow,yellow,yellow,yellow,yellow,yellow,yellow,black,
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
                yellow,yellow,yellow,yellow,yellow,yellow,black,black};

volatile int *matrix[16] = {row0,row1,row2,row3,row4,row5,row6,row7,row8,row9,row10,row11,row12,row13,row14,row15};

// Game state
int pacman_x = 5;
int pacman_y = 5;
int direction = 0; // 0=right, 1=down, 2=left, 3=up
int lastButtonState = 1;
int moveCounter = 0;
volatile int buttonDebounce = 0;

int enemy_x = 31;
int enemy_y = 15;
int enemy_dircect = 0;

void setup(void) {
    CLKDIVbits.RCDIV = 0; //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0xffff; //sets all pins to digital I/O
    TRISB = 0b0000000000000011; //and port B to outputs 
    TRISBbits.TRISB5 = 0;
    LATBbits.LATB5 = 0;
    LATBbits.LATB4 = 0;
    LATBbits.LATB3 = 0;
    LATBbits.LATB6 = 0; // SDA1 is input
    LATBbits.LATB7 = 0; // SCL1 is input
    LATBbits.LATB8 = 0; // SDA1 is input
    LATBbits.LATB9 = 0; // SCL1 is input
    LATBbits.LATB10 = 0; // SDA1 is input
    LATBbits.LATB11 = 0; // SCL1 is input
    LATBbits.LATB12 = 0; // SDA1 is input
    LATBbits.LATB13 = 0; // SCL1 is input
//    

    // Wait for the I2C module to stabilize
    delay(20);
//    lcd_init();
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

// Assumes 'data' is an array where:
// Bit 0 = Red
// Bit 1 = Green
// Bit 2 = Blue
// length = total number of pixels in one row (e.g., 32 or 64)
void blast_row_pic24(uint8_t *data, uint16_t length) {
    delay(10000);
    LATBbits.LATB4 = 1;
    delay(1000);
    
    int *ptr = data;
    // Pointers are faster than array indexing (data[i])
//    int count = 0;
    // Loop through every pixel in the row
    do {
        uint8_t pixel = *ptr++;
//        LATBbits.LATB3 = 1;
////        delay(100);
//        LATBbits.LATB3 = 0;
//        int *pixel[3] = *(data++); // Get current pixel color, move to next
//        
//        // --- STEP 1: Set Data Pins ---
//        // We isolate the bit we want and assign it to the pin
//        
//        // If Bit 0 of pixel is 1, set R1 High, else Low
//        if(pixel & 0x01) R1_LAT = PIN_HIGH; 
//        else             R1_LAT = PIN_LOW;
//        
//        // If Bit 1 of pixel is 1, set G1 High, else Low
//        if(pixel & 0x02) G1_LAT = PIN_HIGH;
//        else             G1_LAT = PIN_LOW;
//        
//        // If Bit 2 of pixel is 1, set B1 High, else Low
//        if(pixel & 0x04) B1_LAT = PIN_HIGH;
//        else             B1_LAT = PIN_LOW;
        uint8_t r = pixel & 0x01;
        setcolor(pixel & 0x01 ,pixel & 0x02,pixel & 0x04);

        // --- STEP 2: Pulse Clock ---
        // Data is clocked in on the Rising Edge (Low -> High)
        
        CLK_LAT = PIN_HIGH; // Clock UP (Matrix reads data now)
        
        // Small delay might be needed here depending on cable length/speed
        // Nop(); 
//        delay(10000);
        CLK_LAT = PIN_LOW;  // Clock DOWN
        
    } while(length--);
//    delay(10000);
    LATBbits.LATB4 = 0;
//    delay(1000);
    
    // After the loop finishes, we have shifted out the whole row.
    // The Latch (LAT) pulse happens AFTER this function returns 
    // (usually in the interrupt handler).
}

void reset(){
//    LATBbits.LATB5 = 0;
//    LATBbits.LATB4 = 0;
//    LATBbits.LATB3 = 0;
    LATBbits.LATB6 = 0; // SDA1 is input
    LATBbits.LATB7 = 0; // SCL1 is input
    LATBbits.LATB8 = 0; // SDA1 is input
    LATBbits.LATB9 = 0; // SCL1 is input
    LATBbits.LATB10 = 0; // SDA1 is input
    LATBbits.LATB11 = 0; // SCL1 is input
    LATBbits.LATB12 = 0; // SDA1 is input
//    LATBbits.LATB13 = 0; // SCL1 is input
}

void updateMatrix(int startrow, int FOV) {
    for (int i = startrow; i< startrow + FOV; i++){
        blast_row_pic24(matrix[i],31);
        
        int state9 = (i & 0x01) == 1;
        int state10 = (i & 0x02) == 2;
        int state11 = (i & 0x04) == 4; 
        int state12 = (i & 0x08) == 8;
        LATBbits.LATB4 = 1;
//        delay(1);
        
        LAT_LAT = PIN_HIGH;  // "Save" command
        delay(1);
//        // (A tiny delay might be needed here for very fast chips, e.g., Nop();)
        LAT_LAT = PIN_LOW;  // Release
        LATBbits.LATB9 = state9;
        LATBbits.LATB10 = state10;
        LATBbits.LATB11 = state11; 
        LATBbits.LATB12 = state12;
//        LAT_LAT = PIN_HIGH;
        LATBbits.LATB4 = 0;
//        delay(1);
    }
}



//void changeRow(int *pixels)

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
    int temp = matrix[y][x] == yellow;
    return (matrix[y][x] == yellow);
}

void drawPacman() {
    // Draw pacman as yellow pixel at current position
    if (pacman_y >= 0 && pacman_y < 16 && pacman_x >= 0 && pacman_x < 32) {
        matrix[pacman_y][pacman_x] = cyan;
    }
}

void drawEnemy() {
    if (enemy_y >= 0 && enemy_y < 16 && enemy_x >= 0 && enemy_x < 32) {
        matrix[enemy_y][enemy_x] = red;
    }
}

void clearPacman() {
    // Clear pacman's old position
    if (pacman_y >= 0 && pacman_y < 16 && pacman_x >= 0 && pacman_x < 32) {
        if (matrix[pacman_y][pacman_x] == cyan) {
            matrix[pacman_y][pacman_x] = black;
        }
    }
}

void clearEnemy() {
        // Clear pacman's old position
    if (enemy_y >= 0 && enemy_y < 16 && enemy_x >= 0 && enemy_x < 32) {
        if (matrix[enemy_y][enemy_x] == red) {
            matrix[enemy_y][enemy_x] = black;
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
    if (!(checkWall(new_x, new_y))) {
        clearPacman();
        pacman_x = new_x;
        pacman_y = new_y;
        drawPacman();
    } else {
        drawPacman();
    }
}

void moveEnemy() {
    int new_x = enemy_x;
    int new_y = enemy_y;
    
    // Calculate new position
    if (!(checkWall(new_x+1, new_y))){
        new_x++;
    } else if(!(checkWall(new_x-1, new_y))) {
        new_x--;
    } else if(!(checkWall(new_x, new_y+1))) {
        new_y++;
    } else if(!(checkWall(new_x, new_y-1))) {
        new_y--;
    }
    
    // Check if new position is valid (not a wall)
    if (!(checkWall(new_x, new_y))) {
        clearEnemy();
        enemy_x = new_x;
        enemy_y = new_y;
        drawEnemy();
    } else {
        drawEnemy();
    }
}



int main(void) {
    setup();
    reset();
//    blast_row_pic24(row1,32);
    reset();
    
    drawPacman();
    drawEnemy();

    while(1){
        
        checkButton();
        
        // Move pacman every N iterations
        moveCounter++;
        if (moveCounter >= 10) {
            clearPacman();
            movePacman();
            clearEnemy();
            moveEnemy();
            moveCounter = 0;
        }
        
        updateMatrix(0, 16);
        
//        delay(1);
//        reset();
//        LATBbits.LATB4 = 1;
//        delay(1000);
//        LATBbits.LATB9 = 1;
//        LATBbits.LATB10 = 0;
//        LATBbits.LATB11 = 0; 
//        LATBbits.LATB12 = 0; 
////        blast_row_pic24(arr,16);
////        setcolor(1,1,0);
//        LATBbits.LATB4 = 0;
//        delay(5000);
//        reset();
//        LATBbits.LATB4 = 1;
//        delay(1000);
//        LATBbits.LATB9 = 0;
//        LATBbits.LATB10 = 0;
//        LATBbits.LATB11 = 0; 
//        LATBbits.LATB12 = 0; 
////        blast_row_pic24(arr,16);
////        setcolor(1,1,0);
//        LATBbits.LATB4 = 0;
//        delay(5000);
//        reset();
////              LATBbits.LATB4 = 1;
//        delay(1000);
//        LATBbits.LATB9 = 0;
//        LATBbits.LATB10 = 1;
//        LATBbits.LATB11 = 0; 
//        LATBbits.LATB12 = 0; 
////        blast_row_pic24(arr,16);
////        setcolor(1,1,0);
//        LATBbits.LATB4 = 0;
//        delay(5000);
//        reset();
//        
//        
//        
//        LATBbits.LATB4 = 1;
//        delay(1000);
//        LATBbits.LATB9 = 1;
//        LATBbits.LATB10 = 1;
//        LATBbits.LATB11 = 0; 
//        LATBbits.LATB12 = 0; 
////        blast_row_pic24(arr,16);
////        setcolor(1,1,0);
//        LATBbits.LATB4 = 0;
//        delay(5000);
//        reset();
////        LATBbits.LATB4 = 1;
////        delay(1000);
////        LATBbits.LATB9 = 0;
////        LATBbits.LATB10 = 1;
////        LATBbits.LATB11 = 0; 
////        LATBbits.LATB12 = 0; 
//////        blast_row_pic24(arr,16);
//////        setcolor(1,1,0);
////        LATBbits.LATB4 = 0;
////        delay(5000);
////        reset();
//        LATBbits.LATB4 = 1;
//        delay(1);
//        
//        for (int i = 0; i<32;i++){
//            if (row1[i] == 0x02) {
//                row1[i] = 0x03;
//            } else if (row1[i] == 0x02) {
//                row1[i] = 0x00;
//            }
//        }
//        blast_row_pic24(row1,32);
//        LATBbits.LATB4 = 0;
//        delay(1000);
        
//        LATBbits.LATB4 = 1;
//        delay(1000);
//        LATBbits.LATB9 = 1;
//        LATBbits.LATB10 = 1;
//        LATBbits.LATB11 = 0; 
//        LATBbits.LATB12 = 0; 
////        blast_row_pic24(arr,16);
////        setcolor(1,1,0);
//        LATBbits.LATB4 = 0;
//        delay(5000);
//        reset();
    }
//    return 0;
}
