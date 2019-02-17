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
static double p[4];

static void resetGame(void) {
    ballVelocity.x=0.1;
    ballVelocity.y=0.25;
    ballLocation.x=1.5;//rand()%(GSIZE*10)/10.0;
    ballLocation.y=0;//rand()%(GSIZE*10)/10.0;
    score=0;
}
static void genImageAndPats(void) {
    for (int i=0;i<GSIZE;++i) {
        for (int j=0;j<GSIZE;++j) {
            image[i][j]=0;
        }
    }
    /*int x=0.5+ballLocation.x;
    int y=0.5+ballLocation.y;
    image[x][y]=1;
    generatePattern(image,pat1);*/
    double x=ballLocation.x;
    double y=ballLocation.y;
    int x1=x;
    int y1=y;
    int x2=x+1-EPS;
    int y2=y+1-EPS;
    double bx1,bx2,by1,by2;
    bx1=x2-x;
    bx2=x-x1;
    by1=y2-y;
    by2=y-y1;
    p[0]=bx1*by1;
    p[1]=bx1*by2;
    p[2]=bx2*by1;
    p[3]=bx2*by2;
    p[0]*=p[0];
    p[1]*=p[1];
    p[2]*=p[2];
    p[3]*=p[3];
    image[x1][y1]=1;
    generatePattern(image,pat[0]);
    image[x1][y1]=0;
    image[x1][y2]=1;
    generatePattern(image,pat[1]);
    image[x1][y2]=0;
    image[x2][y1]=1;
    generatePattern(image,pat[2]);
    image[x2][y1]=0;
    image[x2][y2]=1;
    generatePattern(image,pat[3]);
    image[x2][y2]=0;
}

static void graphicsUpdate(void) {
    genImageAndPats();
    displayNI(pat,p,4,FPSTEP,input,&signals);
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
    displayN(pat,p,4,40);
    return score;
}
