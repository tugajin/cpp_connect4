#ifndef __SEARCH_HPP__
#define __SEARCH_HPP__

#include <climits>
#include "game.hpp"
#include "common.hpp"
#include "movelegal.hpp"
#include "io.hpp"
#include "tt.hpp"
#include "hash.hpp"

namespace search {

constexpr int SEARCH_MATE = 10000;
constexpr int SEARCH_MAX  = 20000;
constexpr int SEARCH_MIN  = -SEARCH_MAX;

bool has_winner(const game::Position &pos, movelist::MoveList &ml) {
    REP(i, ml.len()) {
        auto next_pos = pos.next(ml[i]);
        if (next_pos.is_lose()) {
            return true;
        }
    }
    return false;
}

int search(game::Position &pos, const int depth);

Move search_root(game::Position &pos, const int depth, int &best_sc) {
    movelist::MoveList ml;
    gen::legal_moves(pos,ml);
    auto best_move = MOVE_NONE;
    auto lose_num = 0;
    best_sc = -99;
    for (const auto m : ml) {
        auto next_pos = pos.next(m);
        const auto score = -search(next_pos, depth-1);
        if (score == 1) {
            return m;
        } else if (score == 0) {
            best_sc = 0;
            best_move = m;
        } else if (score == -1) {
            lose_num++;
        }
    }
    if (lose_num == ml.len()) {
        best_sc = -1;
    }
    return best_move;
}

int search(game::Position &pos, int depth) {
    ASSERT2(pos.is_ok(),{
        Tee<<pos<<std::endl;
    });
    if (pos.is_win()) {
        return 1;
    }
    if (pos.is_lose()) {
        return -1;
    }
    if (pos.is_draw()) {
        return 0;
    }
    if (depth < 0) {
        return -99;
    }
    movelist::MoveList ml;
    gen::legal_moves(pos, ml);
    // 勝ちにつながる手があるなら終わり
    if (has_winner(pos,ml)) {
        return 1;
    }
    auto best_score = -99;
    auto lose_num = 0;
    for (const auto m : ml) {
        auto next_pos = pos.next(m);
        const auto score = -search(next_pos, depth-1);
        if (score == 1) {
            return 1;
        } else if (score == 0) {
            best_score = 0;
        } else if (score == -1) {
            lose_num++;
        }
    }
    //最善が引き分け
    if (best_score == 0) {
        return 0;
    } else if (ml.len() == lose_num) {
        return -1;
    }
    //結果がわからない
    return -99;
}

}
#endif