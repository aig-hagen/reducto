#include "../../include/logic/PreProcessor_DS_PR.h"

static pre_proc_result reduce_by_grounded(AF &framework, VectorBitSet &active_args, uint32_t query, VectorBitSet &out_reduct)
{
	// fill list with unattacked arguments
	list<uint32_t> ls_unattacked_unprocessed;
	vector<uint32_t> num_attacker;
	//iterate through active arguments
	for (int i = 0; i < active_args._vector.size(); i++) {
		//check if argument is unattacked
		if (framework.attackers[active_args._vector[i]]._vector.empty()) {
			ls_unattacked_unprocessed.push_back(active_args._vector[i]);
		}

		num_attacker.push_back(framework.attackers[active_args._vector[i]]._vector.size());
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

		//iterate through victims of the victims of ua
		for (int i = 0; i < framework.victims[ua]._vector.size(); i++) {
			uint32_t vua = framework.victims[ua]._vector[i];

			if (!out_reduct._bitset[vua]) {
				//only account victims that are still active
				continue;
			}

			for (int j = 0; j < framework.victims[vua]._vector.size(); j++) {
				uint32_t vvua = framework.victims[vua]._vector[j];

				if (!out_reduct._bitset[vvua]) {
					//only account victims of victims that are still active
					continue;
				}

				//update number of attackers
				num_attacker[vvua - 1]--;

				//check if victim of victim is unattacked
				if (num_attacker[vvua - 1] == 0) {
					ls_unattacked_unprocessed.push_back(vvua);
				}
			}
		}

		//reduce active argument by unattacked argument + update current reduct
		out_reduct = Reduct::get_reduct(out_reduct, framework, ua);
	}

	return pre_proc_result::unknown;

	//gehe durch Liste der unattacked arguments  _ua
		//ist query unter den victims von _ua? wenn ja beende und gebe rejected zur�ck
		//reduziere um das unattacked argument _ua
		//update _r mit aktuellen redukt und mache redukt so �ber einzelne schleifendurchl�ufe hinaus persistent
		//pr�fe ob in den victims jedes victims des unattacked arguments, also in victim(victim(_ua))  _vua
			//pr�fe ob argument _vua im neuen reduct unattacked ist
			//wenn ja f�ge es zur liste hinzu
		
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
