#include "../../include/logic/Parser_iccma.h"

using namespace std;

void ParserICCMA::parse_af(AF &framework, string file)
{
	ifstream input;
	input.open(file);

	if (!input.good()) {
		cerr << "Cannot open input file\n";
		exit(1);
	}

	string line;
	int32_t n_args = 0;
	
	getline(input, line);
	std::istringstream iss(line);
	if (line[0] == 'p') {
		string p, af;
		iss >> p >> af >> n_args;
	}
	else {
		cerr << "File does not comply to ICCMA'23 format\n";
		exit(1);
	}

	framework.initialize(n_args);

	while (!input.eof()) {
		getline(input, line);
		if (line.length() == 0 || line[0] == '#') continue;
		std::istringstream iss(line);
		uint32_t attacker, victim;
		iss >> attacker >> victim;
		framework.add_attack(attacker, victim);
	}

	input.close();
	framework.finish_initilization();
}