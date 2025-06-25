#include "../include/Solver.h"

Solver::Solver(){
	_framework = AF();
	_query_argument = 0;
	_semantics = admissible;
	_solution = list<uint32_t>();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Solver::set_framework(AF &framework) {
	_framework = framework;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Solver::set_query(uint32_t arg) {
	_query_argument = arg;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Solver::set_semantics(semantics sem) {
	_semantics = sem;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

int32_t Solver::solve(bool credulous_mode) {
	if (_query_argument == 0 || _query_argument > _framework.num_args) {
		return -1;
	}

	_solution = list<uint32_t>();
	bool isAccepted = false;
	if(credulous_mode) {
		switch (_semantics) {
		case complete:
			isAccepted = Solver_DC_CO::solve(_query_argument, _framework, _solution);
			break;
		case stable:
			isAccepted = Solver_DC_ST::solve(_query_argument, _framework, _solution);
			break;
		default:
			cerr << _semantics << ": Unsupported semantics\n";
			return -1;
		}
	}
	else {
		switch (_semantics) {
		case preferred:
			isAccepted = Solver_DS_PR::solve(_query_argument, _framework, _solution);
			break;
		case stable:
			isAccepted = Solver_DS_ST::solve(_query_argument, _framework, _solution);
			break;
		default:
			cerr << _semantics << ": Unsupported semantics\n";
			return -1;
		}
	}

	if (isAccepted) {
		return 10;
	}
	else {
		return 20;
	}
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

int32_t Solver::val(uint32_t arg) {
	if (tools::Tools_List::contains(_solution, arg)) {
		return arg;
	}
	else {
		return -1 * arg;
	}
}