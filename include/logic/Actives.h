#ifndef ACTIVES_H
#define ACTIVES_H

#include <vector>
#include <cstdint>

using namespace std;

class Actives {

public:
	static int64_t isActive(vector<uint32_t> activeArgs, uint32_t argument);
};

#endif
