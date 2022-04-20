#include <iostream>
#include "./../ai.hpp"

using namespace std;

int global_level = 5;
int ai_player = 0;

inline double calc_result_value(int v){
    return (double)v;
}

inline double output_coord(int policy, int raw_val){
    return 1000.0 * (double)(HW2_M1 - policy) + 100.0 + max(-64.0, min(64.0, calc_result_value(raw_val)));
}

int main(int argc, char *argv[]){
    bit_init();
    flip_init();
    board_init();
    evaluate_init();
    parent_transpose_table.first_init();
    child_transpose_table.first_init();
    cout << "AI initialized" << endl;
    return 0;
}

extern "C" void init_ai(int a_player, int level){
    ai_player = a_player;
    global_level = level;
    cout << "AI param " << ai_player << " " << global_level << endl;
}

extern "C" double ai_calc(int *arr_board){
    cout << "start AI" << endl;
    int i;
    Board board;
    Search_result result;
    board.player = 0ULL;
    board.opponent = 0ULL;
    for (i = 0; i < HW2; ++i){
        board.player |= (uint64_t)(arr_board[i] == 0) << (HW2_M1 - i);
        board.opponent |= (uint64_t)(arr_board[i] == 1) << (HW2_M1 - i);
    }
    if (ai_player)
        swap(board.player, board.opponent);
    board.n = pop_count_ull(board.player | board.opponent);
    board.p = ai_player;
    board.print();
    cout << board.n - 4 << "moves" << endl;
    result = ai(board, global_level);
    cout << result.value << " " << idx_to_coord(result.policy) << endl;
    cout << "policy " << result.policy << endl;
    double res = output_coord(result.policy, result.value);
    cout << "res " << res << endl;
    return res;
}

extern "C" void calc_value(int *arr_board, int e_count, int direction, int *res){
    cout << "start hint" << endl;
    int i;
    Board board;
    board.player = 0ULL;
    board.opponent = 0ULL;
    for (i = 0; i < HW2; ++i){
        board.player |= (uint64_t)(arr_board[i] == 0) << (HW2_M1 - i);
        board.opponent |= (uint64_t)(arr_board[i] == 1) << (HW2_M1 - i);
    }
    if (1 - ai_player)
        swap(board.player, board.opponent);
    board.n = pop_count_ull(board.player | board.opponent);
    board.p = 1 - ai_player;
    board.print();

    for (i = 0; i < HW2; ++i)
        res[10 + i] = -1;

    uint64_t legal = board.get_legal();
    Flip flip;
    bool is_mid_search;
    int depth;
    bool use_mpc;
    double mpct;
    int val;
    get_level(5, board.n - 4, &is_mid_search, &depth, &use_mpc, &mpct);
    for (uint_fast8_t cell = first_bit(&legal); legal; cell = next_bit(&legal)){
        calc_flip(&flip, &board, cell);
        board.move(&flip);
            val = (int)tree_search_noid(board, depth, use_mpc, mpct);
        board.undo(&flip);
        res[10 + HW2_M1 - cell] = -val;
    }

    for (int y = 0; y < HW; ++y){
        for (int x = 0; x < HW; ++x)
            cout << res[10 + y * HW + x] << " ";
        cout << endl;
    }
}