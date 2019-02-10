#include "gio.h"
#include "hardware.h"

#define COLS 9

static const int rows[GSIZE][GSIZE] = {
    {0, 1, 0, 1, 0},
    {2, 2, 2, 2, 2},
    {1, 0, 1, 2, 1},
    {0, 0, 0, 0, 0},
    {2, 1, 2, 1, 2}
};
static const int cols[GSIZE][GSIZE] = {
    {0, 3, 1, 4, 2},
    {3, 4, 5, 6, 7},
    {1, 8, 2, 8, 0},
    {7, 6, 5, 4, 3},
    {2, 6, 0, 5, 1}
};
static const unsigned rowMasks[ROWS] = {
    0x3ff0, 0x5ff0, 0x9ff0
};
static const unsigned rowMasksFlip[ROWS] = {
    0x2000, 0x4000, 0x8000
};
static const unsigned colMasks[COLS] = {
    0x0010, 0x0020, 0x0040,
    0x0080, 0x0100, 0x0200,
    0x0400, 0x0800, 0x1000,
};
/* generatePattern -- generates the pattern for a grid of pixels */
void generatePattern(const unsigned image[GSIZE][GSIZE], unsigned pattern[ROWS])
{
    for (int i=0;i<ROWS;++i) {
        pattern[i]=rowMasks[i];
    }
    for (int i=0;i<GSIZE;++i) {
        for (int j=0;j<GSIZE;++j) {
            if (image[i][j]) pattern[rows[i][j]]-=colMasks[cols[i][j]];
        }
    }
}
/* generateNumberPattern -- generate a pattern for a number in unary */
void generateNumberPattern(int n, unsigned pattern[ROWS]) {
    int curr=0;
    unsigned flip=0;
    while(n>GSIZE*GSIZE) {
        n-=GSIZE*GSIZE;
        flip=!flip;
    }
    for (int i=0;i<ROWS;++i) {
        if (!flip) pattern[i]=rowMasks[i];
        else pattern[i]=rowMasksFlip[i];
    }
    for (int i=0;i<GSIZE && curr<n;++i) {
        for (int j=0;j<GSIZE && curr<n;++j)
        {
            if (!flip) pattern[rows[i][j]]-=colMasks[cols[i][j]];
            else pattern[rows[i][j]]+=colMasks[cols[i][j]];
            ++curr;
        }
    }
}
/* delay -- pause for n microseconds */
void delay(int n) {
    unsigned t = n << 1;
    while (t > 0) {
        // 500nsec per iteration at 16MHz
        nop(); nop(); nop();
        t--;
    }
}
/* display -- display a pattern n times */
void display(const unsigned pattern[ROWS], int n) {
    for (int i=0;i<n;++i) {
        // Takes 15msec per iteration
        for (int p = 0; p < ROWS; p++) {
            GPIO_OUT = pattern[p];
            delay(5000);
        }
    }
}
unsigned prevA;
unsigned prevB;
int handleInput(int input) {
    unsigned signal=0;
    unsigned currA=!(input & BIT(BUTTON_A));
    unsigned currB=!(input & BIT(BUTTON_B));
    if (currA && !prevA) signal+=1;
    else if (!currA && prevA) signal+=4;
    if (currB && !prevB) signal+=2;
    else if (!currB && prevB) signal+=8;
    prevA=currA;
    prevB=currB;
    return signal;
}
/* displayI -- display a pattern n times and record input */
void displayI(const unsigned pattern[ROWS], int n, unsigned input[], int* signals) {
    unsigned curr;
    for (int i=0;i<n;++i) {
        curr=handleInput(GPIO_IN);
        if (curr) input[(*signals)++]=curr;
        // Takes 15msec per iteration
        for (int p = 0; p < ROWS; p++) {
            GPIO_OUT = pattern[p];
            delay(5000);
        }
    }
}
/* getPress -- finds the first press of a button and moves all array elements */
int getPress(unsigned input[], int* signals) {
    int curr=0;
    int i=0;
    while (!curr &&  i<(*signals)) {
        if (input[i]&1) curr+=1;
        if (input[i]&2) curr+=2;
        ++i;
    }
    for (int j=0;j<(*signals)-i;++j) {
        input[j]=input[i+j];
    }
    (*signals)-=i;
    return curr;
}
/* initGio -- initializes graphics and IO */
void initGio(void) {
    GPIO_DIR = 0xfff0;
    GPIO_PINCNF[BUTTON_A] = 0;
    GPIO_PINCNF[BUTTON_B] = 0;
}
