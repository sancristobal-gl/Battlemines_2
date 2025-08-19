#include "board.h"
#include <functional>

typedef Position (*userInputFunctionPointer)(Board const &, Player , RNGPointer );

void awaitUserInput(gameType gameType);

Board createBoard();

Position getPlayerInputPosition(Board const &board, Player player, RNGPointer RNG) ;

void printToPlayer(Player const &player, std::string const &message);

int getValuesWithinRange(std::string const &prompt, int min, int max);

void printBoard(Board const &board, int perspective = -1);