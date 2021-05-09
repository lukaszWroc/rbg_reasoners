
#include <array>
#include <cstdint>
#include <vector>

#define BOARD_ROWS 10

#if BOARD_ROWS * BOARD_ROWS > 128
  #undef BOARD_ROWS
  #define BOARD_ROWS 10
#endif

#define BOARD_SIZE BOARD_ROWS*BOARD_ROWS

namespace reasoner
{
constexpr int NUMBER_OF_PLAYERS   = 3;
constexpr int MONOTONIC_CLASSES   = 0;
constexpr int NUMBER_OF_VARIABLES = 2;
constexpr int NUMBER_OF_PIECES    = 2;

class resettable_bitarray_stack {};

#if BOARD_ROWS * BOARD_ROWS > 64
  typedef __int128_t board;
#elif BOARD_ROWS * BOARD_ROWS > 32
  typedef uint64_t board;
#elif BOARD_ROWS * BOARD_ROWS > 16
  typedef uint32_t board;
#endif

struct move
{
  uint32_t idx,x1,y1;

  move(void) = default;
  move(uint32_t _idx, uint32_t _x1, uint32_t _y1) :
    idx(_idx), x1(_x1), y1(_y1) {};
  move(uint32_t _x1, uint32_t _y1) :
    x1(_x1), y1(_y1) {};
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
  void get_moves_arrow(uint32_t piece, std::vector<move>& moves);
  void get_moves_piece(uint32_t piece, uint32_t idx, std::vector<move>& moves);

  /*** inicializajca wzgledem rozmiaru ********/
  board empty = ((board)4831838240 << 64) +  (board)1152922055436402760;
  uint32_t pieces[2][4] = {{3, 6, 30, 39}, {60, 69, 93, 96}};
  /******************************************/

  uint32_t current_player = 1;
  uint32_t last_moved     = 0;

  bool piece_move = true;

  uint32_t variables[NUMBER_OF_VARIABLES] = {50, 50};

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