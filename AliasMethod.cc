#include "AliasMethod.hpp"
using namespace std;

// Constructor
AliasMethod::AliasMethod(string id, vector<double> probability_list, default_random_engine rnd)
: diceId(id), probability(probability_list), random_number_generator(rnd)  {
	// Check for empty
	if(probability.size() < 1)  {
		cout << "Probability list can't be empty!!" << endl;
		exit(1);
	}

	/* Allocate space for the probability and alias tables. */
	alias_table.resize(probability.size());

	// Check if our allocation was successfull
	if(probability.size() < 1 || alias_table.size() < 1)  {
		cout << "Table allocation failed. Exiting..." << endl;
		exit(1);
	}

	// Calculate the average probability
	double average_probability = 1.0/probability.size();

	// Create two lists, large and small, representing the two probabilities in a rectangle
	vector<int> small;
	vector<int> large;

	/* Populate the stacks with the input probabilities. */
	for (int i = 0; i < probability.size(); i++) {
		/* If the probability is below the average probability, then we add
		 * it to the small list; otherwise we add it to the large list.
		 */
		if (probability[i] >= average_probability)
			large.push_back(i);
		else
			small.push_back(i);
	}

	// This is the modified Vose's Algorithm
	// Since floating point operations can cause inconsistancies
	// We check that both lists aren't empty to avoid looping forever
	while(!small.empty() && !large.empty())  {
		int less = small.back(); small.pop_back();
		int more = large.back(); large.pop_back();

		alias_table[less] = more;

		// Decrease the probability of the larger one by the appropriate
		// amount.
		probability[more] = (probability[more] + probability[less]) - average_probability;
		if (probability[more] >= average_probability)
			large.push_back(more);
		else
			small.push_back(more);
	}

	// That's all the probabilities that needed scaling
	// Now just make the remaining ones average
	while(!small.empty())  {
		probability[small.back()] = average_probability;
		small.pop_back ();
	}
	while(!large.empty())  {
		probability[large.back()] = average_probability;
		large.pop_back();
	}

	int n = static_cast<int>(probability.size());
	transform(probability.cbegin(), probability.cend(), probability.begin(),
	[n](double p){ return p * n; });
}

// Simulates a loaded dice roll
int AliasMethod::rollIt() {
	/* Generate a fair die roll to determine which column to inspect. */
	uniform_int_distribution<int> distribution(0,5);
	int column = distribution(random_number_generator);


	/* Generate a biased coin toss to determine which option to pick. */
	uniform_real_distribution<double> distribution1(0.0,1.0);
	bool coinToss = distribution1(random_number_generator) < probability[column];

	/* Based on the outcome, return either the column or its alias. */
	return coinToss? column : alias_table[column];
}
