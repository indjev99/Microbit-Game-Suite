#ifndef GIO_H_INCLUDED
#define GIO_H_INCLUDED

#define GSIZE 5
#define ROWS 3

/* generatePattern -- generates the pattern for a grid of pixels */
void generatePattern(const unsigned image[GSIZE][GSIZE], unsigned pattern[ROWS]);
/* generateNumberPattern -- generate a pattern for a number in unary */
void generateNumberPattern(int n, unsigned pattern[ROWS]);
/* delay -- pause for n microseconds */
void delay(int n);
/* display -- display a pattern for n*15msec */
void display(const unsigned pattern[ROWS], int n);
/* displayN -- display k patterns for n*15msec */
void displayN(const unsigned pattern[][ROWS], const double p[], const int k, int n);
/* displayI -- display a pattern for n*15msec and record input */
void displayI(const unsigned pattern[ROWS], int n, unsigned input[], int* signals);
/* displayNI -- display k patterns for n*15msec and record input */
void displayNI(const unsigned pattern[][ROWS], const double p[], const int k, int n, unsigned input[], int* signals);
/* initGio -- initializes graphics and IO */
void initGio(void);

#endif // GIO_H_INCLUDED
