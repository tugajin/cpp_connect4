#ifndef __HASH_HPP__
#define __HASH_HPP__
#include "game.hpp"
#include "common.hpp"
namespace hash {

Key hash_key(const game::Position &pos) {
    Key k(0ULL);
    const auto me = pos.turn();
    k = static_cast<Key>(pos.stone(BLACK) + pos.stone(WHITE) + pos.stone(me));
    return k;
}
std::string hash_key2(const game::Position &pos) {
    std::string str = "";
    if (pos.turn() == BLACK) {
        str += "b";
    } else {
        str += "w";
    }
    REP(i,33) {
        if ((1ull << i) & pos.stone(BLACK)) {
            str += "o";
        }
        else if ((1ull << i) & pos.stone(WHITE)) {
            str += "x";
        } else {
            str += ".";
        }
    }
    return str;
}
game::Position from_hash2(const std::string str) {
    auto i = 0;
    Bitboard stones[COLOR_SIZE] = {0ull,0ull};
    for (const auto c : str) {
        if (c == 'b' || c == 'w') { continue; }
        if (c == 'o') {
            stones[BLACK] |= static_cast<Bitboard>(1ull << i);
        } 
        if (c == 'x') {
            stones[WHITE] |= static_cast<Bitboard>(1ull << i);
        }
        i++;
    }
    i = 0;
    auto pos = game::Position();
    while (stones[BLACK] | stones[WHITE]) {
        const auto col = pos.turn();
        const auto id = bit_first(stones[col]);
        stones[col] ^= (1ull << id);
        const auto mv = static_cast<Move>(1ull << id);
        pos = pos.next(mv);
    }
    return pos;
}
void test_hash() {
    Tee<<from_hash2("bxxoxo..x......xoxxx..oooxo..ooxo.")<<std::endl;
    Tee<<from_hash2("boox....x.........................")<<std::endl;
}
}
#endif