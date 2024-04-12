#include "../../include/logic/Parser_iccma.h"

using namespace std;

static string get_next_non_space(string line, uint32_t idxStart, uint32_t *out_newIdxStart)
{
	uint32_t idxEnd = line.find(' ', idxStart);
	string output = line.substr(idxStart, idxEnd - idxStart);

	*out_newIdxStart = line.find_first_not_of(' ', idxEnd);

	return output;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

argFramework_t* ParserICCMA::parse_af(string file)
{
	//float start_time = omp_get_wtime();																										//DEBUG
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
	//printf("%d: ------- setup initialization framework --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());						//DEBUG
	//long mem_base = get_mem_usage();																											//DEBUG

	uint32_t attacker, victim;
	//printf("Start parsing\n");																												//DEBUG
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
	//printf("Finished parsing\n");																												//DEBUG

	input.close();
	//printf("%d: ------- finished reading file --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());								//DEBUG
	//printf("Memory space needed to process AF as input: %ld [kB]\n", get_mem_usage() - mem_base);												//DEBUG

	//float end_time = omp_get_wtime();																											//DEBUG
	//float duration = end_time - start_time;																									//DEBUG
	//printf("Parsing framework: %.2f s\n", duration);																							//DEBUG

	return initialize_framework(head);
}