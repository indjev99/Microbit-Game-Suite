#include "game_list.h"
#include "gio.h"
#include "gio_arrays.h"
#include "rng.h"

unsigned endScreen(int score) {
    for (int i=0;i<=score;++i) {
        generateNumberPattern(i,pat1);
        display(pat1,8);
    }
    signals=0;
    int press=0;
    while (!press) {
        displayI(pat1,5,input,&signals);
        press=getPress(input,&signals);
    }
    signals=0;
    if (press>1) return 0;
    return 1;
}

int playGame(int currGame) {
    return (*gameList[currGame].playGame)();
}

const unsigned* gamePreview(int currGame) {
    return gameList[currGame].gamePreview;
}

void selectGame(int* currGame) {
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