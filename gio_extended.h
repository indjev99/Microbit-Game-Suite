#ifndef GIO_EXTENDED_H_INCLUDED
#define GIO_EXTENDED_H_INCLUDED

#include "gio.h"

/* getPress -- finds the first press of a button and moves all array elements */
int getPress(unsigned input[], int* signals);
/* getSinglePress -- finds the first press of a single button and moves all array elements */
int getSinglePress(unsigned input[], int* signals);
/* generateNumberPattern -- generates a pattern for a number in unary */
void generateNumberPattern(int n, unsigned pattern[ROWS]);
/* selectNumber -- input a number in unary in some closed interval, uses gio_arrays */
void selectNumber(int from, int to, int* number);
/* generateFloatPatterns -- generates the patterns and times for a grid of float pixels */
void generateFloatPatterns(const float image[GSIZE][GSIZE], unsigned patterns[][ROWS], int times[], int patternNumber);


#endif // GIO_EXTENDED_H_INCLUDED
