#include "gio.h"
#include "rng.h"

struct point {
    int x, y;
    unsigned f;
};

static unsigned image[GSIZE][GSIZE];
static unsigned patOn[ROWS];
static unsigned patOff[ROWS];
static int signals=0;
static unsigned input[100];

static struct point snake[GSIZE*GSIZE+1];
static unsigned isFree[GSIZE][GSIZE];
static struct point food;
static int snakeLen;
static int snakeDir;
static unsigned snakeAlive;
static int score;
static int FPSTEP=9;
static unsigned playAgain;

void genFood(void) {
    if (snakeLen==GSIZE*GSIZE) return;
    int pos=rand()%(GSIZE*GSIZE-snakeLen);
    int curr=-1;
    for (int i=0;i<GSIZE && curr<pos;++i) {
        for (int j=0;j<GSIZE && curr<pos;++j) {
            if (isFree[i][j])
            {
                ++curr;
                if (curr==pos) {
                    food.x=i;
                    food.y=j;
                    return;
                }
            }
        }
    }
}

void selectDifficulty() {
    int curr=0;
    while (!curr) {
        generateNumberPattern(FPSTEP,patOn);
        displayI(patOn,5,input,&signals);
        curr=getPress(input,&signals);
        if (curr>1) {
            ++FPSTEP;
            if (FPSTEP > 20) FPSTEP=3;
            curr-=2;
        }
    }
    signals=0;
}

void resetGame(void) {
    for (int i=0;i<GSIZE;++i) {
        for (int j=0;j<GSIZE;++j) {
            isFree[i][j]=1;
        }
    }
    snakeLen=3;
    snake[0].x=1;
    snake[0].y=2;
    snake[0].f=0;
    isFree[1][2]=0;
    snake[1].x=2;
    snake[1].y=2;
    snake[1].f=0;
    isFree[2][2]=0;
    snake[2].x=3;
    snake[2].y=2;
    snake[2].f=0;
    isFree[3][2]=0;
    snakeAlive=1;
    snakeDir=0;
    score=3;
    genFood();
    selectDifficulty();
}

void graphicsUpdate() {
    for (int i=0;i<GSIZE;++i) {
        for (int j=0;j<GSIZE;++j) {
            image[i][j]=0;
        }
    }
    for (int i=0;i<snakeLen;++i)
    {
        image[snake[i].x][snake[i].y]=1;
    }
    generatePattern(image,patOff);
    image[food.x][food.y]=1;
    generatePattern(image,patOn);
    displayI(patOn,FPSTEP/3,input,&signals);
    displayI(patOff,2*FPSTEP/3,input,&signals);
    displayI(patOn,FPSTEP,input,&signals);
}

void processInput(void) {
    int curr=3;
    while (signals && curr==3) {
        curr=getPress(input,&signals);
    }
    if (curr==2) ++snakeDir;
    if (curr==1) --snakeDir;
    if (snakeDir<0) snakeDir=3;
    if (snakeDir==4) snakeDir=0;
}

void endScreen(void) {
    graphicsUpdate();
    display(patOn,40);
    int curr=0;
    while (curr<score) {
        generateNumberPattern(curr,patOn);
        display(patOn,8);
        ++curr;
    }
    signals=0;
    curr=0;
    while (!curr) {
        displayI(patOn,5,input,&signals);
        curr=getPress(input,&signals);
    }
    if (curr>1) playAgain=0;
    signals=0;
}

void moveSnake(void) {
    struct point head=snake[0];
    switch (snakeDir) {
        case 0: {
            --head.x;
            break;
        }
        case 1: {
            ++head.y;
            break;
        }
        case 2: {
            ++head.x;
            break;
        }
        case 3: {
            --head.y;
            break;
        }
    }
    head.f=0;
    int prevLen=snakeLen;
    if (head.x<0) head.x=GSIZE-1;
    if (head.x==GSIZE) head.x=0;
    if (head.y<0) head.y=GSIZE-1;
    if (head.y==GSIZE) head.y=0;
    if (snake[snakeLen-1].f) {
        snake[snakeLen-1].f=0;
        ++snakeLen;
    }
    else isFree[snake[snakeLen-1].x][snake[snakeLen-1].y]=1;
    for (int i=snakeLen-1;i>0;--i) {
        snake[i]=snake[i-1];
    }
    snake[0]=head;
    if (head.x==food.x && head.y==food.y) {
        snake[0].f=1;
        isFree[head.x][head.y]=0;
        genFood();
        ++score;
    }
    else if (!isFree[head.x][head.y]) {
        snakeAlive=0;
        snakeLen=prevLen+1;
    }
    else isFree[head.x][head.y]=0;
}

void init(void) {
    initGio();
    srand(0);
    playAgain=1;
    while (playAgain) {
        resetGame();
        while (snakeAlive) {
            graphicsUpdate();
            processInput();
            moveSnake();
        }
        endScreen();
    }
}
