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

static int convert[] = {0, 3, 6, 9, 12, 15, 18, 22, 25, 28, 32, 35, 38, 42, 45, 49, 52, 56, 60, 63, 67, 71, 75, 79, 83, 87, 91, 95, 99, 103, 107, 112, 116, 120, 125, 129, 134, 138, 143, 148, 153, 157, 162, 167, 172, 177, 182, 188, 193, 198, 203, 209, 214, 220, 226, 231, 237, 243, 249, 255, 261, 267, 273, 280, 286, 292, 299, 305, 312, 319, 326, 333, 340, 347, 354, 361, 368, 376, 383, 391, 399, 406, 414, 422, 430, 438, 447, 455, 464, 472, 481, 490, 498, 507, 517, 526, 535, 544, 554, 564, 573, 583, 593, 604, 614, 624, 635, 645, 656, 667, 678, 689, 700, 712, 723, 735, 747, 759, 771, 783, 796, 808, 821, 834, 847, 860, 873, 887, 900, 914, 928, 942, 957, 971, 986, 1001, 1016, 1031, 1046, 1062, 1078, 1094, 1110, 1126, 1143, 1160, 1177, 1194, 1211, 1229, 1246, 1264, 1283, 1301, 1320, 1338, 1358, 1377, 1396, 1416, 1436, 1457, 1477, 1498, 1519, 1540, 1562, 1583, 1605, 1628, 1650, 1673, 1696, 1720, 1743, 1767, 1792, 1816, 1841, 1866, 1892, 1917, 1943, 1970, 1997, 2024, 2051, 2079, 2107, 2135, 2164, 2193, 2222, 2252, 2282, 2312, 2343, 2374, 2406, 2438, 2470, 2503, 2536, 2569, 2603, 2638, 2672, 2707, 2743, 2779, 2815, 2852, 2890, 2927, 2966, 3004, 3043, 3083, 3123, 3164, 3205, 3246, 3288, 3331, 3374, 3418, 3462, 3506, 3551, 3597, 3643, 3690, 3738, 3786, 3834, 3883, 3933, 3983, 4034, 4086, 4138, 4191, 4244, 4298, 4353, 4408, 4464, 4521, 4578, 4637, 4695, 4755, 4815, 4876, 4938, 5000};

void generateAnaloguePatterns(const int imageAnalogue[GSIZE][GSIZE], unsigned patterns[][ROWS], int times[], int patternNumber) {
    static int convertedImage[GSIZE][GSIZE];
    for (int i=0;i<patternNumber;++i) {
        times[i]=0;
    }
    for (int i=0;i<GSIZE;++i) {
        for (int j=0;j<GSIZE;++j) {
            convertedImage[i][j]=convert[imageAnalogue[i][j]];
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
