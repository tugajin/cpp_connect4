#ifndef __ORACLE_HPP__
#define __ORACLE_HPP__

#include <unordered_map>  
#include "game.hpp"
#include "movelegal.hpp"
#include "search.hpp"
#include "hash.hpp"

namespace oracle {
int search(const game::Position &pos, uint64 &nodes, std::unordered_map<std::string, int> &mp);

void check_all_pos() {
    auto pos = game::Position();
    uint64 nodes = 0ull;
    std::unordered_map<std::string, int> mp;
    search(pos, nodes, mp);

    std::ofstream file("output.txt"); // ファイル書き込み用のofstreamオブジェクトを作成

    if (!file.is_open()) {
        std::cerr << "ファイルを開けませんでした" << std::endl;
        return ;
    }

    for(auto pair:mp) {
	    file << pair.first << ":" << pair.second << std::endl;
    }
    file.close(); // ファイルを閉じる
}

void store_data(std::unordered_map<std::string, int> &mp, const std::string io, const int score) {
    ASSERT2((score == 0 || score == 1 || score == -1),{
        Tee<<"error score\n";
        Tee<<score<<std::endl;
    });
    if (mp.find(io) != mp.end()) {
        return;
    }
    mp[io] = score;
}

int search(const game::Position &pos, uint64 &nodes, std::unordered_map<std::string, int> &mp) {
    nodes++;

    ASSERT2(pos.is_ok(),{
        Tee<<pos<<std::endl;
    });
    if (nodes % 100000 == 0) {
        Tee<<"nodes:"<<nodes<<" mp:"<<mp.size()<<std::endl;
    }
    const std::string key = hash::hash_key2(pos);
    if (mp.find(key) != mp.end()) {
        const auto score = mp[key];
        // Tee<<"find\n";
        // Tee<<key<<std::endl;
        // Tee<<mp[key]<<std::endl;
        // Tee<<pos<<std::endl;
        // Tee<<"----------------------\n";
        return score;
    }
    ASSERT(mp.find(key) == mp.end());
    if (pos.is_win()) {
        // Tee<<"store\n";
        // Tee<<key<<std::endl;
        // Tee<<1<<std::endl;
        // Tee<<pos<<std::endl;
        // Tee<<"----------------------\n";
        store_data(mp,key,1);
        return 1;
    }
    if (pos.is_lose()) {
        // Tee<<"store2\n";
        // Tee<<key<<std::endl;
        // Tee<<-1<<std::endl;
        // Tee<<pos<<std::endl;
        // Tee<<"----------------------\n";
        store_data(mp,key,-1);
        return -1;
    }
    if (pos.is_draw()) {
        // Tee<<"store3\n";
        // Tee<<key<<std::endl;
        // Tee<<0<<std::endl;
        // Tee<<pos<<std::endl;
        // Tee<<"----------------------\n";
        store_data(mp,key,0);
        return 0;
    }
    movelist::MoveList ml;
    gen::legal_moves(pos, ml);
    // // 勝ちにつながる手があるなら終わり
    // if (search::has_winner(pos,ml)) {
    //     store_data(mp,io,1);
    //     return 1;
    // }
    auto best_score = -2;
    for (const auto m : ml) {
        auto next_pos = pos.next(m);
        const auto score = -search(next_pos, nodes, mp);
        if (score > best_score) {
            best_score = score;
        }
    }
    ASSERT2(std::abs(best_score) <= 1,{
        Tee<<"best_score:"<<best_score<<std::endl;
    } );
    // Tee<<"store4\n";
    // Tee<<key<<std::endl;
    // Tee<<best_score<<std::endl;
    // Tee<<pos<<std::endl;
    // Tee<<"----------------------\n";
    if (mp.find(key) != mp.end()) {
        ASSERT2(mp[key] == best_score,{
            Tee<<pos<<std::endl;
            Tee<<key<<std::endl;
            Tee<<mp[key]<<std::endl;
            Tee<<best_score<<std::endl;
        });
    }
    store_data(mp,key,best_score);
    return best_score;
}
}

#endif