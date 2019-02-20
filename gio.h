#ifndef GIO_H_INCLUDED
#define GIO_H_INCLUDED

#define GSIZE 5
#define ROWS 3
#define TIME 5000

/* generatePattern -- generates the pattern for a grid of pixels */
void generatePattern(const unsigned image[GSIZE][GSIZE], unsigned pattern[ROWS]);
/* delay -- pause for n microseconds */
void delay(int n);
/* display -- display a pattern for n*15msec */
void display(const unsigned pattern[ROWS], int n);
/* displayN -- display patterns for n*15msec if sum of times is 5000 */
void displayN(const unsigned patterns[][ROWS], const int times[], int patternNumber, int n);
/* displayI -- display a pattern for n*15msec and record input */
void displayI(const unsigned pattern[ROWS], int n, unsigned input[], int* signals);
/* displayNI -- display patterns for n*15msec and record input if sum of times is 5000 */
void displayNI(const unsigned patterns[][ROWS], const int times[], int patternNumber, int n, unsigned input[], int* signals);
/* initGio -- initializes graphics and IO */
void initGio(void);

#endif // GIO_H_INCLUDED
