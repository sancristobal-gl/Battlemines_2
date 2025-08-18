#include "board_console.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <optional>

void chooseMinePositions(Board &board, Player &player, RNGPointer RNG, userInputFunctionPointer getPlayerInput);

bool guess(Board &board, Player &player, RNGPointer RNG, userInputFunctionPointer getPlayerInput);

bool checkMineCollision(Board &board);