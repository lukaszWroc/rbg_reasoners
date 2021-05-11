
#include <array>
#include <cstdint>
#include <vector>

#undef STANDARD

#define BOARD_ROWS 6

#define BOARD_SIZE BOARD_ROWS*BOARD_ROWS

namespace reasoner
{
constexpr int NUMBER_OF_PLAYERS   = 3;
constexpr int MONOTONIC_CLASSES   = 0;
constexpr int NUMBER_OF_VARIABLES = 2;
constexpr int NUMBER_OF_PIECES    = 2;

class resettable_bitarray_stack {};

struct move
{
  uint32_t mr, pos;

  move(void) = default;
  constexpr move(uint32_t _mr, uint32_t _pos) : mr(_mr), pos(_pos) {};
  bool operator==(const move& rhs) const;
};

constexpr std::array<move, BOARD_SIZE> fill_empty_array()
{
  std::array<move, BOARD_SIZE> result{};

  for (uint32_t i=0;i<BOARD_SIZE;i++)
  {
    result[i] = move(i,i);
  }

  return result;
}

typedef uint64_t board;

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
  bool win_condition(uint32_t pl);
  bool rotation_win(uint32_t x);

private:
  board pieces[2] = {0};

  bool piece_move = true;
  bool played     = false;
  bool exit       = false;

  uint32_t winner         = 1;
  uint32_t current_player = 1;
  uint32_t last_moved     = 0;
  uint32_t last_pos       = 0;

  int rotates_cnt[4] = {0,0,0,0};

  uint32_t index_pos[4][9] =
    {{0,1,2,6,7,8,12,13,14}, {3,4,5,9,10,11,15,16,17},
    {18,19,20,24,25,26,30,31,32}, {21,22,23,27,28,29,33,34,35}};

  uint32_t rotations[4] = {0,0,0,0};
  uint32_t variables[NUMBER_OF_VARIABLES] = {50, 50};

  void rotate(int x, uint32_t pos, uint32_t type);
  void rotate_right(bool *tab);
  void rotate_left(bool *tab);
  void rotate_right(uint32_t *tab);
  void rotate_left(uint32_t *tab);
  void rotate_all(uint32_t idx, uint32_t type);

  int get_quarter(uint32_t pos);
  uint32_t get_pos(uint32_t pos);

  static constexpr std::array<move, BOARD_SIZE> empty_array = fill_empty_array();
  static constexpr std::array<move, 8> rotates_vec = {move(0,0), move(0,1), move(1,0), move(1,1), move(2,0), move(2,1), move(3,0), move(3,1)};
  std::vector<move> empty;
};
}