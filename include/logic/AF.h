#ifndef AF_H
#define AF_H

#include <vector>
#include <unordered_set>
#include <cstdint>
#include <iostream>	
#include <omp.h>

template <class T>
inline void hash_combine(std::size_t &seed, const T &v)
{
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std
{
	template<typename S, typename T> struct hash<pair<S, T>>
	{
		inline size_t operator()(const pair<S, T> &v) const
		{
			size_t seed = 0;
			::hash_combine(seed, v.first);
			::hash_combine(seed, v.second);
			return seed;
		}
	};
}

class AF {

public:

    uint32_t num_args;
    std::vector<std::unordered_set<uint32_t>> attackers;
	std::vector<std::unordered_set<uint32_t>> victims;
	std::vector<uint8_t> self_attack;
    std::unordered_set<std::pair<uint32_t, uint32_t>> attacks;
	std::unordered_set<std::pair<uint32_t, uint32_t>> symmetric_attacks;

    bool add_attack(uint32_t attacker, uint32_t victim);
	void initialize(uint32_t number_args);
    void initialize_af();
    bool exists_attack(uint32_t attacker, uint32_t victim) const;
};

#endif