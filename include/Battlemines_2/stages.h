#include "game_logic.h"

namespace gameStages {
	void roundStart(Board const &board);

	int minePlacement(Board &board, RNGPointer RNG, userInputFunctionPointer getPlayerInput);

	int guessing(Board &board, RNGPointer RNG, userInputFunctionPointer getPlayerInput);

	int roundEnd(Board &board);
}
