#include "../../include/logic/PreProcessor_DS_PR.h"

static pre_proc_result reduce_by_grounded(AF &framework, unordered_set<uint32_t> &active_args, uint32_t query, unordered_set<uint32_t> *output_reduct)
{
	// fill list with unattacked arguments
	list<uint32_t> ls_unattacked_unprocessed;
	//iterate through active arguments
	for (size_t bno = 0; bno < active_args.bucket_count(); ++bno) {
		for (auto bit = active_args.begin(bno), end = active_args.end(bno); bit != end; ++bit) {
			const auto &argument = *bit;
			//check if argument is unattacked
			if (framework.attackers[argument].empty()) {
				ls_unattacked_unprocessed.push_back(argument);
			}
		}
	}

	// init variable of current reduct
	*output_reduct = active_args;

	//process list of unattacked arguments
	for (list<uint32_t>::iterator mIter = std::next(ls_unattacked_unprocessed.begin()); mIter != ls_unattacked_unprocessed.end(); ++mIter) {
		const auto &ua = *mIter;
		//reject query if it gets attacked by argument of grounded extension
		if (framework.victims[ua].count(query)) {
			return pre_proc_result::rejected;
		}

		//reduct active argument by unattacked argument
		unordered_set<uint32_t> tmp = Reduct::get_reduct(*output_reduct, framework, ua);
		//update current reduct
		output_reduct->clear();
		*output_reduct = tmp;

		//iterate through victims of the victims of ua
		unordered_set<uint32_t> victims = framework.victims[ua];
		for (size_t bno = 0; bno < victims.bucket_count(); ++bno) {
			for (auto bit = victims.begin(bno), end = victims.end(bno); bit != end; ++bit) {
				const auto &vua = *bit;

				unordered_set<uint32_t> victims_of_victims = framework.victims[vua];
				for (size_t bno2 = 0; bno2 < victims_of_victims.bucket_count(); ++bno2) {
					for (auto bit2 = victims_of_victims.begin(bno2), end = victims_of_victims.end(bno2); bit2 != end; ++bit2) {
						const auto &vvua = *bit2;
						//check if victim is unattacked
						vector<uint32_t> attackers_vvua = framework.attackers[vvua];
						uint8_t is_unattacked = 1;
						for (int i = 0; i < attackers_vvua.size(); i++) {
							if (output_reduct->count(attackers_vvua[i])) {
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


pre_proc_result PreProc_DS_PR::process(AF &framework, unordered_set<uint32_t> &active_args, uint32_t argument) {
	
	if (framework.victims[argument].count(argument))
	{
		return pre_proc_result::rejected;
	}

	if (framework.attackers[argument].empty())
	{
		return pre_proc_result::accepted;
	}

	return unknown;
}
