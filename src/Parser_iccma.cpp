#include "../include/Parser_iccma.h"

using namespace std;

static string get_next_non_space(string line, uint32_t idxStart, uint32_t *out_newIdxStart)
{
	uint32_t idxEnd = line.find(' ', idxStart);
	string output = line.substr(idxStart, idxEnd - idxStart);

	*out_newIdxStart = line.find_first_not_of(' ', idxEnd);

	return output;
}

argFramework_t* ParserICCMA::parse_af(string file)
{
	ifstream input;
	input.open(file);

	if (!input.good()) {
		cerr << "Cannot open input file\n";
		exit(1);
	}

	string line, word;
	uint32_t idxWord = 0;

	getline(input, line);
	word = get_next_non_space(line, idxWord, &idxWord);
	
	if (line.length() < 4 || word != "p")
	{
		cerr << "File does not comply to ICCMA'23 format\n";
		exit(1);
	}
	word = get_next_non_space(line, idxWord, &idxWord);
	if (word != "af")
	{
		cerr << "File does not describe a simple abstract argumentation framework (af)\n";
		exit(1);
	}

	word = get_next_non_space(line, idxWord, &idxWord);
	uint32_t numArgs = std::stoi(word);
	argumentInitTemp_t *head = set_up_initialization(numArgs);

	uint32_t attacker, victim;
	while (!input.eof()) {
		getline(input, line);
		idxWord = 0;
		if (line.length() == 0 || line[0] == '#') continue;
		word = get_next_non_space(line, idxWord, &idxWord);
		attacker = std::stoi(word);
		word = get_next_non_space(line, idxWord, &idxWord);
		victim = std::stoi(word);
		add_attack(head, attacker, victim);
	}

	return initialize_framework(head);
}