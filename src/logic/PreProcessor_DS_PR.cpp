#include "../../include/logic/PreProcessor_DS_PR.h"

static pre_proc_result reduce_by_grounded(AF &framework, VectorBitSet &active_args, uint32_t query, VectorBitSet &out_reduct)
{
	// fill list with unattacked arguments
	list<uint32_t> ls_unattacked_unprocessed;
	//iterate through active arguments
	for (int i = 0; i < active_args._vector.size(); i++) {
		//check if argument is unattacked
		if (framework.attackers[active_args._vector[i]]._vector.empty()) {
			ls_unattacked_unprocessed.push_back(active_args._vector[i]);
		}
	}

	

	// init variable of current reduct
	out_reduct = active_args;

	//process list of unattacked arguments
	for (list<uint32_t>::iterator mIter = ls_unattacked_unprocessed.begin(); mIter != ls_unattacked_unprocessed.end(); ++mIter) {
		const auto &ua = *mIter;

		//reject query if it gets attacked by argument of grounded extension
		if( ua == query) {
			return pre_proc_result::accepted;
		}


		//reject query if it gets attacked by argument of grounded extension
		if (framework.victims[ua]._bitset[query]) {
			return pre_proc_result::rejected;
		}

		//reduce active argument by unattacked argument + update current reduct
		out_reduct = Reduct::get_reduct(out_reduct, framework, ua);

		//iterate through victims of the victims of ua
		for (int i = 0; i < framework.victims[ua]._vector.size(); i++) {
			uint32_t vua = framework.victims[ua]._vector[i];
			for (int j = 0; j < framework.victims[vua]._vector.size(); j++) {
				uint32_t vvua = framework.victims[vua]._vector[j];

				if (!out_reduct._bitset[vvua]) {
					//only account victims of victims that are still active
					continue;
				}

				//check if victim of victim is unattacked
				vector<uint32_t> attackers_vvua = framework.attackers[vvua]._vector;
				uint8_t is_unattacked = 1;
				for (int k = 0; k < attackers_vvua.size(); k++) {
					if (out_reduct._bitset[attackers_vvua[k]]) {
						is_unattacked = 0;
						break;
					}
				}

				//add to list if vvua is unattacked
				if (is_unattacked) {
					ls_unattacked_unprocessed.push_back(vvua);
				}
			}
		}
	}

	return pre_proc_result::unknown;

	//gehe durch Liste der unattacked arguments  _ua
		//ist query unter den victims von _ua? wenn ja beende und gebe rejected zurück
		//reduziere um das unattacked argument _ua
		//update _r mit aktuellen redukt und mache redukt so über einzelne schleifendurchläufe hinaus persistent
		//prüfe ob in den victims jedes victims des unattacked arguments, also in victim(victim(_ua))  _vua
			//prüfe ob argument _vua im neuen reduct unattacked ist
			//wenn ja füge es zur liste hinzu
		
}


pre_proc_result PreProc_DS_PR::process(AF &framework, VectorBitSet &active_args, uint32_t query, VectorBitSet &out_reduct) {
	
	if (framework.victims[query]._bitset[query])
	{
		return pre_proc_result::rejected;
	}

	if (framework.attackers[query]._vector.empty())
	{
		return pre_proc_result::accepted;
	}
	
	return reduce_by_grounded(framework, active_args, query, out_reduct);
}
