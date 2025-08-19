#include "Battlemines_2/board.h"

bool Position::operator==(const Position &b) const {
	return ((xpos == b.xpos) && (ypos == b.ypos));
}

bool Mine::operator==(const Mine &b) const {
	return ((position.xpos == b.position.xpos) && (position.ypos == b.position.ypos)) && (owner == b.owner);
}

bool Mine::operator==(const Position &b) const {
	return ((position.xpos == b.xpos) && (position.ypos == b.ypos));
}

bool isValueWithinRange(int value, int min, int max) {
	return (value >= min || value <= max);
}

bool isInputValid(int gameTypeValue, unsigned int width, unsigned int height, int mineCount, int playerCount) {
	if (! isValueWithinRange(static_cast<int>(gameTypeValue), 0, gameType::SIZE_OF_ENUM - 1)) return false;
	if (!isValueWithinRange(width, MINWIDTH, MAXWIDTH)) return false;
	if (!isValueWithinRange(height, MINHEIGHT, MAXHEIGHT)) return false;
	if (!isValueWithinRange(mineCount, MINMINECOUNT, MAXMINECOUNT)) return false;
	if (!isValueWithinRange(playerCount, MINPLAYERCOUNT, MAXPLAYERCOUNT)) return false;
	return true;
}

Board createBoard(gameType gameTypeValue, unsigned int width, unsigned int height, int mineCount, int playerCount) { // overloaded instead of merging into one function because once requires player input and the other doesn't
	if (!isInputValid(gameTypeValue, width, height, mineCount, playerCount)) {
		throw std::exception("Board parameters are invalid");
	}
	Board board;
	board.gameType = static_cast<gameType>(gameTypeValue);
	board.width = width;
	board.height = height;
	board.playerCount = playerCount;
	for (int p = 0; p < board.playerCount; p++) {
		Player player;
		player.mineCount = mineCount;
		player.id = p + 1;
		player.isAI = ((board.gameType == PVE) && (p > 0)) || (board.gameType == EVE);
		board.players.push_back(player);
	}
	board.validTiles = generateValidTilesArray(board);
	return board;
}

Board::~Board() {
	// used to have memory deallocation
	// preserving just in case
}

int getRandomValueInRange(int max, int min) {
	return (rand() % (max - min) + min);
}

Position getRandomValidPosition(Board const &board, Player const &player, RNGPointer RNG) { // helper function for bot players
	std::vector<Position> validTiles = board.validTiles;
	std::vector<Mine> playerMines = getPlayerMines(board, player);
	for (auto it = validTiles.begin(); it != validTiles.end();) { // cursed for-loop, somewhat inefficient
		bool erase = false;
		for (Mine mine: playerMines) {
			if (mine.position == *it) {
				erase = true;
				break;
			}
		}
		if (erase) {
			it = validTiles.erase(it);
		} else {
			it++;
		}
	}
	return validTiles[RNG(validTiles.size(), 0)];
}

void placeMine(Board &board, Mine mine) {
	board.placedMines.push_back(mine);
}

bool isPositionValid(Board const &board, Position const &pos) { // check if pos if withing acceptable values
	// If the position is outside the board, return false;
	if ((pos.xpos < 1) || (pos.ypos < 1) || (pos.xpos > board.width) || (pos.ypos > board.height)) {
		return false;
	}
	// if the position is disabled, return false
	for (Position const &disabledPos: board.disabledPositions) {
		if (pos == disabledPos) {
			return false;
		}
	}
	// else, return true
	return true;
}

int getPlayerPositionInArray(Board const &board, int playerID) { // defined in case the player.ID format changes in the future
	for (int i = 0; i < board.players.size(); i++) {
		if (board.players[i].id == playerID) {
			return i;
		}
	}
	return -1;
}
// gets all possible valid positions
// mainly for bot logic
std::vector<Position> generateValidTilesArray(Board const &board) {
	std::vector<Position> validPositions;
	for (unsigned int x = 1; x <= board.width; x++) {
		for (unsigned int y = 1; y <= board.height; y++) {
			Position pos{x, y};
			if (isPositionValid(board, pos)) {
				validPositions.push_back(std::move(pos));
			}
		}
	}
	return validPositions;
}

void removeFromValidTiles(Board &board, Position pos) {
	std::vector<Position>::iterator positionToRemove = board.validTiles.end();
	for (auto it = board.validTiles.begin(); it != board.validTiles.end(); it++) {
		if (*it == pos) {
			positionToRemove = it;
			break;
		}
	}
	if (positionToRemove != board.validTiles.end()) {
		board.validTiles.erase(positionToRemove);
	}
}

std::vector<Mine> getPlayerMines(Board const &board, Player const &player) {
	std::vector<Mine> ret;
	for (Mine mine: board.placedMines) {
		if (mine.owner == player.id) {
			ret.push_back(mine);
		}
	}
	return ret;
}

void disablePosition(Board &board, Position const &disabledPosition) {
	board.disabledPositions.insert(disabledPosition);
	removeFromValidTiles(board, disabledPosition);
}

void disablePosition(Board &board, Mine const &disabledMine) {
	disablePosition(board, disabledMine.position);
}

bool removeMine(Board &board, Mine mine) {
	for (auto it = board.placedMines.begin(); it != board.placedMines.end(); it++) {
		if (*it == mine) {
			board.placedMines.erase(it);
			int mineOwnerIndex = getPlayerPositionInArray(board, mine.owner);
			if (mineOwnerIndex >= 0) {
				board.players[getPlayerPositionInArray(board, mine.owner)].mineCount--;
			}
			disablePosition(board, mine);
			return true;
		}
	}
	return false; // just in case I need to check if a mine was properly erased or not
}

// function to be called at the end of each turn
void disableTilesUsed(Board &board) {
	for (Mine const &placedMine: board.placedMines) {
		disablePosition(board, placedMine);
	}
}

void eliminatePlayers(Board &board) {
	int newPlayerCount = board.playerCount;
	std::vector<Player> newPlayerList;
	for (Player const &player: board.players) {
		Player newPlayer = player;
		if (newPlayer.mineCount <= 0) { // player is removed when they have no mines remaining
			newPlayerCount--;
		} else {
			newPlayerList.push_back(newPlayer);
		}
	}
	std::vector<Player> players(newPlayerCount); // build new player array without the removed players
	for (int i = 0; i < newPlayerCount; i++) {
		players[i] = newPlayerList[i];
	}
	board.players = players;
	board.playerCount = newPlayerCount;
}
// when only one player has mines remaining, they win the game
// if no players have mines, game is a draw
int gameEndCondition(Board &board) {
	static const int cNoWinner = -1;						// this value represents a state where the game is still going, as there's no winner yet
	static const int cDraw = 0;								// this value represents a state where the game has ended with no player being a winner
	if (board.playerCount == 1) return board.players[0].id; // player wins if they're the only one remaining
	if (board.playerCount == 0) return cDraw;				// if no players remain, game is a draw
	// check if there are enough tiles to accomodate every player's mines, if not, the game is a draw
	unsigned int maxPlayerMines = 0;
	for (Player const &player: board.players) {
		if (player.mineCount > maxPlayerMines) {
			maxPlayerMines = player.mineCount;
		}
	}
	unsigned int tilesRemaining = board.validTiles.size();
	std::cout << "tiles remaining: " << tilesRemaining << '\n';
	if ((tilesRemaining < maxPlayerMines) || (board.validTiles.size() < board.playerCount)) { // if not enough tiles remain for another turn, the player with the most mines remaining wins
		Player playerWithMaxMines;
		int winner = 0;
		for (Player player: board.players) {
			if (player.mineCount > playerWithMaxMines.mineCount) {
				playerWithMaxMines = player;
				winner = player.id;
			} else if (player.mineCount == playerWithMaxMines.mineCount) { // if two or more players share the highest mine count, game's a draw
				winner = 0;
			}
		}
		std::cout << "Not enough tiles remain to continue playing, the player with the greatest amount of mines remaining is the winner\n";
		std::cout << "(Unless two or more players share the highest mine count, in which case it's a draw)\n";
		return winner;
	}

	return cNoWinner; // else, the game is not over
}