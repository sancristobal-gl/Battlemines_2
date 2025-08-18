#include "Battlemines_2/stages.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

unsigned int initRand(std::optional<unsigned int> customSeed = std::nullopt) { // initialize randomizer with either random or custom seed
	unsigned int seed = customSeed.value_or(time(0));
	srand(seed);
	return seed;
}
int gameLoop(Board &board, RNGPointer RNG, userInputFunctionPointer getPlayerInput) {
	/*
	winner = 0, tie
	winner = 1, player 1 won
	winner = 2, player 2 won
	etc
	*/
	int winner = -1;
	while (winner == -1) { // could probably iterate over an array of function pointers?
		gameStages::roundStart(board);
		if (winner != -1) return winner; // not needed here but it looks more symmetrical
		winner = gameStages::minePlacement(board, RNG, getPlayerInput);
		if (winner != -1) return winner;
		winner = gameStages::guessing(board, RNG, getPlayerInput);
		if (winner != -1) return winner;
		winner = gameStages::roundEnd(board);
		if (winner != -1) return winner;
	}
	return winner;
}
int main() {
#ifdef LOG
	std::ofstream log("seed.txt");
	log << initRand();
	log.close();
#endif
#ifdef QUICKSIM
	Board board = createBoard(2, 20, 20, 3, 8);
#else
	Board board = createBoard();
#endif
	userInputFunctionPointer getPlayerInput = getPlayerInputPosition;
	RNGPointer RNG = getRandomValueInRange; //pointer to random number generator function
	int winner = gameLoop(board, RNG, getPlayerInput);
	if (winner != 0) {
		std::cout << "The winner is: player " << winner << ", Congratulations!\n";
	} else {
		std::cout << "The game is a draw!\n";
	}
}
