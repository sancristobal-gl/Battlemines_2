#include "game_logic.h"

namespace gameStages {
	void roundStart(Board const &board);

	int minePlacement(Board &board, RNGPointer RNG);

	int guessing(Board &board, RNGPointer RNG);

	int roundEnd(Board &board);
}
