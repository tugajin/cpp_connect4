#ifndef __IO_HPP__
#define __IO_HPP__

#include "game.hpp"
#include "movelegal.hpp"
#include "bitboard.hpp"

namespace io {

game::Position from_str(std::string position_str) {
    auto pos = game::Position();
    for (const auto c : position_str) {
        if (c == 'i' || c == 'o') {
            continue;
        }
        std::string input_file_str{c}; 
        const auto input_file = to_int(input_file_str);
        movelist::MoveList ml;
        gen::legal_moves(pos, ml);
        auto next_move = MOVE_NONE;
        REP(i,ml.len()) {
            const auto mv = ml[i];
            const auto sq = bit_first(static_cast<Bitboard>(mv));
            const auto mv_file = to_file(sq)+1;
            if (mv_file == input_file) {
                next_move = mv;
                break;
            }
        }
        if (next_move == MOVE_NONE) {
            Tee<<"error\n";
            std::exit(1);
        }
        pos = pos.next(next_move);
    }
    return pos;
}

std::string io(const game::Position &pos) {
    return pos.io();
}

game::Position hirate() {
    auto pos = game::Position();
    return pos;
}

void test_io() {
    auto pos = from_str("44455554221");
    Tee<<pos<<std::endl;
}

};

#endif