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
static unsigned manAlive;
static struct point blocks[GSIZE*GSIZE/2];
static int blockNum;
static int freeBlocks[GSIZE*GSIZE/2];
static int freeBlockNum;
static unsigned isFree[GSIZE+1][GSIZE];
static unsigned isReachable[GSIZE+1][GSIZE];
static int score;
static int difficulty=1;
static int FPSTEP=1;
static int blockDelay;
static int flicker;
static int timeStamp;

static void DFS(int x, int y) {
    isReachable[x][y]=timeStamp;
    if (x>1 && y>0 && isFree[x][y-1] && isReachable[x][y-1]!=timeStamp) DFS(x,y-1);
    if (x>1 && y<GSIZE-1 && isFree[x][y+1] && isReachable[x][y+1]!=timeStamp) DFS(x,y+1);
    if (x>0 && isFree[x-1][y] && isReachable[x-1][y]!=timeStamp) DFS(x-1,y);
}
static void genBlock(int index) {
    ++timeStamp;
    DFS(man.x+1,man.y);
    int poses[GSIZE];
    int posNum=0;
    for (int i=0;i<GSIZE;++i) {
        if (!isFree[0][i] || isReachable[0][i]!=timeStamp) continue;
        if ((isReachable[0][i]!=timeStamp) ||
            (i>0 && isReachable[0][i-1]==timeStamp) ||
            (i<GSIZE-1 && isReachable[0][i+1]==timeStamp)) {
            poses[posNum]=i;
            ++posNum;
            continue;
        }
    }
    if (!posNum) {
        if (index<blockNum) {
            freeBlocks[freeBlockNum]=index;
            ++freeBlockNum;
        }
        return;
    }
    int pos=poses[rand()%posNum];
    if (index==blockNum) {
        if (freeBlockNum) {
            --freeBlockNum;
            index=freeBlocks[freeBlockNum];
        }
        else ++blockNum;
    }
    blocks[index].x=-1;
    blocks[index].y=pos;
    isFree[blocks[index].x+1][blocks[index].y]=0;
}

static void selectDifficulty() {
    selectNumber(1,10,&difficulty);
    blockDelay=3*(14-difficulty)/2;
}

static void resetGame(void) {
    for (int i=0;i<=GSIZE;++i) {
        for (int j=0;j<GSIZE;++j) {
            isFree[i][j]=1;
        }

    }
    man.x=4;
    man.y=2;
    manAlive=1;
    score=0;
    blockNum=0;
    freeBlockNum=0;
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
    display2I(pat1,pat2,0.033,FPSTEP,input,&signals);
}

static void processInput(void) {
    int curr=getSinglePress(input,&signals);
    while (curr) {
        if (curr==2 && man.y+1<GSIZE && isFree[man.x+1][man.y+1]) ++man.y;
        if (curr==1 && man.y-1>=0 && isFree[man.x+1][man.y-1]) --man.y;
        curr=getSinglePress(input,&signals);
    }
}

static void moveBlocks(void) {
    for (int i=0;i<blockNum;++i) {
        ++blocks[i].x;
        if (blocks[i].x-1>=0 && blocks[i].x-1<=GSIZE) isFree[blocks[i].x-1][blocks[i].y]=1;
        if (man.x==blocks[i].x && man.y==blocks[i].y) manAlive=0;
    }
    for (int i=0;i<blockNum;++i) {
        if (blocks[i].x+1<=GSIZE && blocks[i].x+1>=0) isFree[blocks[i].x+1][blocks[i].y]=0;
        if (blocks[i].x<=GSIZE && blocks[i].x>=0) isFree[blocks[i].x][blocks[i].y]=0;
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
            if (cnt2==(GSIZE+2)+2) {
                cnt2=0;
                genBlock(blockNum);
            }
        }
    }
    genImageAndPats();
    display(pat1,40);
    return score/5.0;
}
