#include <gtest/gtest.h>

#include "Battlemines_2/board.h"

const int gameTypeValue = 2;
const int width = 20;
const int height = 20;
const int mineCount = 3;
const int playerCount = 8;

TEST(Placement, board_is_created_properly) {

	Board board = createBoard(gameTypeValue, width, height, mineCount, playerCount);
	ASSERT_TRUE(board.gameType == static_cast<gameType>(gameTypeValue));
	ASSERT_TRUE(board.width == width);
	ASSERT_TRUE(board.height == height);
	ASSERT_TRUE(board.players.front().mineCount == mineCount);
	ASSERT_TRUE(board.playerCount == playerCount);
}
TEST(Placement, placed_mines_are_in_board) {
	Board board = createBoard(gameTypeValue, width, height, mineCount, playerCount);
	placeMine(board, Mine{2, 2});
	ASSERT_TRUE(board.placedMines.size() == 1);
	ASSERT_TRUE(board.placedMines.front().position.xpos == 2);
	ASSERT_TRUE(board.placedMines.front().position.xpos == 2);
}
TEST(Destruction, players_are_eliminated_when_mineCount_is_0) {
	Board board = createBoard(gameTypeValue, width, height, mineCount, playerCount);
	board.players[1].mineCount = 0;
	board.players[5].mineCount = 0;
	eliminatePlayers(board);
	ASSERT_EQ(board.playerCount, playerCount - 2);
}
TEST(End, game_ends_when_one_player_remains) {
	for (int testWinner = 0; testWinner < playerCount; testWinner++) {
		Board board = createBoard(gameTypeValue, width, height, mineCount, playerCount);
		for (int i = 0; i < board.playerCount; i++) {
			if (i != testWinner) {
				board.players[i].mineCount = 0;
			}
		}
		eliminatePlayers(board);
		ASSERT_EQ(gameEndCondition(board), board.players.front().id);
	}
}
TEST(End, game_ends_in_draw_when_no_player_remains) {
	Board board = createBoard(gameTypeValue, width, height, mineCount, playerCount);
	for (int i = 0; i < board.playerCount; i++) {
		board.players[i].mineCount = 0;
	}
	eliminatePlayers(board);
	ASSERT_EQ(gameEndCondition(board), 0);
}
TEST(End, game_doesnt_end_when_end_conditions_arent_met) {
	Board board = createBoard(gameTypeValue, width, height, mineCount, playerCount);
	eliminatePlayers(board);
	ASSERT_EQ(gameEndCondition(board), -1);
}