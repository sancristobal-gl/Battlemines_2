#include "Battlemines_2/console_interface.h"

const int undefinedPerspective = -1; // perspective represent the player who the board is being printed for
// a player should be able to see the position of their own hidden mines but not of the other players

const int charsPerLabel = 3;

void awaitUserInput(gameType gameType) {
	std::cin.clear();
	// std::cin.ignore(std::numeric_limits<std::streamsize>::max()); // causes "enter" to have to be pressed twice before proceeding
	if (gameType != EVE) {
		std::cout << "Press enter to continue\n";
		std::cin.get();
	}
	system("cls");
	return;
}

Board createBoard() {
	gameType gameType = PVP;
	int width = 5;
	int height = 5;
	int mineCount = 3;
	int playerCount = 2;
	gameType = static_cast<::gameType>(getValuesWithinRange("choose game mode (0=PVP, 1=PVE, 2=EVE)", 0, 2)); // not sure if this is good practice
	width = getValuesWithinRange("choose the width of the field", 5, 20);
	height = getValuesWithinRange("choose the height of the field", 5, 20);
	mineCount = getValuesWithinRange("choose the number of mines on the field", 3, 8);
	playerCount = getValuesWithinRange("choose the number of players", 2, 8);
	return createBoard(gameType, width, height, mineCount, playerCount);
}

namespace boardConsoleDisplayHelper {
	void showPositionStatus(Board const &board, unsigned int x, unsigned int y, int perspective = undefinedPerspective);

	void printRow(Board const &board, unsigned int y);

	void printColumnInRow(Board const &board, unsigned int x, unsigned int y, int perspective);
}

Position getPlayerInputPosition(Board const &board, Player player, RNGPointer RNG) {
	Position pos;
	if (player.isAI == false) {
		std::cout << "x: ";
		std::cin >> pos.xpos;
		std::cout << "y: ";
		std::cin >> pos.ypos;
	} else {
		pos = getRandomValidPosition(board, player, RNG);
		std::cout << '\n';
	}
	return pos;
}

void printToPlayer(Player const &player, std::string const &message) { // shows message on console, unless the player is AI, to avoid spammig the console and potentially crashing the program
	if (!player.isAI) {
		std::cout << message << '\n';
	}
}

void boardConsoleDisplayHelper::showPositionStatus(Board const &board, unsigned int x, unsigned int y, int perspective) {
	// print the status of the position {x, y}
	//(" " = not valid, "O" = valid position with unknown contents, "M" = player mine in position)
	bool isPositionEnabled = true;
	Position pos = {x, y};
	for (Position const &disabledPos: board.disabledPositions) { // find if position is disabled
		if (disabledPos == pos) {
			isPositionEnabled = false;
			break;
		}
	}
	bool mineInPos = false;
	if (perspective != undefinedPerspective) { // show ony the mines belonging to player
		for (Mine const &placedMine: board.placedMines) {
			if (placedMine.position.xpos == x && placedMine.position.ypos == y) {
				if (placedMine.owner == perspective) { // id is player position in array + 1
					mineInPos = true;
					break;
				}
			}
		}
	}
	if (mineInPos == true)
		std::cout << "  M";
	else if (isPositionEnabled == false)
		std::cout << "   ";
	else if (isPositionEnabled == true)
		std::cout << "  O";
}

void boardConsoleDisplayHelper::printRow(Board const &board, unsigned int y) {
	int spaceLength = (charsPerLabel - std::to_string(y).length());
	if (y == 0) { // if is first row, we print empty space
		std::cout << "   ";
	} else { // else, print row label
		std::cout << y;
		for (int space = 0; space < spaceLength; space++) {
			std::cout << ' ';
		}
	}
}

void boardConsoleDisplayHelper::printColumnInRow(Board const &board, unsigned int x, unsigned int y, int perspective) {
	int spaceLength = (charsPerLabel - std::to_string(x).length());
	if (y == 0) { // if it's first column, we print column label
		for (int space = 0; space < spaceLength; space++) {
			std::cout << ' ';
		};
		std::cout << std::to_string(x);
	} else { // else, print position status
		boardConsoleDisplayHelper::showPositionStatus(board, x, y, perspective);
	}
	return;
}

// print board to console
// may be depreciated if an UI is implemented in the future
void printBoard(Board const &board, int perspective) {
	for (int y = 0; y <= board.height; y++) { // for each row
		boardConsoleDisplayHelper::printRow(board, y);
		for (int x = 1; x <= board.width; x++) { // next to the row label, we print all positions in row
			boardConsoleDisplayHelper::printColumnInRow(board, x, y, perspective);
		}
		std::cout << '\n';
	}
}

int getValuesWithinRange(std::string const &prompt, int min, int max) { // function to input an int value within range, used for board initialization, probably could be reporopused for other uses
	int ret = min - 1;
	while ((ret < min) || (ret > max)) {
		std::cout << prompt << '\n';
		std::cout << "value must be between " << min << " and " << max << '\n';
		std::cin >> ret;
	}
	return ret;
}