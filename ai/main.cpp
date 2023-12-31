#include "game.hpp"
#include "movelist.hpp"
#include "util.hpp"
#include "search.hpp"
#include "selfplay.hpp"
#include "ubfm.hpp"
#include "hash.hpp"
#include "nn.hpp"
#include "countreward.hpp"
#include "model.hpp"
#include "io.hpp"
#include "tt.hpp"
#include "oracle.hpp"
#include "hash.hpp"

TeeStream Tee;

namespace ubfm {
UBFMSearcherGlobal g_searcher_global;
}
namespace selfplay {
SelfPlayWorker g_selfplay_worker[SelfPlayWorker::NUM];
int g_thread_counter;
SelfPlayInfo g_selfplay_info;
}
namespace model {
GPUModel g_gpu_model[GPUModel::GPU_NUM];
}
namespace tt {
//TranspositionTable g_tt;
}
int main(int argc, char **argv){
    auto num = 999999999;
    if (argc > 1) {
        num = std::stoi(std::string(argv[1]));
    }
    check_mode();
    model::g_gpu_model[0].load_model(0);
    selfplay::execute_selfplay(num);
    //hash::test_hash();
    //gen::test_gen();
    //search::test_search2();
    //io::test_io();
    //oracle::check_all_pos();
    return 0;
}
