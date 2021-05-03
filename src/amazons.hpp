
#include <cstdint>
#include <vector>

namespace reasoner
{
constexpr int NUMBER_OF_PLAYERS   = 3;
constexpr int MONOTONIC_CLASSES   = 0;
constexpr int NUMBER_OF_VARIABLES = 2;
constexpr int NUMBER_OF_PIECES    = 2;

class resettable_bitarray_stack {};

struct move
{
  uint32_t cnt,x1,y1,x2,y2;

  move(void) = default;
  move(uint32_t _cnt, uint32_t _x1, uint32_t _y1, uint32_t _x2, uint32_t _y2 ) :
    cnt(_cnt), x1(_x1), y1(_y1), x2(_x2), y2(_y2) {};
  bool operator==(const move& rhs) const;
};

class game_state
{
public:
  int get_current_player(void) const;
  int get_player_score(int player_id) const;
  int get_monotonicity_class(void);

  void apply_move(const move& m);
  void get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves);

  bool apply_any_move(resettable_bitarray_stack&);
  bool is_legal(const move& m) const;

private:
  void get_moves(uint32_t pice, uint32_t idx, std::vector<move>& moves);
  void get_moves(uint32_t y, uint32_t x, uint32_t idx, uint32_t x1, uint32_t y1, std::vector<move>& moves);

  __uint128_t empty = ((__uint128_t)4831838240 << 64) +  (__uint128_t)1152922055436402760;

  uint32_t array_cnt      = 0;
  uint32_t current_player = 1;

  uint32_t variables[NUMBER_OF_VARIABLES] = {50, 50};

  uint32_t pieces[2][4] = {{3, 6, 30, 39}, {60, 69, 93, 96}};

  uint32_t array_pos[500]     = {0};
  uint32_t array_idx[500]     = {0};
  __uint128_t array_mask[500] = {0};

  uint32_t index_to_pos[10][10] =
    {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
    {10, 11, 12, 13, 14, 15, 16, 17, 18, 19},
    {20, 21, 22, 23, 24, 25, 26, 27, 28, 29},
    {30, 31, 32, 33, 34, 35, 36, 37, 38, 39},
    {40, 41, 42, 43, 44, 45, 46, 47, 48, 49},
    {50, 51, 52, 53, 54, 55, 56, 57, 58, 59},
    {60, 61, 62, 63, 64, 65, 66, 67, 68, 69},
    {70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
    {80, 81, 82, 83, 84, 85, 86, 87, 88, 89},
    {90, 91, 92, 93, 94, 95, 96, 97, 98, 99}};

  uint32_t pos_to_index[100][2] =
    {{0,0}    ,{0,1}    ,{0,2}    ,{0,3}
    ,{0,4}    ,{0,5}    ,{0,6}    ,{0,7}
    ,{0,8}    ,{0,9}    ,{1,0}    ,{1,1}
    ,{1,2}    ,{1,3}    ,{1,4}    ,{1,5}
    ,{1,6}    ,{1,7}    ,{1,8}    ,{1,9}
    ,{2,0}    ,{2,1}    ,{2,2}    ,{2,3}
    ,{2,4}    ,{2,5}    ,{2,6}    ,{2,7}
    ,{2,8}    ,{2,9}    ,{3,0}    ,{3,1}
    ,{3,2}    ,{3,3}    ,{3,4}    ,{3,5}
    ,{3,6}    ,{3,7}    ,{3,8}    ,{3,9}
    ,{4,0}    ,{4,1}    ,{4,2}    ,{4,3}
    ,{4,4}    ,{4,5}    ,{4,6}    ,{4,7}
    ,{4,8}    ,{4,9}    ,{5,0}    ,{5,1}
    ,{5,2}    ,{5,3}    ,{5,4}    ,{5,5}
    ,{5,6}    ,{5,7}    ,{5,8}    ,{5,9}
    ,{6,0}    ,{6,1}    ,{6,2}    ,{6,3}
    ,{6,4}    ,{6,5}    ,{6,6}    ,{6,7}
    ,{6,8}    ,{6,9}    ,{7,0}    ,{7,1}
    ,{7,2}    ,{7,3}    ,{7,4}    ,{7,5}
    ,{7,6}    ,{7,7}    ,{7,8}    ,{7,9}
    ,{8,0}    ,{8,1}    ,{8,2}    ,{8,3}
    ,{8,4}    ,{8,5}    ,{8,6}    ,{8,7}
    ,{8,8}    ,{8,9}    ,{9,0}    ,{9,1}
    ,{9,2}    ,{9,3}    ,{9,4}    ,{9,5}
    ,{9,6}    ,{9,7}    ,{9,8}    ,{9,9}};

__uint128_t index_to_mask[10][10] =
  {{((__uint128_t)0 << 64) + (__uint128_t)1,
  ((__uint128_t)0 << 64) + (__uint128_t)2,
  ((__uint128_t)0 << 64) + (__uint128_t)4,
  ((__uint128_t)0 << 64) + (__uint128_t)8,
  ((__uint128_t)0 << 64) + (__uint128_t)16,
  ((__uint128_t)0 << 64) + (__uint128_t)32,
  ((__uint128_t)0 << 64) + (__uint128_t)64,
  ((__uint128_t)0 << 64) + (__uint128_t)128,
  ((__uint128_t)0 << 64) + (__uint128_t)256,
  ((__uint128_t)0 << 64) + (__uint128_t)512},
  {((__uint128_t)0 << 64) + (__uint128_t)1024,
  ((__uint128_t)0 << 64) + (__uint128_t)2048,
  ((__uint128_t)0 << 64) + (__uint128_t)4096,
  ((__uint128_t)0 << 64) + (__uint128_t)8192,
  ((__uint128_t)0 << 64) + (__uint128_t)16384,
  ((__uint128_t)0 << 64) + (__uint128_t)32768,
  ((__uint128_t)0 << 64) + (__uint128_t)65536,
  ((__uint128_t)0 << 64) + (__uint128_t)131072,
  ((__uint128_t)0 << 64) + (__uint128_t)262144,
  ((__uint128_t)0 << 64) + (__uint128_t)524288},
  {((__uint128_t)0 << 64) + (__uint128_t)1048576,
  ((__uint128_t)0 << 64) + (__uint128_t)2097152,
  ((__uint128_t)0 << 64) + (__uint128_t)4194304,
  ((__uint128_t)0 << 64) + (__uint128_t)8388608,
  ((__uint128_t)0 << 64) + (__uint128_t)16777216,
  ((__uint128_t)0 << 64) + (__uint128_t)33554432,
  ((__uint128_t)0 << 64) + (__uint128_t)67108864,
  ((__uint128_t)0 << 64) + (__uint128_t)134217728,
  ((__uint128_t)0 << 64) + (__uint128_t)268435456,
  ((__uint128_t)0 << 64) + (__uint128_t)536870912},
  {((__uint128_t)0 << 64) + (__uint128_t)1073741824,
  ((__uint128_t)0 << 64) + (__uint128_t)2147483648,
  ((__uint128_t)0 << 64) + (__uint128_t)4294967296,
  ((__uint128_t)0 << 64) + (__uint128_t)8589934592,
  ((__uint128_t)0 << 64) + (__uint128_t)17179869184,
  ((__uint128_t)0 << 64) + (__uint128_t)34359738368,
  ((__uint128_t)0 << 64) + (__uint128_t)68719476736,
  ((__uint128_t)0 << 64) + (__uint128_t)137438953472,
  ((__uint128_t)0 << 64) + (__uint128_t)274877906944,
  ((__uint128_t)0 << 64) + (__uint128_t)549755813888},
  {((__uint128_t)0 << 64) + (__uint128_t)1099511627776,
  ((__uint128_t)0 << 64) + (__uint128_t)2199023255552,
  ((__uint128_t)0 << 64) + (__uint128_t)4398046511104,
  ((__uint128_t)0 << 64) + (__uint128_t)8796093022208,
  ((__uint128_t)0 << 64) + (__uint128_t)17592186044416,
  ((__uint128_t)0 << 64) + (__uint128_t)35184372088832,
  ((__uint128_t)0 << 64) + (__uint128_t)70368744177664,
  ((__uint128_t)0 << 64) + (__uint128_t)140737488355328,
  ((__uint128_t)0 << 64) + (__uint128_t)281474976710656,
  ((__uint128_t)0 << 64) + (__uint128_t)562949953421312},
  {((__uint128_t)0 << 64) + (__uint128_t)1125899906842624,
  ((__uint128_t)0 << 64) + (__uint128_t)2251799813685248,
  ((__uint128_t)0 << 64) + (__uint128_t)4503599627370496,
  ((__uint128_t)0 << 64) + (__uint128_t)9007199254740992,
  ((__uint128_t)0 << 64) + (__uint128_t)18014398509481984,
  ((__uint128_t)0 << 64) + (__uint128_t)36028797018963968,
  ((__uint128_t)0 << 64) + (__uint128_t)72057594037927936,
  ((__uint128_t)0 << 64) + (__uint128_t)144115188075855872,
  ((__uint128_t)0 << 64) + (__uint128_t)288230376151711744,
  ((__uint128_t)0 << 64) + (__uint128_t)576460752303423488},
  {((__uint128_t)0 << 64) + (__uint128_t)1152921504606846976,
  ((__uint128_t)0 << 64) + (__uint128_t)2305843009213693952,
  ((__uint128_t)0 << 64) + (__uint128_t)4611686018427387904,
  ((__uint128_t)0 << 64) + (__uint128_t)9223372036854775808U,
  ((__uint128_t)1 << 64) + (__uint128_t)0,
  ((__uint128_t)2 << 64) + (__uint128_t)0,
  ((__uint128_t)4 << 64) + (__uint128_t)0,
  ((__uint128_t)8 << 64) + (__uint128_t)0,
  ((__uint128_t)16 << 64) + (__uint128_t)0,
  ((__uint128_t)32 << 64) + (__uint128_t)0},
  {((__uint128_t)64 << 64) + (__uint128_t)0,
  ((__uint128_t)128 << 64) + (__uint128_t)0,
  ((__uint128_t)256 << 64) + (__uint128_t)0,
  ((__uint128_t)512 << 64) + (__uint128_t)0,
  ((__uint128_t)1024 << 64) + (__uint128_t)0,
  ((__uint128_t)2048 << 64) + (__uint128_t)0,
  ((__uint128_t)4096 << 64) + (__uint128_t)0,
  ((__uint128_t)8192 << 64) + (__uint128_t)0,
  ((__uint128_t)16384 << 64) + (__uint128_t)0,
  ((__uint128_t)32768 << 64) + (__uint128_t)0},
  {((__uint128_t)65536 << 64) + (__uint128_t)0,
  ((__uint128_t)131072 << 64) + (__uint128_t)0,
  ((__uint128_t)262144 << 64) + (__uint128_t)0,
  ((__uint128_t)524288 << 64) + (__uint128_t)0,
  ((__uint128_t)1048576 << 64) + (__uint128_t)0,
  ((__uint128_t)2097152 << 64) + (__uint128_t)0,
  ((__uint128_t)4194304 << 64) + (__uint128_t)0,
  ((__uint128_t)8388608 << 64) + (__uint128_t)0,
  ((__uint128_t)16777216 << 64) + (__uint128_t)0,
  ((__uint128_t)33554432 << 64) + (__uint128_t)0},
  {((__uint128_t)67108864 << 64) + (__uint128_t)0,
  ((__uint128_t)134217728 << 64) + (__uint128_t)0,
  ((__uint128_t)268435456 << 64) + (__uint128_t)0,
  ((__uint128_t)536870912 << 64) + (__uint128_t)0,
  ((__uint128_t)1073741824 << 64) + (__uint128_t)0,
  ((__uint128_t)2147483648 << 64) + (__uint128_t)0,
  ((__uint128_t)4294967296 << 64) + (__uint128_t)0,
  ((__uint128_t)8589934592 << 64) + (__uint128_t)0,
  ((__uint128_t)17179869184 << 64) + (__uint128_t)0,
  ((__uint128_t)34359738368 << 64) + (__uint128_t)0}};
};
}