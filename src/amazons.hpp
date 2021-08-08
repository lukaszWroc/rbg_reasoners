
#include <array>
#include <cstdint>
#include <vector>

#define BOARD_ROWS 10
#define BOARD_SIZE BOARD_ROWS*BOARD_ROWS

namespace reasoner
{
constexpr int NUMBER_OF_PLAYERS   = 3;
constexpr int MONOTONIC_CLASSES   = 0;
constexpr int NUMBER_OF_VARIABLES = 2;
constexpr int NUMBER_OF_PIECES    = 2;

class resettable_bitarray_stack {};

typedef __int128_t board;

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
  void get_moves(uint32_t piece, uint32_t idx, std::vector<move>& moves);
  void get_moves(uint32_t y, uint32_t x, uint32_t idx, uint32_t x1, uint32_t y1, std::vector<move>& moves);

  board empty = ((board)4831838240 << 64) +  (board)1152922055436402760;
  uint32_t pieces[2][4] = {{3, 6, 30, 39}, {60, 69, 93, 96}};

  uint32_t array_cnt      = 0;
  uint32_t current_player = 1;

  uint32_t variables[NUMBER_OF_VARIABLES] = {50, 50};

  uint32_t array_pos[500] = {0};
  uint32_t array_idx[500] = {0};
  board array_mask[500]   = {0};

  constexpr std::array<board, BOARD_SIZE> fill_index_to_mask()
  {
    std::array<board, BOARD_SIZE> result{};

    for (int i=0;i<BOARD_SIZE;i++)
    {
      result[i] = (board)1 << i;
    }

    return result;
  }

  const std::array<board, BOARD_SIZE> index_to_mask = fill_index_to_mask();
};
}