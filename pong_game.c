#include "pong_game.h"
#include "gio_extended.h"
#include "gio_arrays.h"
#include "rng.h"

const unsigned pongGamePreview[ROWS] = {
    0x3ff0, 0x5bf0, 0x8fe0
};

struct point {
    int x, y;
};

static void movePoint(struct point* location, struct point* velocity) {
    location->x+=velocity->x;
    location->y+=velocity->y;
    while (location->x<0 || location->x>=(GSIZE-1)*256) {
        if (location->x<0) location->x=-location->x;
        else location->x=(GSIZE-1)*512-location->x-1;
        velocity->x=-velocity->x;
    }
    while (location->y<0 || location->y>=(GSIZE-1)*256) {
        if (location->y<0) location->y=-location->y;
        else location->y=(GSIZE-1)*512-location->y-1;
        velocity->y=-velocity->y;
    }
}

static struct point ballVelocity;
static struct point ballLocation;
static int score;
static int FPSTEP=1;

static void resetGame(void) {
    ballVelocity.x=0;
    ballVelocity.y=0;
    ballLocation.x=0;//rand()%(GSIZE*256);
    ballLocation.y=3*256;//rand()%(GSIZE*256);
    score=0;
}
static void genImageAndPats(void) {
    for (int i=0;i<GSIZE;++i) {
        for (int j=0;j<GSIZE;++j) {
            imageA[i][j]=0;//(i*GSIZE+j)*256/(GSIZE*GSIZE);
        }
    }

    imageA[ballLocation.x/256][ballLocation.y/256]=ballLocation.x-ballLocation.x/256*256;
    times[0]=5000;

    /*int x=ballLocation.x;
    int y=ballLocation.y;
    int x1=x>>8;
    int y1=y>>8;
    int bx1,bx2,by1,by2;
    bx2=x-(x1<<8);
    bx1=255-bx2;
    by2=y-(y1<<8);
    by1=255-by2;
    int b11,b12,b21,b22;
    b11=(128+bx1*by1)>>8;
    b12=(128+bx1*by2)>>8;
    b21=(128+bx2*by1)>>8;
    b22=(128+bx2*by2)>>8;
    imageA[x1][y1]=bx1;
    imageA[x1][y1+1]=bx1;
    imageA[x1+1][y1]=bx2;
    imageA[x1+1][y1+1]=bx2;*/
    generateAnaloguePatterns(imageA,pat,times,13);
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
        //moveBall();
        ballLocation.x+=1;
    }
    genImageAndPats();
    displayN(pat,times,4,40);
    return score;
}
