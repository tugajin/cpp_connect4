#ifndef __MOVELEGAL_HPP__
#define __MOVELEGAL_HPP__

#include "game.hpp"
#include "movelist.hpp"
#include <unordered_set>

namespace gen {
void legal_moves(const game::Position &pos, movelist::MoveList &ml) {
    constexpr Bitboard bottom = Bitboard(0x10204081);
    const auto me = pos.turn();
    const auto opp = change_turn(me);
    const auto all = pos.stone(me) + pos.stone(opp);
    const auto next = all + bottom;
#define ADD_MOVE(num) do {\
        if (!(all & (Bitboard(0x10) << num))) {\
            ml.add(static_cast<Move>(next & (Bitboard(0x1F) << num)));\
        }\
    } while(0)
    ADD_MOVE(14);
    ADD_MOVE(7);
    ADD_MOVE(21);
    ADD_MOVE(0);
    ADD_MOVE(28);
#undef ADD_MOVE
}
void test_gen() {
    auto pos = game::Position();
    while(true) {
        Tee<<pos<<std::endl;
        movelist::MoveList ml;
        gen::legal_moves(pos,ml);
        if (ml.len() == 0) { break; }
        Tee<<ml<<std::endl;
        pos = pos.next(ml[0]);
    }
    Tee<<pos<<std::endl;
    Tee<<pos.stone(BLACK) + pos.stone(WHITE)<<std::endl;
    movelist::MoveList ml;
    gen::legal_moves(pos,ml);
    Tee<<ml<<std::endl;
}
void test_gen2() {
    std::unordered_set<std::string> key_set;
    REP(i,10000000000000) {
        auto pos = game::Position();
        Tee<<key_set.size()<<"\r";
        while (1) {
            Tee<<pos<<std::endl;
            Tee<<pos.io()<<std::endl;
            key_set.emplace(pos.history());
            if (pos.is_draw()) {
                //Tee<<pos<<std::endl;
                break;
            }
            if (pos.is_win()) {
                //Tee<<pos<<std::endl;
                break;
            }
            if (pos.is_lose()) {
                //Tee<<pos<<std::endl;
                break;
            }
            movelist::MoveList ml;
            legal_moves(pos,ml);
            if (ml.len() == 0) {
                Tee<<pos<<std::endl;
                std::exit(1);
            }
            const auto action = ml[my_rand(ml.len())];
            pos = pos.next(action);
        }
    }
    Tee<<key_set.size()<<std::endl;
}
}

#endif