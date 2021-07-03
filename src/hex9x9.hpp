
#include <array>
#include <cstdint>
#include <vector>

#define STANDARD

#define BOARD_ROWS 9

#define BOARD_SIZE BOARD_ROWS * BOARD_ROWS

namespace reasoner
{
constexpr int NUMBER_OF_PLAYERS   = 3;
constexpr int MONOTONIC_CLASSES   = 0;
constexpr int NUMBER_OF_VARIABLES = 2;
constexpr int NUMBER_OF_PIECES    = 2;
constexpr uint32_t bitmask = (1 << 9);
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

static constexpr int16_t fill_mask(uint32_t mask, uint32_t p)
{
  bool in = 0;

  for (int j=0;j<9;j++)
  {
    if (mask & ((uint32_t)1 << j))
    {
      in = 1;
    }
    else if ((p & ((uint32_t)1 << j)) == (uint32_t)0)
    {
      in = 0;
    }

    if (in)
    {
      mask |= ((uint32_t)1 << j);
    }
  }

  in = 0;

  for(int j=8;j>=0;j--)
  {
    if (mask & ((uint32_t)1 << j))
    {
      in = 1;
    }
    else if ((p & ((uint32_t)1 << j)) == (uint32_t)0)
    {
      in = 0;
    }

    if (in)
    {
      mask |= ((uint32_t)1 << j);
    }
  }

  return mask;
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

  bool win_condition_left_right();
  bool win_condition_up_down();

  game_state()
  {
    empty.insert(empty.begin(), std::begin(empty_array), std::end(empty_array));
  }

private:
  uint32_t up_down[BOARD_ROWS]    = {0};
  uint32_t left_right[BOARD_ROWS] = {0};

  uint32_t current_player = 1;
  uint32_t last_moved     = 0;
  uint32_t last_pos       = 100;

  bool exit = false;

  uint32_t variables[NUMBER_OF_VARIABLES] = {50, 50};

  const std::array<move, BOARD_SIZE> empty_array = fill_empty();
  std::vector<move> empty;

  static constexpr std::array<uint16_t, bitmask*bitmask> mask_to_mask = []()
  {
    std::array<uint16_t, bitmask*bitmask> tmp = {};

    uint32_t mask = 0;

    for (uint32_t i=0;i<bitmask;i++)
    {
      for (uint32_t j=0;j<bitmask;j++)
      {
        mask = i;
        mask = (mask & j) | (((mask << (uint32_t)1) & (uint32_t)0b111111111) & j);
        mask = fill_mask(mask, j);
        tmp[(i << 9) + j] = mask;
      }
    }

    return tmp;
  }();
};
}