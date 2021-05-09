
#include <array>
#include <cstdint>
#include <vector>

#define STANDARD

#define BOARD_ROWS 15

#define BOARD_SIZE BOARD_ROWS*BOARD_ROWS

namespace reasoner
{
constexpr int NUMBER_OF_PLAYERS   = 3;
constexpr int MONOTONIC_CLASSES   = 0;
constexpr int NUMBER_OF_VARIABLES = 2;
constexpr int NUMBER_OF_PIECES    = 2;
constexpr uint64_t bit_mask = (1 << 6) -1;
class resettable_bitarray_stack {};

struct move
{
  uint32_t mr;

  move(void) = default;
  move(uint32_t _mr) : mr(_mr) {};
  bool operator==(const move& rhs) const;
};

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
  bool win_condition();

private:
  board pieces[2] = {0};

  uint32_t current_player = 1;
  uint32_t last_moved     = 0;

  bool exit = false;

  uint32_t variables[NUMBER_OF_VARIABLES] = {50, 50};

  constexpr std::array<move, BOARD_SIZE> fill_index_to_mask()
  {
    std::array<move, BOARD_SIZE> result{};

    for (uint32_t i=0;i<BOARD_SIZE;i++)
    {
      result[i] = i;
    }

    return result;
  }

  const std::array<move, BOARD_SIZE> empty = fill_index_to_mask();
};
}