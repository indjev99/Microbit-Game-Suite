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
/* display -- display a pattern n times */
void display(const unsigned pattern[ROWS], int n);
/* displayI -- display a pattern n times and record input */
void displayI(const unsigned pattern[ROWS], int n, unsigned input[], int* signals);
/* initGio -- initializes graphics and IO */
void initGio(void);

#endif // GIO_H_INCLUDED
