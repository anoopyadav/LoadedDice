#pragma once
#include <algorithm>
#include <vector>
#include <deque>
#include <iostream>
#include <random>
#include <ctime>

class AliasMethod  {
	private:
		// Random number generator
		std::default_random_engine random_number_generator;

		// Probability and alias tables
		std::vector<int> alias_table;
		std::vector<double> probability;
	public:
		// Dice ID
		std::string diceId;
		// Constructor
		AliasMethod(std::string diceId, std::vector<double> probability_list, std::default_random_engine rnd);
		int rollIt();
};
