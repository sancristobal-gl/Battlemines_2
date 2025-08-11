#include "Battlemines_2/stages.h"

void gameStages::roundStart(Board const &board) {
	for (Player const &p: board.players) {
		std::cout << "Player " << p.id << " mine count: " << p.mineCount << std::endl;
	}
	printBoard(board);
	std::cout << "Press enter to commence the round!" << std::endl;
	awaitUserInput(board.gameType);
}

int gameStages::minePlacement(Board &board, RNGPointer RNG) {
	for (Player &p: board.players) {
		std::cout << "Player " << p.id << "'s turn to place their mines:" << std::endl;
		chooseMinePositions(board, p, RNG);
	}
	bool wasThereCollision = checkMineCollision(board); // check if mines collide, if they do, notify the player
	if (wasThereCollision == true) {
		awaitUserInput(board.gameType);
	}
	eliminatePlayers(board);
	return gameEndCondition(board); // every end step, check if a winner has been decided
}

unsigned int getGuessAmount(unsigned int playerCount) { // players have guesses depending on the number of players
	// should change this function if support for more players and game rules are implemented
	return ceil(log2(playerCount)); // currently, players<=2 have 1 guess, <=4 have 2 guesses, 8<= have 3
}

int gameStages::guessing(Board &board, RNGPointer RNG) {
	for (Player &p: board.players) {
		printToPlayer(p, std::string("Player ") + std::to_string(p.id) + std::string("'s turn to guess:"));
		printToPlayer(p, std::string("You've got ") + std::to_string(getGuessAmount(board.playerCount)) + std::string(" guesses"));
		for (int i = 0; i < getGuessAmount(board.playerCount); i++) {
			guess(board, p, RNG);
			awaitUserInput(board.gameType);
			eliminatePlayers(board);
			int winner = gameEndCondition(board);
			if (winner != -1) {
				return winner;
			}
		}
	}
	eliminatePlayers(board);
	return gameEndCondition(board);
}

int gameStages::roundEnd(Board &board) {
	system("cls");
	disableTilesUsed(board);		// tiles that were in play during this turn will become unavailable for the rest of the game
	board.placedMines.clear();		// clear all mines from the field
	return gameEndCondition(board); // for "not enough tiles remaining" draw condition
}