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
            image[i][j]=0;
        }
    }
    /*int x=0.5+ballLocation.x;
    int y=0.5+ballLocation.y;
    image[x][y]=1;
    generatePattern(image,pat[0]);
    times[0]=5000;*/
    int x1=ballLocation.x;
    int y1=ballLocation.y;
    int x2=ballLocation.x+1-EPS;
    int y2=ballLocation.y+1-EPS;
    float bx1,bx2,by1,by2;
    bx1=x2-ballLocation.x;
    bx2=ballLocation.x-x1;
    by1=y2-ballLocation.y;
    by2=ballLocation.y-y1;
    float b11,b12,b21,b22;
    b11=bx1*by1;
    b11=b11*b11*b11*b11;
    b12=bx1*by2;
    b12=b12*b12*b12*b12;
    b21=bx2*by1;
    b21=b21*b21*b21*b21;
    b22=bx2*by2;
    b22=b22*b22*b22*b22;
    float norm=4;
    float nn=b11+b12+b21+b22;
    //float norm=1/(b11+b12+b21+b22);
    b11*=norm;
    b12*=norm;
    b21*=norm;
    b22*=norm;
    times[0]=5000*b11;
    times[1]=5000*b12;
    times[2]=5000*b21;
    times[3]=5000*b22;
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
    displayNI(pat,times,4,FPSTEP,input,&signals);
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
