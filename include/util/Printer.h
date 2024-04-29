#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <cstdint>
#include <list>

using namespace std;

class Printer {

public:
	static void print_list(list<uint32_t> &list);

	static void print_vector(vector<uint32_t> &vector);
};
#endif