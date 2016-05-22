#include "AliasMethod.hpp"
#include <fstream>
#include <cstdlib>
using namespace std;

// String split routine
vector<string> split(const char *str, char c = '=')  {
    vector<string> result;

    do  {
        const char *begin = str;

        while(*str != c && *str)
            str++;

        result.push_back(string(begin, str));
    } while (0 != *str++);

	//for(int i = 0; result.size(); i++)  {
	//	cout << result[i] << endl;
	//}
    return result;
}

// Prepare probability vector
vector<double> getProbabilityVector(int loadedSide, int loadAmount)  {
	vector<double> probabilityVector;
	double denominator = 5.0 + loadAmount;
	for(int i = 0; i < 6; i++)  {
		if(i == loadedSide - 1)  {
			probabilityVector.push_back(loadAmount/denominator);
			continue;
		}
		probabilityVector.push_back(1.0/denominator);
	}
	return probabilityVector;

}

int main(int argc, char** argv)  {
	if(argc != 2)  {
		cout << "Usage: dice <filename>" << endl;
		return 1;
	}

	vector<AliasMethod> am_list;
	// Initialize the random number generator
	default_random_engine rnd;
	rnd.seed(clock());

	// Read the key/value pairs from the file
	vector<double> probabilityList;
	string diceId;
	int loadedSide;
	int loadAmount;

	ifstream infile(argv[1]);
	string line;
	getline(infile, line);

	vector<string> tokens = split(line.c_str());

	// Total rolls
	int rolls = atoi(tokens[1].c_str());

	while (getline(infile, line))  {
		tokens = split(line.c_str());
		diceId = tokens[1];

		getline(infile, line);
		tokens = split(line.c_str());
		loadedSide = atoi(tokens[1].c_str());

		getline(infile, line);
		tokens = split(line.c_str());
		loadAmount = atoi(tokens[1].c_str());

		probabilityList = getProbabilityVector(loadedSide, loadAmount);

		// Put the dice AM in the vector
		AliasMethod am = AliasMethod(diceId, probabilityList, rnd);
		am_list.push_back(am);
	}

	//cout << rolls << endl;
	// Create a 2D array to keep track of rolls for each die
	int** rollsTable;
	int* temp;

	rollsTable = (int**)malloc(am_list.size() * sizeof(int*));
	temp = (int*)malloc(am_list.size() * 6 * sizeof(int));
	for (int i = 0; i < am_list.size(); i++) {
	  rollsTable[i] = temp + (i * 6);
	}

	// zero initialize
	for(int i = 0; i < am_list.size(); i++)  {
		for(int j = 0; j < 6; j++)  {
			rollsTable[i][j] = 0;
		}
	}

	// Main loop for rolls
	for(int i = 0; i < rolls; ++i)  {
		cout << "Throw " << i + 1 << ":";
		for(int j = 0; j < am_list.size(); j++)  {
			int face = am_list[j].rollIt();
			++rollsTable[j][face];
			cout << am_list[j].diceId << " rolled a " << face + 1;

			if(j != am_list.size() - 1)  {
				cout << ", ";
			}
		}
		cout << endl;
	}

	// Statistics
	cout << endl;
	for(int i = 0; i < am_list.size(); i++)  {
		cout << am_list[i].diceId << " statistics for " << rolls << " rolls:" << endl;
		for(int j = 0; j < 6; j++)  {
			cout << "Side " << j +1 << ":" << static_cast<double>(rollsTable[i][j])/static_cast<double>(rolls) * 100 << "%" << endl;
		}
		cout << endl;
	}

	//for(int i=0; i<6; ++i)
	//printf("%f\n", 6.0*static_cast<double>(count[i])/static_cast<double>(tries));
	//cout << (count[i]) << endl;
	return 0;
}
