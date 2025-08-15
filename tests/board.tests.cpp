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

TEST(OperatorTests, position_EQ_position) {
	using XType = decltype(Position{}.xpos);
	using YType = decltype(Position{}.ypos);

	Position p1{}, p2{};

	// Equal cases
	p1 = Position{0, 0};
	p2 = Position{0, 0};
	EXPECT_EQ(p1, p2);

	p1 = Position{1, 0};
	p2 = Position{1, 0};
	EXPECT_EQ(p1, p2);

	p1 = Position{0, 1};
	p2 = Position{0, 1};
	EXPECT_EQ(p1, p2);

	p1 = Position{1, 1};
	p2 = Position{1, 1};
	EXPECT_EQ(p1, p2);

	p1 = Position{std::numeric_limits<XType>::max(), std::numeric_limits<YType>::max()};
	p2 = Position{std::numeric_limits<XType>::max(), std::numeric_limits<YType>::max()};
	EXPECT_EQ(p1, p2);

	p1 = Position{std::numeric_limits<XType>::min(), std::numeric_limits<YType>::min()};
	p2 = Position{std::numeric_limits<XType>::min(), std::numeric_limits<YType>::min()};
	EXPECT_EQ(p1, p2);

	// Not equal cases — use EXPECT_NE
	p1 = Position{1, 0};
	p2 = Position{0, 1};
	EXPECT_NE(p1, p2);

	p1 = Position{0, 1};
	p2 = Position{1, 0};
	EXPECT_NE(p1, p2);

	p1 = Position{1, 1};
	p2 = Position{0, 1};
	EXPECT_NE(p1, p2);

	p1 = Position{std::numeric_limits<XType>::min(), std::numeric_limits<YType>::max()};
	p2 = Position{std::numeric_limits<XType>::max(), std::numeric_limits<YType>::min()};
	EXPECT_NE(p1, p2);
}

TEST(OperatorTests, mine_EQ_mine) {
	using XType = decltype(Position{}.xpos);
	using YType = decltype(Position{}.ypos);
	Mine m1{};
	Mine m2{};

	// Equal mines
	m1 = Mine{{0, 0}, 1};
	m2 = Mine{{0, 0}, 1};
	EXPECT_EQ(m1, m2);

	m1 = Mine{{std::numeric_limits<XType>::max(), std::numeric_limits<XType>::max()}, 5};
	m2 = Mine{{std::numeric_limits<XType>::max(), std::numeric_limits<XType>::max()}, 5};
	EXPECT_EQ(m1, m2);

	// Different position, same owner
	m1 = Mine{{1, 0}, 1};
	m2 = Mine{{0, 0}, 1};
	EXPECT_NE(m1, m2);

	// Same Position, different owner
	m1 = Mine{{0, 0}, 1};
	m2 = Mine{{0, 0}, 2};
	EXPECT_NE(m1, m2);
}

TEST(OperatorTests, Mine_EQ_Position) {
	using XType = decltype(Position{}.xpos);
	using YType = decltype(Position{}.ypos);

	Mine m{};
	Position p{};

	// Equal position
	m = Mine{Position{10, 20}, 1};
	p = Position{10, 20};
	EXPECT_EQ(m, p);

	// Max values
	m = Mine{Position{std::numeric_limits<XType>::max(), std::numeric_limits<YType>::max()}, 7};
	p = Position{std::numeric_limits<XType>::max(), std::numeric_limits<YType>::max()};
	EXPECT_EQ(m, p);

	// Not equal
	m = Mine{Position{10, 20}, 1};
	p = Position{20, 10};
	EXPECT_NE(m, p);
}

TEST(BoardTest, board_creation) {
	constexpr int gameTypeValue = -2;
	constexpr int width = -20;
	constexpr int height = -20;
	constexpr int mineCount = -3;
	constexpr int playerCount = -8;
	Board board = createBoard(gameTypeValue, width, height, mineCount, playerCount);
	EXPECT_EQ(board.gameType, static_cast<gameType>(gameTypeValue));
	ASSERT_TRUE(board.width == width);
	ASSERT_TRUE(board.height == height);
	ASSERT_TRUE(board.players.front().mineCount == mineCount);
	ASSERT_TRUE(board.playerCount == playerCount);
}