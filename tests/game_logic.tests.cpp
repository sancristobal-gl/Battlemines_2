#include <gtest/gtest.h>

#include "Battlemines_2/game_logic.h"

int RNGTestFunc(int min, int max) {
	return max;
}

Position getPlayerInputPositionTestFunc(Board const &board, Player player, RNGPointer RNG) {
	Position pos = Position{2, 2};
	return pos;
}
TEST(Colission, checkMineCollision_detects_collission) {
	Board board = createBoard(2, 10, 10, 3, 2);
	placeMine(board, Mine{2, 2});
	placeMine(board, Mine{2, 2});
	ASSERT_TRUE(checkMineCollision(board));
}

TEST(Detection, guess_detects_mine) {
	Board board = createBoard(2, 10, 10, 3, 2);
	placeMine(board, Mine{2, 2});
	Player player;
	RNGPointer RNG = RNGTestFunc;
	userInputPointer getPlayerInputTest = getPlayerInputPositionTestFunc;
	ASSERT_TRUE(guess(board, player, RNG, getPlayerInputTest));
}

TEST(Detection, guess_doesnt_detect_mine_in_empty_space) {
	Board board = createBoard(2, 10, 10, 3, 2);
	placeMine(board, Mine{1, 1});
	Player player;
	RNGPointer RNG = RNGTestFunc;
	userInputPointer getPlayerInputTest = getPlayerInputPositionTestFunc;
	ASSERT_FALSE(guess(board, player, RNG, getPlayerInputTest));
}