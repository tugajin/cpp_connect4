#ifndef __NN_HPP__
#define __NN_HPP__
#include "game.hpp"
#include "common.hpp"
#include <vector>
namespace nn {
constexpr inline int FEAT_SIZE = 2;
typedef std::vector<std::vector<int>> Feature;
typedef double NNScore;

inline NNScore to_nnscore(const float sc) {
    auto score = static_cast<int>(sc * 10000);
    if (score >= 10000) {
        score = 9999;
    } else if (score <= -10000) {
        score = -9999;
    }
    return static_cast<NNScore>(static_cast<double>(score) / 10000.0);
}

Feature feature(const game::Position &pos) {
    Feature feat(FEAT_SIZE, std::vector<int>(SQUARE_SIZE, 0));
    const auto me = pos.turn();
    const auto opp = change_turn(me);
    auto sp = 0;
    REP_RANK(rank) {
        REP_FILE(file) { 
            const auto sq = to_sq(file, rank);
            if (pos.stone(me) & (1ull << sq)) {
                feat[me][sp] = 1;
            } else if (pos.stone(opp) & (1ull << sq)) {
                feat[opp][sp] = 1;
            }
            sp++;
        }
    }
    return feat;
}
void test_nn() {
}

}
#endif