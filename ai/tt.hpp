#ifndef __TT_HPP__
#define __TT_HPP__

#include "common.hpp"
#include "util.hpp"

namespace tt {

class TranspositionTable {
private:
    struct TTEntry {
        uint64 key;
        int8 val;
    };
    static constexpr uint64 size = 1ull << 2;
    static constexpr uint64 mask = size-1;
    TTEntry* ptr;
 
public:
    TranspositionTable() {
        this->ptr = new TTEntry[size];
        this->clear();
    }
    ~TranspositionTable() {
        delete[] this->ptr;
    }
    void clear() {
        Tee<<"clear...";
        for (auto i = 0ull; i < size; i++) {
            this->ptr[i].key = 0ull;
        }
        Tee<<"end\n";
    }
    void put(const Key key, const int val) {
        const auto index = key & mask;
        this->ptr[index].key = key;
        this->ptr[index].val = static_cast<int8>(val);
    }
    bool get(const Key key, int &score) const {
        const auto index = key & mask;
        if (this->ptr[index].key != key) {
            return false;
        } else {
            score = static_cast<int>(this->ptr[index].val);
            return true;
        }
    }
};
//extern TranspositionTable g_tt;

};
#endif