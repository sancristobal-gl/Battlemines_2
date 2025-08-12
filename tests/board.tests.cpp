#include <gtest/gtest.h>

#include "Battlemines_2/board.h"

TEST(Placement, board_is_created_properly){
    int gameTypeValue = 2;
    int width = 20;
    int height = 20;
	int mineCount = 3;
	int playerCount = 8;
	Board board = createBoard(gameTypeValue, width, height, mineCount, playerCount);
    ASSERT_TRUE(board.gameType == static_cast<gameType>(gameTypeValue));
    ASSERT_TRUE(board.width == width);
    ASSERT_TRUE(board.height == height);
	ASSERT_TRUE(board.players.front().mineCount == mineCount);
    ASSERT_TRUE(board.playerCount == playerCount);
    }
TEST(Placement, placed_mines_are_in_board) {
    Board board = createBoard(2, 20, 20, 3, 8);
    placeMine(board, Mine{2, 2});
    ASSERT_TRUE(board.placedMines.size() == 1);
    ASSERT_TRUE(board.placedMines.front().position.xpos == 2);
    ASSERT_TRUE(board.placedMines.front().position.xpos == 2);
}

