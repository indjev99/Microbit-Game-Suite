#include "pong_game.h"
#include "gio_extended.h"
#include "gio_arrays.h"
#include "rng.h"

#define EPS 1e-6

const unsigned pongGamePreview[ROWS] = {
    0x3ff0, 0x5bf0, 0x8fe0
};

struct point {
    double x, y;
};

static void movePoint(struct point* location, struct point* velocity) {
    location->x+=velocity->x;
    location->y+=velocity->y;
    while (location->x<0 || location->x>GSIZE-1) {
        if (location->x<0) location->x=-location->x;
        else location->x=2*GSIZE-2-location->x;
        velocity->x=-velocity->x;
    }
    while (location->y<0 || location->y>GSIZE-1) {
        if (location->y<0) location->y=-location->y;
        else location->y=2*GSIZE-2-location->y;
        velocity->y=-velocity->y;
    }
}

static struct point ballVelocity;
static struct point ballLocation;
static int score;
static int FPSTEP=2;

static void resetGame(void) {
    ballVelocity.x=0.025;
    ballVelocity.y=0.0625;
    ballLocation.x=1.5;//rand()%(GSIZE*10)/10.0;
    ballLocation.y=1.5;//rand()%(GSIZE*10)/10.0;
    score=0;
}
static void genImageAndPats(void) {
    for (int i=0;i<GSIZE;++i) {
        for (int j=0;j<GSIZE;++j) {
            imageF[i][j]=0;//(i*GSIZE+j)*1.0/(GSIZE*GSIZE);
        }
    }
    imageF[0][0]=0;
    imageF[0][1]=0.0625;
    imageF[0][2]=0.125;
    imageF[0][3]=0.1875;
    imageF[0][4]=0.25;
    imageF[1][4]=0.3125;
    imageF[2][4]=0.375;
    imageF[3][4]=0.4375;
    imageF[4][4]=0.5;
    imageF[4][3]=0.5625;
    imageF[4][2]=0.625;
    imageF[4][1]=0.6875;
    imageF[4][0]=0.75;
    imageF[3][0]=0.8125;
    imageF[2][0]=0.875;
    imageF[2][1]=0.9375;
    imageF[2][2]=1;

    /*int x=0.5+ballLocation.x;
    int y=0.5+ballLocation.y;
    image[x][y]=1;
    generatePattern(image,pat[0]);
    times[0]=5000;*/
    /*float x=ballLocation.x;
    float y=ballLocation.y;
    int x1=x;
    int y1=y;
    int x2=x+1-EPS;
    int y2=y+1-EPS;
    float bx1,bx2,by1,by2;
    bx1=x2-x;
    bx2=x-x1;
    by1=y2-y;
    by2=y-y1;
    float b11,b12,b21,b22;
    b11=bx1*by1;
    b12=bx1*by2;
    b21=bx2*by1;
    b22=bx2*by2;*/
    /*imageF[x1][y1]=b11;
    imageF[x1][y2]=b12;
    imageF[x2][y1]=b21;
    imageF[x2][y2]=b22;*/
    generateFloatPatterns(imageF,pat,times,13);
}

static void graphicsUpdate(void) {
    genImageAndPats();
    displayNI(pat,times,13,FPSTEP,input,&signals);
}

static void processInput(void) {

}

static void moveBall(void) {
    movePoint(&ballLocation,&ballVelocity);
}

int playPongGame(void) {
    resetGame();
    while (1) {
        graphicsUpdate();
        processInput();
        moveBall();
    }
    genImageAndPats();
    displayN(pat,times,4,40);
    return score;
}
