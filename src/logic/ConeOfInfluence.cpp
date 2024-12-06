#include "../../include/logic/ConeOfInfluence.h"

ConeOfInfluence::ConeOfInfluence(AF &framework) {
	distance_to_query.resize(static_cast<uint64_t>(framework.num_args) + 1);
	std::fill(distance_to_query.begin(), distance_to_query.end(), 0);
	max_distance = 0;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

ArgumentDistancePair::ArgumentDistancePair(uint32_t _argument, ConeOfInfluence coi) {
	argument = _argument;
	distance_to_query = coi.distance_to_query[_argument];
}


bool ArgumentDistancePair::compare_by_distance(ArgumentDistancePair firstElem, ArgumentDistancePair secondElem) {
	return firstElem.distance_to_query >= secondElem.distance_to_query;
}