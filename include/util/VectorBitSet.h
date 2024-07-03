#ifndef VECTOR_BITSET_H
#define VECTOR_BITSET_H

#include <vector>
#include <cstdint>
#include <algorithm>
#include <iterator>

using namespace std;

/// <summary>
/// This class is realizes a combined data structure of an array and a bitset.
/// </summary>
class ArrayBitSet {

public:
    /// <summary>
    /// Array of fixed length, used for iterating through the contained elements.
    /// </summary>
    std::vector<uint32_t> _array;
    /// <summary>
    /// Bitset of fixed length, used for existance checks of an element.
    /// </summary>
    std::vector<uint8_t> _bitset;

    ArrayBitSet() {}

    ArrayBitSet(std::vector<uint32_t> new_vector, std::vector<uint8_t> new_bitset) : _array{ new_vector }, _bitset{ new_bitset } {}

    ~ArrayBitSet() {
        _bitset.clear();
        _array.clear();
    }

    /// <summary>
    /// This method creates a new instance and copies all elements of this
    /// instance in the new instance created.
    /// </summary>
    /// <returns>New instance containing all elements of this instance.</returns>
    ArrayBitSet copy() {
        const uint64_t vector_size = _array.size();
        std::vector<uint32_t> copy_vector;
        const uint64_t bitset_size = _bitset.size();
        std::vector<uint8_t> copy_bitset;
        copy_bitset.resize(bitset_size);

        for (int i = 0; i < bitset_size; i++) {
            if (i < vector_size) {
                copy_vector.push_back(_array[i]);
            }

            copy_bitset[i] = _bitset[i];
        }

        return ArrayBitSet(copy_vector, copy_bitset);
    }

    /// <summary>
    /// Frees the space of this instance, but not of its elements.
    /// </summary>
    void clear() {
        _bitset.clear();
        _array.clear();
    }
};

#endif