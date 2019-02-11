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
/* display -- display between two patterns for n*15msec */
void display2(const unsigned pattern1[ROWS], const unsigned pattern2[ROWS], double p, int n);
/* displayI -- display a pattern for n*15msec and record input */
void displayI(const unsigned pattern[ROWS], int n, unsigned input[], int* signals);
/* display2I -- display between two patterns pattern for n*15msec and record input */
void display2I(const unsigned pattern[ROWS], const unsigned pattern2[ROWS], double p, int n, unsigned input[], int* signals);
/* initGio -- initializes graphics and IO */
void initGio(void);

#endif // GIO_H_INCLUDED
