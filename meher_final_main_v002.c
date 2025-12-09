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

volatile int row3[32] = {yellow,black,black,black,black,black,yellow,black,
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
                yellow,yellow,yellow,yellow,yellow,yellow,black,black};

volatile int *matrix[16] = {row0,row1,row2,row3,row4,row5,row6,row7,row8,row9,row10,row11,row12,row13,row14,row15};

// Game state
<<<<<<< HEAD
int pacman_x = 0;
int pacman_y = 0;
int direction = 1 ; // 0=right, 1=down, 2=left, 3=up
int lastButtonState = 1;
int moveCounter = 0;
=======
volatile int pacman_x = 0;
volatile int pacman_y = 0;
volatile int direction = 0; // 0: right & purple, 1: down & cyan, 2: left & white, 3: up & blue
volatile int lastButtonState = 1; // 0: pressed, 1: NOT pressed
volatile int moveCounter = 0;
>>>>>>> 87d919e1ae5a870a2f0687ddab2eeece9640d6f0
volatile int buttonDebounce = 0;

volatile unsigned long lastCaptureTime = 0;
volatile unsigned long captureTime = 0;
volatile unsigned int debouncePeriod = 17000;
volatile unsigned int count = 0;

int enemy_x = 31;
int enemy_y = 15;
int enemy_dircect = 0;

volatile int gameCont = 0;
volatile int winLoss  = 0;
volatile int gameStart = 0;

void initPushButton(void) {
    // 1. Digital Mode Configuration
    // Ensure RB2 is digital. (AD1PCFG logic depends on your specific PIC24)
    // Assuming 1 = Digital. Check if RB2 (AN2) needs bit 2 set.
    AD1PCFG = 0xFFFF; // SAFEST: Set ALL pins to digital to avoid analog issues

    // 2. Configure Pin Direction for RB2
    TRISBbits.TRISB2 = 1;    // Set RB2 as Input

    // 3. Peripheral Pin Select (PPS) - The Critical Step!
    // Connect Input Capture 1 (IC1) to Pin RP2 (which is usually RB2)
    // Note: You must unlock registers if OSCCON lock is active (omitted for simplicity)
    __builtin_write_OSCCONL(OSCCON & 0xbf); // Unlock PPS
    RPINR7bits.IC1R = 2;     // Map IC1 to RP2 (Pin RB2)
    __builtin_write_OSCCONL(OSCCON | 0x40); // Lock PPS

    // 4. Configure Pull-Up Resistor for RB2
    // On many PIC24s, RB2 corresponds to Change Notification pin CN6.
    // Check your specific datasheet! (e.g., PIC24FJ64GA002)
    CNPU1bits.CN6PUE = 1;    // Enable pull-up on RB2 (CN6)

    // 5. Timer 2 Setup (Unchanged)
    T2CON = 0;
    T2CONbits.TCKPS = 0b11;  // Prescaler 1:256
    TMR2 = 0;
    PR2 = 62500-1;
    _T2IF = 0;
    _T2IP = 3;
    _T2IE = 1;
    T2CON = 0x8000;          // Enable Timer 2

    // 6. Input Capture Setup (Unchanged)
    IC1CON = 0;              // Reset IC1
    IC1CONbits.ICTMR = 1;    // Select Timer 2 as the time base
    IC1CONbits.ICM = 0b011;  // Capture on every rising edge
}


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

void __attribute__((interrupt, auto_psv)) _T2Interrupt(){
    IFS0bits.T2IF = 0;
}

void __attribute__((__interrupt__, __auto_psv__)) _IC1Interrupt(void)
{
   _IC1IF = 0;
   checkButton();
   if (gameStart == 0) {
       gameCont = 1;
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
    
    
    unsigned int icVal = IC1BUF;   // Read captured timer value
    unsigned int captureTime = icVal;           // Could be stored for analysis

    // Compute time since last valid event
    unsigned long now = TMR2;
    unsigned long delta;

    if (now >= lastCaptureTime)
    {
        delta = now - lastCaptureTime;
    }
    else
    {
        delta = (0xFFFF - lastCaptureTime) + now;
    }
    
    if (delta > debouncePeriod/4)
    {
        // This is a valid press (not bounce)
        lastCaptureTime = now;
        direction = (direction + 1) % 4; // Rotate clockwise: right->down->left->up
        lastButtonState = 0;
    }
}

int checkWall(int x, int y) {
    if (y < 0 || y >= 16 || x < 0 || x >= 32) return 1;
    int temp = matrix[y][x] == yellow;
    return (matrix[y][x] == yellow);
}

void drawPacman() {
        // Draw pacman as yellow pixel at current position
    if (pacman_y >= 0 && pacman_y < 16 && pacman_x >= 0 && pacman_x < 32) {
        if (direction == 0) { // moving right makes player purple
            matrix[pacman_y][pacman_x] = purple;
        }
        else if (direction == 1) { // moving down makes player cyan
            matrix[pacman_y][pacman_x] = cyan;
        }
        else if (direction == 2) { // moving left makes player white
            matrix[pacman_y][pacman_x] = white;
        }
        else if (direction == 3) { // moving up makes player blue
            matrix[pacman_y][pacman_x] = blue;
        }
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
        if (matrix[pacman_y][pacman_x] == purple) {
            matrix[pacman_y][pacman_x] = black;
        } else if (matrix[pacman_y][pacman_x] == cyan) {
            matrix[pacman_y][pacman_x] = black;
        } else if (matrix[pacman_y][pacman_x] == white) {
            matrix[pacman_y][pacman_x] = black;
        } else if (matrix[pacman_y][pacman_x] == blue) {
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

    int dx = pacman_x - enemy_x;
    int dy = pacman_y - enemy_y;

    // Prefer moving along the axis where the distance is larger
    if (abs(dx) >= abs(dy)) {
        // Try horizontal first
        if (dx > 0 && !checkWall(enemy_x + 1, enemy_y)) {
            new_x = enemy_x + 1;
        } else if (dx < 0 && !checkWall(enemy_x - 1, enemy_y)) {
            new_x = enemy_x - 1;
        }
        // If horizontal blocked, try vertical
        else if (dy > 0 && !checkWall(enemy_x, enemy_y + 1)) {
            new_y = enemy_y + 1;
        } else if (dy < 0 && !checkWall(enemy_x, enemy_y - 1)) {
            new_y = enemy_y - 1;
        }
    } else {
        // Prefer vertical first
        if (dy > 0 && !checkWall(enemy_x, enemy_y + 1)) {
            new_y = enemy_y + 1;
        } else if (dy < 0 && !checkWall(enemy_x, enemy_y - 1)) {
            new_y = enemy_y - 1;
        }
        // If vertical blocked, try horizontal
        else if (dx > 0 && !checkWall(enemy_x + 1, enemy_y)) {
            new_x = enemy_x + 1;
        } else if (dx < 0 && !checkWall(enemy_x - 1, enemy_y)) {
            new_x = enemy_x - 1;
        }
    }

    // Only move if we actually found a valid new tile
    if (!(checkWall(new_x, new_y))) {
        clearEnemy();
        enemy_x = new_x;
        enemy_y = new_y;
    }

    // Always draw after resolving position
    drawEnemy();
}

int gameLosscheck(){
    if(pacman_x == enemy_x && enemy_y== pacman_y){
        return 0;
    } else {
       return 1; 
    }
    
}

int gameWincheck(){
    if(pacman_x == 15 && pacman_y== 7){
        return 1;
    } else {
       return 0; 
    }
    
}


void setLoseScreen(void) {
    int loseRow5[32] = {black,
            white,black,black,black,white, // letter Y
            black,
            white,white,white, // letter O
            black,
            white,black,white, // letter U
            black,black,
    
            white,black,black, // letter L
            black,
            white,white,white, // letter O
            black,
            white,white,white, // letter S
            black,
            white,white,white, // letter E
            black};
    
    int loseRow6[32] = {black,
            black,white,black,white,black, // letter Y
            black,
            white,black,white, // letter O
            black,
            white,black,white, // letter U
            black,black,
    
            white,black,black, // letter L
            black,
            white,black,white, // letter O
            black,
            white,black,black, // letter S
            black,
            white,black,black, // letter E
            black};
    
    int loseRow7[32] = {black,
            black,black,white,black,black, // letter Y
            black,
            white,black,white, // letter O
            black,
            white,black,white, // letter U
            black,black,
    
            white,black,black, // letter L
            black,
            white,black,white, // letter O
            black,
            white,white,white, // letter S
            black,
            white,white,black, // letter E
            black};
    
    int loseRow8[32] = {black,
            black,black,white,black,black, // letter Y
            black,
            white,black,white, // letter O
            black,
            white,black,white, // letter U
            black,black,
    
            white,black,black, // letter L
            black,
            white,black,white, // letter O
            black,
            black,black,white, // letter S
            black,
            white,black,black, // letter E
            black};
    
    int loseRow9[32] = {black,
            black,black,white,black,black, // letter Y
            black,
            white,white,white, // letter O
            black,
            white,white,white, // letter U
            black,black,
    
            white,white,white, // letter L
            black,
            white,white,white, // letter O
            black,
            white,white,white, // letter S
            black,
            white,white,white, // letter E
            black};
    
    for(int i = 0; i < 32; i++) {
        matrix[5][i] = loseRow5[i];
        matrix[6][i] = loseRow6[i];
        matrix[7][i] = loseRow7[i];
        matrix[8][i] = loseRow8[i];
        matrix[9][i] = loseRow9[i];
    }
}

void setStartScreen() {
    int startRow2[32] = {black,
            white,white,white,white,white, // letter S
            black,
            white,white,white,white,white, // letter T
            black,
            black,black,white,black,black, // letter A
            black,
            white,white,white,white,black, // letter R
            black,
            white,white,white,white,white, // letter T
            black};
    
    int startRow3[32] = {black,
            white,black,black,black,black, // letter S
            black,
            black,black,white,black,black, // letter T
            black,
            black,white,black,white,black, // letter A
            black,
            white,black,black,black,white, // letter R
            black,
            black,black,white,black,black, // letter T
            black};
    
    int startRow4[32] = {black,
            white,white,white,white,white, // letter S
            black,
            black,black,white,black,black, // letter T
            black,
            black,white,white,white,black, // letter A
            black,
            white,white,white,white,black, // letter R
            black,
            black,black,white,black,black, // letter T
            black};
    
    int startRow5[32] = {black,
            black,black,black,black,white, // letter S
            black,
            black,black,white,black,black, // letter T
            black,
            white,black,black,black,white, // letter A
            black,
            white,black,black,black,white, // letter R
            black,
            black,black,white,black,black, // letter T
            black};
 
    int startRow6[32] = {black,
            white,white,white,white,white, // letter S
            black,
            black,black,white,black,black, // letter T
            black,
            white,black,black,black,white, // letter A
            black,
            white,black,black,black,white, // letter R
            black,
            black,black,white,black,black, // letter T
            black};
 
    /*
    int startRow8[32] = {black,black,black,black,
            white,white,white,white,white, // letter G
            black,
            black,black,white,black,black, // letter A
            black,
            white,black,black,black,white, // letter M
            black,
            white,white,white,white,white, // letter E
            black,black,black,black};
    
    int startRow9[32] = {black,black,black,black,
            white,black,black,black,black, // letter G
            black,
            black,white,black,white,black, // letter A
            black,
            white,white,black,white,white, // letter M
            black,
            white,black,black,black,black, // letter E
            black,black,black,black};
    
    int startRow10[32] = {black,black,black,black,
            white,black,white,white,white, // letter G
            black,
            black,white,white,white,black, // letter A
            black,
            white,black,white,black,white, // letter M
            black,
            white,white,white,black,black, // letter E
            black,black,black,black};
    
    int startRow11[32] = {black,black,black,black,
            white,black,black,black,white, // letter G
            black,
            white,black,black,black,white, // letter A
            black,
            white,black,black,black,white, // letter M
            black,
            white,black,black,black,black, // letter E
            black,black,black,black};
    
    int startRow12[32] = {black,black,black,black,
            white,white,white,white,white, // letter G
            black,
            white,black,black,black,white, // letter A
            black,
            white,black,black,black,white, // letter M
            black,
            white,white,white,white,white, // letter E
            black,black,black,black};
    */
    
    for (int i = 0; i < 32; i++) {
        /*
        matrix[2][i] = startRow2[i];
        matrix[3][i] = startRow3[i];
        matrix[4][i] = startRow4[i];
        matrix[5][i] = startRow5[i];
        matrix[6][i] = startRow6[i];
        matrix[7][i] = black;
        matrix[8][i] = startRow8[i];
        matrix[9][i] = startRow9[i];
        matrix[10][i] = startRow10[i];
        matrix[11][i] = startRow11[i];
        matrix[12][i] = startRow12[i];
         */
        
        matrix[5][i] = startRow2[i];
        matrix[6][i] = startRow3[i];
        matrix[7][i] = startRow4[i];
        matrix[8][i] = startRow5[i];
        matrix[9][i] = startRow6[i];
    }
}

void setWinScreen(void) {
    int endRow5[32] = {black,
            white,black,black,black,white, // letter Y
            black,
            white,white,white,white, // letter O
            black,
            white,black,black,white, // letter U
            black,
            white,black,black,black,black,white, // letter W
            black,
            white, // letter I
            black,
            white,black,black,black,white, // letter N
            black};
 
    int endRow6[32] = {black,
            black,white,black,white,black, // letter Y
            black,
            white,black,black,white, // letter O
            black,
            white,black,black,white, // letter U
            black,
            white,black,black,black,black,white, // letter W
            black,
            white, // letter I
            black,
            white,white,black,black,white, // letter N
            black};
 
    int endRow7[32] = {black,
            black,black,white,black,black, // letter Y
            black,
            white,black,black,white, // letter O
            black,
            white,black,black,white, // letter U
            black,
            white,black,white,white,black,white, // letter W
            black,
            white, // letter I
            black,
            white,white,white,black,white, // letter N
            black};
 
    int endRow8[32] = {black,
            black,black,white,black,black, // letter Y
            black,
            white,black,black,white, // letter O
            black,
            white,black,black,white, // letter U
            black,
            black,white,black,black,white,black, // letter W
            black,
            white, // letter I
            black,
            white,black,white,white,white, // letter N
            black};
 
    int endRow9[32] = {black,
            black,black,white,black,black, // letter Y
            black,
            white,white,white,white, // letter O
            black,
            white,white,white,white, // letter U
            black,
            black,white,black,black,white,black, // letter W
            black,
            white, // letter I
            black,
            white,black,black,white,white, // letter N
            black};
    
    for (int i = 0; i < 32; i++) {
        /*
        matrix[2][i] = startRow2[i];
        matrix[3][i] = startRow3[i];
        matrix[4][i] = startRow4[i];
        matrix[5][i] = startRow5[i];
        matrix[6][i] = startRow6[i];
        matrix[7][i] = black;
        matrix[8][i] = startRow8[i];
        matrix[9][i] = startRow9[i];
        matrix[10][i] = startRow10[i];
        matrix[11][i] = startRow11[i];
        matrix[12][i] = startRow12[i];
         */
        
        matrix[5][i] = endRow5[i];
        matrix[6][i] = endRow6[i];
        matrix[7][i] = endRow7[i];
        matrix[8][i] = endRow8[i];
        matrix[9][i] = endRow9[i];
    }
}

void resetGameMaze(void) {
    int trow5[32] = {yellow,black,black,black,yellow,black,yellow,black,
                yellow,yellow,yellow,yellow,yellow,yellow,black,yellow,
                black,black,black,yellow,black,yellow,yellow,black,
                yellow,yellow,yellow,yellow,yellow,black,black,yellow};

    int trow6[32] = {yellow,black,yellow,yellow,yellow,black,black,black,
                black,black,black,black,black,black,black,yellow,
                yellow,yellow,black,yellow,black,black,black,black,
                black,black,black,black,black,black,yellow,yellow};

    int trow7[32] = {black,black,black,black,black,black,yellow,yellow,
                yellow,yellow,yellow,yellow,yellow,yellow,yellow,green,
                black,black,black,black,black,yellow,yellow,black,
                yellow,yellow,black,black,yellow,yellow,black,black};

    int trow8[32] = {black,black,yellow,yellow,yellow,black,black,black,
                black,black,black,yellow,black,black,black,yellow,
                yellow,black,yellow,yellow,yellow,black,black,black,
                yellow,black,black,yellow,black,black,black,yellow};

    int trow9[32] = {yellow,black,yellow,black,black,black,yellow,yellow,
                black,yellow,yellow,yellow,black,yellow,black,black,
                black,black,yellow,black,black,black,yellow,black,
                black,yellow,black,black,black,yellow,black,yellow};
    for(int i = 0; i < 32; i++) {
        matrix[5][i] = trow5[i];
        matrix[6][i] = trow6[i];
        matrix[7][i] = trow7[i];
        matrix[8][i] = trow8[i];
        matrix[9][i] = trow9[i];
    }
}

int main(void) {
    setup();
    initPushButton();
    reset();
//    blast_row_pic24(row1,32);
    reset();
    
    drawPacman();
    drawEnemy();
    

    _IC1IE = 1;
    while(1){
        setStartScreen(); // Prepares the start screen
        lastButtonState = 1;
        while (lastButtonState) { // If they press the button, start the game
            updateMatrix(5, 5); // Display the start screen
        }
        resetGameMaze(); // Resets the part of the maze used for a screen
        gameCont = 1; // Allows for the game to be replayed after a win/loss
        pacman_x = 0;
        pacman_y = 0;
        enemy_x = 31;
        enemy_y = 15;
        while (gameCont){
        
            // Move pacman every N iterations
            moveCounter++;
            if (moveCounter >= 10) {
                if(gameLosscheck() == 0){
                    gameCont = 0;
                    winLoss = 0;
                } else if(gameWincheck() == 1){
                    gameCont = 0;
                    winLoss = 1;
                }
                clearPacman();
                movePacman();
                clearEnemy();
                moveEnemy();
                moveCounter = 0;
            }
            
            if (pacman_y < 2) {
                // Handles when player is in the top two rows
                updateMatrix(0, 5);
            } else if (pacman_y > 13) {
                // Handles when player is in the bottom two rows
                updateMatrix(11, 5);
            } else {
                // Shows the area of the maze with the player inside
                updateMatrix(pacman_y-2, 5);
            }
        }
        
        // Determines which end screen to display
        if (winLoss) {
            setWinScreen();
        } else {
            setLoseScreen();
        }
        
        // Displays the end screen until they press the button
        lastButtonState = 1;
        while (lastButtonState) {
            updateMatrix(5, 5); // Displays the end screen
        }
        
        
        
//        delay(1);
//        reset();
//        LATBbits.LATB4 = 1;
//        delay(1000);
//        setcolor(1,1,0);
//        LATBbits.LATB9 = 1;
//        LATBbits.LATB10 = 0;
//        LATBbits.LATB11 = 0; 
//        LATBbits.LATB12 = 0; 
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
////        
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
