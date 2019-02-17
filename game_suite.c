#include "game_list.h"
#include "gio.h"
#include "gio_extended.h"
#include "gio_arrays.h"
#include "rng.h"

static unsigned endScreen(int score) {
    for (int i=0;i<=score;++i) {
        generateNumberPattern(i,pat[0]);
        display(pat[0],8);
    }
    signals=0;
    int press=0;
    while (!press) {
        displayI(pat[0],5,input,&signals);
        press=getPress(input,&signals);
    }
    signals=0;
    if (press>1) return 0;
    return 1;
}

static int playGame(int currGame) {
    return (*gameList[currGame].playGame)();
}

static const unsigned* gamePreview(int currGame) {
    return gameList[currGame].gamePreview;
}

static void selectGame(int* currGame) {
    signals=0;
    int press=0;
    while (!press) {
        displayI(gamePreview(*currGame),5,input,&signals);
        press=getPress(input,&signals);
        if (press>1) {
            press-=2;
            ++(*currGame);
            if ((*currGame)==GAMES) (*currGame)=0;
        }
    }
    signals=0;
}

void init(void) {
    initGio();
    srand(0);
    int currGame=0;
    while (1) {
        selectGame(&currGame);
        while (endScreen(playGame(currGame)));
    }
}
