#ifndef GIO_EXTENDED_H_INCLUDED
#define GIO_EXTENDED_H_INCLUDED

/* getPress -- finds the first press of a button and moves all array elements */
int getPress(unsigned input[], int* signals);
/* getSinglePress -- finds the first press of a single button and moves all array elements */
int getSinglePress(unsigned input[], int* signals);
/* selectNumber -- input a number in unary in some closed interval, uses gio_arrays */
void selectNumber(int from, int to, int* number);


#endif // GIO_EXTENDED_H_INCLUDED
