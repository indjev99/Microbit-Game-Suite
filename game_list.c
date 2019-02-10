#include "game_list.h"
#include "snake_game.h"
#include "pong_game.h"
#include "dodge_game.h"

const struct game gameList[GAMES] = {
    {playSnakeGame, snakeGamePreview},
    {playPongGame, pongGamePreview},
    {playDodgeGame, dodgeGamePreview}
};
