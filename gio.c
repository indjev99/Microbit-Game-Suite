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
static const unsigned colMasks[COLS] = {
    0x0010, 0x0020, 0x0040,
    0x0080, 0x0100, 0x0200,
    0x0400, 0x0800, 0x1000
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
/* delay -- pause for n microseconds */
void delay(int n) {
    unsigned t=n<<1;
    while (t>0) {
        // 500nsec per iteration at 16MHz
        nop(); nop(); nop();
        t--;
    }
}
/* display -- display a pattern for n*15msec */
void display(const unsigned pattern[ROWS], int n) {
    for (int i=0;i<n;++i) {
        for (int r=0;r<ROWS;++r) {
            GPIO_OUT = pattern[r];
            delay(TIME);
        }
    }
}
/* displayN -- display patterns for n*15msec if sum of times is 5000 */
void displayN(const unsigned patterns[][ROWS], const int times[], int patternNumber, int n) {
    for (int i=0;i<n;++i) {
        for (int j=0;j<patternNumber;++j) {
            for (int r=0;r<ROWS;++r) {
                GPIO_OUT = patterns[j][r];
                delay(times[j]);
            }
        }
    }
}
static unsigned prevA;
static unsigned prevB;
static int handleInput(int input) {
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
/* displayI -- display a pattern for n*15msec and record input */
void displayI(const unsigned pattern[ROWS], int n, unsigned input[], int* signals) {
    unsigned curr;
    for (int i=0;i<n;++i) {
        curr=handleInput(GPIO_IN);
        if (curr) input[(*signals)++]=curr;
        for (int r=0;r<ROWS;++r) {
            GPIO_OUT = pattern[r];
            delay(TIME);
        }
    }
}
/* displayNI -- display patterns for n*15msec and record input if sum of times is 5000 */
void displayNI(const unsigned patterns[][ROWS], const int times[], int patternNumber, int n, unsigned input[], int* signals) {
    unsigned curr;
    for (int i=0;i<n;++i) {
        curr=handleInput(GPIO_IN);
        if (curr) input[(*signals)++]=curr;
        for (int j=0;j<patternNumber;++j) {
            for (int r=0;r<ROWS;++r) {
                GPIO_OUT = patterns[j][r];
                delay(times[j]);
            }
        }
    }
}
/* initGio -- initializes graphics and IO */
void initGio(void) {
    GPIO_DIR=0xfff0;
    GPIO_PINCNF[BUTTON_A]=0;
    GPIO_PINCNF[BUTTON_B]=0;
    prevA=0;
    prevB=0;
}
