#include <bitset> //for debugging
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Position {
	unsigned int xpos = 0;
	unsigned int ypos = 0;

	bool operator==(const Position &pos) const;
};

struct PositionHash { // hashing implementation for Position
	size_t operator()(const Position &pos) const {
		return std::hash<long long>()((static_cast<long long>(pos.xpos) << 32) ^ static_cast<long long>(pos.ypos)); // xpos and ypos are 32bit unsigned integers, we concatenate them into a 64bit long long to create a key
	}
};

struct Mine {
	Position position;
	int owner = -1;
	bool operator==(const Mine &b) const;
	bool operator==(const Position &b) const;
};

struct Player {
	int id = -1;
	int mineCount = 0;
	bool operator==(const Player b) {
		return (id == b.id);
	}
	bool isAI = false;
};

enum gameType {
	PVP,
	PVE,
	EVE
};

struct Board {
	int width = 0;
	int height = 0;
	std::unordered_set<Position, PositionHash> disabledPositions;
	std::vector<Player> players;
	int playerCount = 0;
	std::vector<Mine> placedMines;
	gameType gameType = PVP;
	~Board();
};

typedef int (*RNGPointer)(int, int);

int getRandomValueInRange(int max, int min);

Position getRandomValidPosition(Board const &board, Player const &player, RNGPointer RNG);

bool isPositionValid(Board const &board, Position const &pos);

std::vector<Position> getValidTiles(Board const &board);

std::vector<Mine> getPlayerMines(Board const &board, Player const &player);

void disablePosition(Board &board, Position const &disabledPosition);

bool removeMine(Board &board, Mine mine);

void disableTilesUsed(Board &board);

void eliminatePlayers(Board &board);

int gameEndCondition(Board &board);