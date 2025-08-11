#include "board.h"
#include <functional>

void awaitUserInput(gameType gameType);

Board createBoard();

Position getPlayerInput(Board const &board, Player player, RNGPointer RNG) ;

void printToPlayer(Player const &player, std::string const &message);

int getValuesWithinRange(std::string const &prompt, int min, int max);

void printBoard(Board const &board, int perspective = -1);