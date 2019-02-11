#include "dodge_game.h"
#include "gio_extended.h"
#include "gio_arrays.h"
#include "rng.h"

const unsigned dodgeGamePreview[ROWS] = {
    0x3ff0, 0x5f70, 0x8ae0
};

struct point {
    int x, y;
};

static struct point man;
static struct point blocks[GSIZE*GSIZE];
static unsigned isFree[GSIZE];
static int blockNum;
static unsigned manAlive;
static int score;
static int difficulty=1;
static int FPSTEP=1;
static int blockDelay;
static int flicker;

static void genBlock(int index) {
    int pos=rand()%GSIZE;
    blocks[index].x=-1;
    blocks[index].y=pos;
    if (index==blockNum) ++blockNum;
}

static void selectDifficulty() {
    selectNumber(1,15,&difficulty);
    blockDelay=5*(18-difficulty)/3;
}

static void resetGame(void) {
    for (int i=0;i<GSIZE;++i) {
        isFree[i]=1;
    }
    man.x=4;
    man.y=2;
    manAlive=1;
    score=0;
    blockNum=0;
    genBlock(blockNum);
    selectDifficulty();
    flicker=4;
}

static void genImageAndPats() {
    for (int i=0;i<GSIZE;++i) {
        for (int j=0;j<GSIZE;++j) {
            image[i][j]=0;
        }
    }
    for (int i=0;i<blockNum;++i)
    {
        if (blocks[i].x>=0 && blocks[i].x<GSIZE) image[blocks[i].x][blocks[i].y]=1;
        if (blocks[i].x>0 && blocks[i].x<=GSIZE) image[blocks[i].x-1][blocks[i].y]=1;
    }
    generatePattern(image,pat2);
    image[man.x][man.y]=1;
    generatePattern(image,pat1);
}

static void graphicsUpdate() {
    genImageAndPats();
    //displayI(pat1,FPSTEP,input,&signals);
    display2I(pat1,pat2,0.025,FPSTEP,input,&signals);
}

static void processInput(void) {
    int curr=getSinglePress(input,&signals);
    while (curr) {
        if (curr==2 && man.y+1<GSIZE && isFree[man.y+1]) ++man.y;
        if (curr==1 && man.y-1>=0 && isFree[man.y-1]) --man.y;
        curr=getSinglePress(input,&signals);
    }
}

static void moveBlocks(void) {
    for (int i=0;i<GSIZE;++i) {
        isFree[i]=1;
    }
    for (int i=0;i<blockNum;++i) {
        ++blocks[i].x;
        if (man.x==blocks[i].x && man.y==blocks[i].y) manAlive=0;
        if (blocks[i].x==GSIZE-1 || blocks[i].x==GSIZE) isFree[blocks[i].y]=0;
        if (blocks[i].x==GSIZE+1) {
            ++score;
            genBlock(i);
        }
    }
}

int playDodgeGame(void) {
    resetGame();
    int cnt=0;
    int cnt2=0;
    while (manAlive) {
        graphicsUpdate();
        processInput();
        ++cnt;
        if (cnt==blockDelay) {
            cnt=0;
            moveBlocks();
            ++cnt2;
            if (cnt2==(GSIZE+2)*3+2) {
                cnt2=0;
                genBlock(blockNum);
            }
        }
    }
    genImageAndPats();
    display(pat1,40);
    return (score+2)/3;
}
