
#include <array>
#include <cstdint>
#include <vector>

// #undef MONOTONIC

namespace reasoner
{
//
// BOARD_ROWS can by any positive integer
//
constexpr int BOARD_ROWS  = 11;
constexpr int BOARD_SIZE  = BOARD_ROWS * BOARD_ROWS;
constexpr int BOARD_ROWS2 = BOARD_ROWS + 2;
constexpr int BOARD_SIZE2 = BOARD_ROWS2 * BOARD_ROWS2;

constexpr int NUMBER_OF_PLAYERS   = 3;
#ifdef MONOTONIC
constexpr int MONOTONIC_CLASSES   = 1;
#else
constexpr int MONOTONIC_CLASSES   = 0;
#endif
constexpr int NUMBER_OF_VARIABLES = 2;
constexpr int NUMBER_OF_PIECES    = 2;
constexpr uint64_t bit_mask = (1 << 6) -1;
class resettable_bitarray_stack {};

template<uint32_t X> struct subset
{
  uint64_t date[X] = {0};
  int N = X;

  __attribute__((hot)) inline uint64_t get(uint32_t x) const
  {
    return (date[x >> 6] & ((uint64_t)1 << (x & bit_mask)));
  }

  __attribute__((hot)) constexpr inline void set(uint32_t x)
  {
    date[x>> 6] |= ((uint64_t)1 << (x & bit_mask));
  }

  __attribute__((hot)) inline void unset(uint32_t x)
  {
    date[x>> 6] ^= ((uint64_t)1 << (x & bit_mask));
  }
};

typedef subset<(BOARD_SIZE2 >> 6)+1> board;
struct move
{
  uint32_t mr;
  #ifndef MONOTONIC
  uint32_t pos;
  #endif

  move(void) = default;
  #ifndef MONOTONIC
  constexpr move(uint32_t _mr, uint32_t _pos) : mr(_mr), pos(_pos) {};
  #else
  constexpr move(uint32_t _mr) : mr(_mr) {};
  #endif

  bool operator==(const move& rhs) const;
};

static constexpr std::array<move, BOARD_SIZE> fill_empty()
{
  std::array<move, BOARD_SIZE> result{};

  int cnt = 0;

  for (uint32_t i=0;i<BOARD_SIZE2;i++)
  {
    if (i%BOARD_ROWS2 != 0 && i%BOARD_ROWS2 != BOARD_ROWS2-1 &&
          i/BOARD_ROWS2 != 0 && i/BOARD_ROWS2 != BOARD_ROWS2-1)
    {
      #ifndef MONOTONIC
      result[cnt] = move(i, cnt);
      #else
      result[cnt] = move(i);
      #endif
      cnt++;
    }
  }

  return result;
}

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

  game_state()
  {
    empty.insert(empty.begin(), std::begin(empty_array), std::end(empty_array));
  }

private:
  uint32_t current_player = 1;
  uint32_t last_moved     = 0;
  uint32_t last_pos       = BOARD_SIZE2 + 1;
  uint32_t vis_list_cnt   = 0;

  board redBoard;
  board blueBoard;

  bool exit       = false;
  bool firstEdge  = false;
  bool secondEdge = false;

  uint32_t variables[NUMBER_OF_VARIABLES] = {50, 50};

  static constexpr std::array<move, BOARD_SIZE> empty_array = fill_empty();

  std::vector<move> empty;

  uint32_t vis_list[BOARD_SIZE2];

  bool vis[BOARD_SIZE2] = {false};

  void win_condition_blue(uint32_t m);
  void win_condition_red(uint32_t m);
};
}