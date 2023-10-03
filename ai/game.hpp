#ifndef __GAME_HPP__
#define __GAME_HPP__

#include <array>
#include <bitset>
#include <functional>
#include "common.hpp"
#include "util.hpp"
#include "movelist.hpp"
#include "bitboard.hpp"

namespace game {
class Position;
}
namespace game {
/*
https://github.com/denkspuren/BitboardC4/blob/master/BitboardDesign.md
  6 13 20 27 34 41 48   55 62     Additional row
+---------------------+ 
| 5 12 19 26 33 40 47 | 54 61     top row
| 4 11 18 25 32 39 46 | 53 60
| 3 10 17 24 31 38 45 | 52 59
| 2  9 16 23 30 37 44 | 51 58
| 1  8 15 22 29 36 43 | 50 57
| 0  7 14 21 28 35 42 | 49 56 63  bottom row
+---------------------+
*/
/*
https://github.com/denkspuren/BitboardC4/blob/master/BitboardDesign.md
| 6 13 20 27 34 | 41 48 55 62     Additional row
| 5 12 19 26 33 | 40 47 54 61     Additional row
+---------------+ 
| 4 11 18 25 32 | 39 46 53 60     top row
| 3 10 17 24 31 | 38 45 52 59
| 2  9 16 23 30 | 37 44 51 58
| 1  8 15 22 29 | 36 43 50 57
| 0  7 14 21 28 | 35 42 49 56 63  bottom row
+---------------+
*/
class Position {
public:
    Position() {
        this->turn_ = BLACK;
        this->stone_[BLACK] = this->stone_[WHITE] = 0ULL;
        this->ply_ = 0;
        REP(i,SQUARE_SIZE) {
            this->moves_[i] = -1;
        }
    }
    Position(const Bitboard stones[], const Color turn) {
        this->turn_ = turn;
        this->stone_[BLACK] = stones[BLACK];
        this->stone_[WHITE] = stones[WHITE];
        this->ply_ = 0;
        REP(i,SQUARE_SIZE) {
            this->moves_[i] = -1;
        }
    }
    bool is_lose() const {
        const auto bitboard = this->stone_[change_turn(this->turn())];
        if ((bitboard & (bitboard >> 6) & (bitboard >> 12) & (bitboard >> 18)) != 0) return true; // diagonal 
        if ((bitboard & (bitboard >> 8) & (bitboard >> 16) & (bitboard >> 24)) != 0) return true; // diagonal 
        if ((bitboard & (bitboard >> 7) & (bitboard >> 14) & (bitboard >> 21)) != 0) return true; // horizontal
        if ((bitboard & (bitboard >> 1) & (bitboard >>  2) & (bitboard >>  3)) != 0) return true; // vertical
        return false;
    }
    bool is_win() const {
        const auto bitboard = this->stone_[this->turn()];
        if ((bitboard & (bitboard >> 6) & (bitboard >> 12) & (bitboard >> 18)) != 0) return true; // diagonal 
        if ((bitboard & (bitboard >> 8) & (bitboard >> 16) & (bitboard >> 24)) != 0) return true; // diagonal 
        if ((bitboard & (bitboard >> 7) & (bitboard >> 14) & (bitboard >> 21)) != 0) return true; // horizontal
        if ((bitboard & (bitboard >> 1) & (bitboard >>  2) & (bitboard >>  3)) != 0) return true; // vertical
        return false;
    }
    bool is_draw() const {
        const auto stones = this->stone(BLACK) | this->stone(WHITE);
        if (stones == 0x1F3E7CF9F) { return true; }
        return false;
    }
    bool is_done() const {
        if (this->is_win()) { return true; }
        if (this->is_lose()) { return true; }
        if (this->is_draw()) { return true; }
        return false;
    }
    int ply() const {
        return this->ply_;
    }
    Bitboard stone(const Color c) const {
        return this->stone_[c];
    }
    Position next(const Move action) const {
        Bitboard stones[COLOR_SIZE];
        stones[BLACK] = this->stone_[BLACK];
        stones[WHITE] = this->stone_[WHITE];
        stones[this->turn()] ^= action;
        auto p = Position(stones,change_turn(this->turn()));
        p.ply_ = this->ply_;
        REP(i,p.ply_+1) {
            p.moves_[i] = this->moves_[i];
        }
        p.moves_[p.ply_++] = bit_first(static_cast<Bitboard>(action));
        return p;
    }
    Color turn() const {
        return this->turn_;
    }
    bool is_ok() const {
        if (this->turn_ != BLACK && this->turn_ != WHITE) {
            return false;
        }
        return true;
    }

    std::string str() const {
        std::string str = "turn:" + color_str(this->turn()) + "\n";
        str += "io:" + this->io() +"\n";
        str += "ply:" + to_string(this->ply_)+"\n";
        REP_RANK(rank) {
            REP_FILE(file) { 
                const auto sq = to_sq(file, rank);
                if (this->stone_[BLACK] & (1ull << sq)) {
                    str += "o";
                } else if (this->stone_[WHITE] & (1ull << sq)) {
                    str += "x";
                } else {
                    str += ".";
                }
            }
            str += "\n";
        }
        return str;
    }
    std::string history() const {
        return this->io();
    }
	friend std::ostream& operator<<(std::ostream& os, const Position& pos) {
        os << pos.str();
		return os;
	}
    std::string io() const {
        std::string str = "io";
        REP(i, this->ply_) {
            str += to_string(to_file(this->moves_[i])+1);
        }
        return str;
    }
private:
    Bitboard stone_[COLOR_SIZE];
    int moves_[SQUARE_SIZE+1];
    int ply_;
    Color turn_;
};

void test_pos() {
  
}    
void test_nn() {
}
}
#endif