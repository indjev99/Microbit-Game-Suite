#include "gio_extended.h"
#include "gio.h"
#include "gio_arrays.h"

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
/* getSinglePress -- finds the first press of a single button and moves all array elements */
int getSinglePress(unsigned input[], int* signals) {
    int curr;
    do {
        curr=getPress(input,signals);
    } while (curr==3);
    return curr;
}
/* selectNumber -- input a number in unary in some closed interval, uses gio_arrays */
void selectNumber(int from, int to, int* number) {
    int curr=0;
    signals=0;
    while (!curr) {
        generateNumberPattern(*number,pat1);
        displayI(pat1,5,input,&signals);
        curr=getPress(input,&signals);
        if (curr>1) {
            ++(*number);
            if ((*number) > to) (*number)=from;
            curr-=2;
        }
    }
    signals=0;
}


