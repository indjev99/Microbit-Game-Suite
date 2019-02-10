#ifndef GAME_LIST_H_INCLUDED
#define GAME_LIST_H_INCLUDED

#include "snake.h"

#define GAMES 1

struct game {
    int (*playGame)(void);
    const unsigned* gamePreview;
} gameList[GAMES] = {
    {playSnakeGame, snakeGamePreview}
};

#endif // GAME_LIST_H_INCLUDED
