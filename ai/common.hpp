#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include "util.hpp"

constexpr int SQUARE_SIZE = 25;
constexpr int FILE_SIZE = 5;
constexpr int RANK_SIZE = 5;

#define REP(i,e) for (auto i = 0; i < (e); ++i)
#define REP_FILE(i) for (auto i = 0; i < FILE_SIZE; ++i)
#define REP_RANK(i) for (auto i = RANK_SIZE-1; i >= 0; --i)

typedef std::vector<std::vector<int>> Feature;
typedef double NNScore;
typedef uint64 Key;
typedef uint64 Bitboard;


enum Move : uint64 {
    MOVE_NONE = 0xFFFFFFFFFFFFFFFF,
};
enum Color : int {
    BLACK = 0, WHITE = 1, COLOR_SIZE = 2,
};
std::string color_str(const Color c) {
    if (c == BLACK) {
        return "BLACK";
    } else {
        return "WHITE";
    }
}
std::ostream& operator<<(std::ostream& os, const Color c) {
    os << color_str(c);
    return os;
}
inline Color change_turn(const Color turn) {
    return static_cast<Color>(1 ^ static_cast<int>(turn));
}
inline bool move_is_ok(const Move m) {
    const auto v = static_cast<uint64>(m);
    return (v >= 1 && v <= (1ull << 31));
}
inline bool sq_is_ok(const int sq) {
    return (sq >= 0 && sq <= 32);
}

inline int to_sq (const int file, const int rank) {
    return (file * 7) + rank;
}

inline int to_file (const int sq) {
    return int(sq / 7);
}

NNScore to_nnscore(const float sc) {
    auto score = static_cast<int>(sc * 10000);
    if (score >= 10000) {
        score = 9999;
    } else if (score <= -10000) {
        score = -9999;
    }
    return static_cast<NNScore>(static_cast<double>(score) / 10000.0);
}
inline std::string move_str(const Move m) {
    const auto v = static_cast<uint64>(m);
    if (v == 0) {
        return "0";
    }
    REP(i,64) {
        if ((1ull << i) & v) {
            return to_string(i);
        }
    }
    return "error("+ to_string(v)+")";
}
void check_mode() {
#if DEBUG
    Tee<<"debug mode\n";
#elif NDEBUG
    Tee<<"release mode\n";
#else
    Tee<<"unknown mode\n";
#endif
}

inline int bit_first(Bitboard b) { ASSERT(b != 0); return __builtin_ctzll(b); }
inline int bit_count(Bitboard b) { return __builtin_popcountll(b); }

#endif
