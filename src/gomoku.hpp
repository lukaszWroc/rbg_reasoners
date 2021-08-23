
#include <array>
#include <cstdint>
#include <vector>

#define STANDARD

#define BOARD_ROWS 15

#define BOARD_SIZE BOARD_ROWS*BOARD_ROWS

// #undef MONOTONIC

namespace reasoner
{
constexpr int NUMBER_OF_PLAYERS   = 3;
#ifdef MONOTONIC
constexpr int MONOTONIC_CLASSES   = 2;
#else
constexpr int MONOTONIC_CLASSES   = 0;
#endif

constexpr int NUMBER_OF_VARIABLES = 2;
constexpr int NUMBER_OF_PIECES    = 2;
constexpr uint64_t bit_mask = (1 << 6) -1;
class resettable_bitarray_stack {};

struct move
{
  uint32_t mr, pos;

  move(void) = default;
  constexpr move(uint32_t _mr, uint32_t _pos) : mr(_mr), pos(_pos) {};
  bool operator==(const move& rhs) const;
};

constexpr std::array<move, BOARD_SIZE> fill_empty()
{
  std::array<move, BOARD_SIZE> result{};

  for (uint32_t i=0;i<BOARD_SIZE;i++)
  {
    result[i] = move(i,i);
  }

  return result;
}

template<uint32_t N> struct subset
{
  uint64_t date[N] = {0};

  __attribute__((hot)) inline uint64_t get(uint32_t x)
  {
    return (date[x >> 6] & ((uint64_t)1 << (x & bit_mask)));
  }

  __attribute__((hot)) inline void set(uint32_t x)
  {
    date[x>> 6] |= ((uint64_t)1 << (x & bit_mask));
  }
};

typedef subset<(BOARD_SIZE >> 6)+1> board;

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
  board pieces[2] = {0};

  uint32_t current_player = 1;

  uint32_t last_moved     = 0;
  #ifndef MONOTONIC
  int last_pos       = 0;
  #endif

  bool exit = false;

  uint32_t variables[NUMBER_OF_VARIABLES] = {50, 50};
  #ifdef MONOTONIC
  std::vector<bool>not_visited;
  #else
  std::vector<move> org_moves;
  std::vector<int> position;
  #endif

  bool win_condition();

  static constexpr std::array<move, BOARD_SIZE> empty = fill_empty();
  static constexpr std::array<int, BOARD_SIZE> position_tmp = []() constexpr
  {
    std::array<int, BOARD_SIZE> tmp ={};
    for (int i=0;i<BOARD_SIZE;i++)
    {
      tmp[i] = i;
    }
    return tmp;
  }();
};
}