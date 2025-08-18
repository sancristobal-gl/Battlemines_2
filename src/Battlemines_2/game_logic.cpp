#include "Battlemines_2/game_logic.h"

// gets input from console if player is human
// generates random input if player is bot

void chooseMinePositions(Board &board, Player &player, RNGPointer RNG, userInputFunctionPointer getPlayerInput) {
	printToPlayer(player, std::string("Player ") + std::to_string(player.id) + std::string("!, choose your mine's positions"));
	for (int mineId = 0; mineId < player.mineCount; mineId++) {
		if (!player.isAI) {
			printBoard(board, player.id);
		}
		bool validPlacement = false;
		Mine mine;
		while (validPlacement == false) {
			printToPlayer(player, std::string("Choose the position of mine ") + std::to_string(mineId));
			mine.position = getPlayerInput(board, player, RNG);
			mine.owner = player.id;
			validPlacement = isPositionValid(board, mine.position);
			if (validPlacement == false) {
				printToPlayer(player, "Invalid position! Choose again");
			}
			for (Mine const &placedMine: board.placedMines) {
				if (placedMine == mine) { // for them to be equal, they need to share position and owner
					printToPlayer(player, "You already placed a mine there! Choose again");
					validPlacement = false;
				}
			}
		}
		placeMine(board, mine);
	}
	awaitUserInput(board.gameType);
}

bool guess(Board &board, Player &player, RNGPointer RNG, userInputFunctionPointer getPlayerInput) {
	printBoard(board, player.id);
	Position guess;
	bool isGuessValid = false; // flag to check if the inputed position is valid. If not, ask the player again
	while (isGuessValid == false) {
		printToPlayer(player, (std::string("Player ") + std::to_string(player.id) + std::string(", take a guess... ")));
		guess = getPlayerInput(board, player, RNG);
		isGuessValid = isPositionValid(board, guess);
		if (isGuessValid == false) {
			printToPlayer(player, "That spot has already been checked! Try again");
		}
	}
	// check if the guessed position shares the same position as a mine
	bool guessedMine = false; // is true if a mine was found
	std::vector<Mine> minesToRemove;
	for (Mine const &mine: board.placedMines) {
		if (mine == guess) {
			if (mine.owner == player.id) {
				printToPlayer(player, "That's your own mine, silly!"); // el jugador pierde la oportunidad de volver a adivinar
				
			} else {
				removeMine(board, mine);
				printToPlayer(player, "You found an enemy mine!");
			}
			guessedMine = true;
			break;
		}
	}
	disablePosition(board, guess);
	return guessedMine;
}

// function to be called after each player has placed their mines
// colliding mines are annihilated and removed from their owner's mine pool
bool checkMineCollision(Board &board) {
	bool wasThereCollision = false;
	std::vector<Mine> conflictingMines;
	for (unsigned int i = 0; i < board.placedMines.size() - 1; i++) { // size is -- because the vector value in i is compared to the values on its right
		if (board.placedMines.size() <= 0) {
			return wasThereCollision;
		}
		Position mine1Pos = board.placedMines[i].position;
		conflictingMines.push_back(board.placedMines[i]);
		for (unsigned int j = i + 1; j < board.placedMines.size(); j++) {
			Position mine2Pos = board.placedMines[j].position;
			if (mine1Pos.xpos == mine2Pos.xpos && mine1Pos.ypos == mine2Pos.ypos) { // they'll be equal if they share the same positoon, the owner is not a factor
				conflictingMines.push_back(board.placedMines[j]);
			}
		}
		if (conflictingMines.size() > 1) {
			i--;																																																	  // i is -- since the current element is to be deleted, meaning the "next" iteration should check the element that will take i's place
			std::cout << std::string("Colisionaron ") << conflictingMines.size() << std::string(" minas en ") << conflictingMines[0].position.xpos << std::string(", ") << conflictingMines[0].position.ypos << '\n'; // conflictingMines will always have a value at [0]
			for (Mine const &mine: conflictingMines) {
				removeMine(board, mine);
				wasThereCollision = true;
			}
		}
		conflictingMines.clear();
	}
	return wasThereCollision;
}