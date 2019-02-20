#include "gio_extended.h"
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
/* generateNumberPattern -- generates a pattern for a number in unary */
void generateNumberPattern(int n, unsigned pattern[ROWS]) {
    int curr=0;
    unsigned flip=0;
    while(n>GSIZE*GSIZE) {
        n-=GSIZE*GSIZE;
        flip=!flip;
    }
    for (int i=0;i<GSIZE;++i) {
        for (int j=0;j<GSIZE;++j) {
            if (curr<n) image[i][j]=!flip;
            else image[i][j]=flip;
            ++curr;
        }
    }
    generatePattern(image,pattern);
}
/* selectNumber -- input a number in unary in some closed interval, uses gio_arrays */
void selectNumber(int from, int to, int* number) {
    int curr=0;
    signals=0;
    while (!curr) {
        generateNumberPattern(*number,pat[0]);
        displayI(pat[0],5,input,&signals);
        curr=getPress(input,&signals);
        if (curr>1) {
            ++(*number);
            if ((*number) > to) (*number)=from;
            curr-=2;
        }
    }
    signals=0;
}
static float qExp(float x) {
    if (x<0.01) return 1+x+x*x/2;
    float ans=qExp(x/2);
    return ans*ans;
}
static const float k=3.3;
static int convert(float p) {
    return TIME*(qExp(k*p)-1)/(qExp(k)-1)+0.5;
}
static int convertedImage[GSIZE][GSIZE];
/* generateFloatPatterns -- generates the patterns and times for a grid of float pixels */
void generateFloatPatterns(const float imageF[GSIZE][GSIZE], unsigned patterns[][ROWS], int times[], int patternNumber) {
    for (int i=0;i<patternNumber;++i) {
        times[i]=0;
    }
    for (int i=0;i<GSIZE;++i) {
        for (int j=0;j<GSIZE;++j) {
            convertedImage[i][j]=convert(imageF[i][j]);
        }
    }
    int cpow=1;
    for (int t=1;t<patternNumber;++t) {
        cpow*=2;
        if (cpow>TIME) {
            patternNumber=t;
            cpow/=2;
            break;
        }
    }
    int leftOver=TIME-cpow+1;
    times[0]=leftOver;
    for (int i=0;i<GSIZE;++i) {
        for (int j=0;j<GSIZE;++j) {
            if (convertedImage[i][j]>=cpow) {
                convertedImage[i][j]-=leftOver;
                image[i][j]=1;
            }
            else image[i][j]=0;
        }
    }
    generatePattern(image,pat[0]);
    for (int t=1;t<patternNumber;++t) {
        cpow/=2;
        times[t]=cpow;
        for (int i=0;i<GSIZE;++i) {
            for (int j=0;j<GSIZE;++j) {
                if (convertedImage[i][j]>=cpow) {
                    convertedImage[i][j]-=cpow;
                    image[i][j]=1;
                }
                else image[i][j]=0;
            }
        }
        generatePattern(image,pat[t]);
    }
}
