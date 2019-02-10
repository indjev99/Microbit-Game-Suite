#ifndef GAME_LIST_H_INCLUDED
#define GAME_LIST_H_INCLUDED

#include "snake_game.h"
#include "pong_game.h"
#include "dodge_game.h"

#define GAMES 3

struct game {
    int (*playGame)(void);
    const unsigned* gamePreview;
};
extern const struct game gameList[GAMES];

#endif // GAME_LIST_H_INCLUDED
