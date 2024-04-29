#include "../../include/util/Printer.h"

using namespace std;

void Printer::print_list(list<uint32_t> &listToPrint) {

	if (listToPrint.empty()) {
		return;
	}

	cout << *listToPrint.begin();
	for (list<uint32_t>::iterator mIter = std::next(listToPrint.begin()); mIter != listToPrint.end(); ++mIter) {
		cout << " " << *mIter;
	}
}

void Printer::print_vector(vector<uint32_t> &vector) {
	if (vector.empty()) {
		return;
	}

	cout << vector[0];
	for (int i = 1; i < vector.size(); i++) {
		cout << " " << vector[i];
	}
}