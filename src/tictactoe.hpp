
#include <array>
#include <cstdint>
#include <vector>

#define BOARD_SIZE 9

#undef MONOTONIC

namespace reasoner
{
constexpr int NUMBER_OF_PLAYERS   = 3;
#ifdef MONOTONIC
constexpr int MONOTONIC_CLASSES   = 1;
#else
constexpr int MONOTONIC_CLASSES   = 0;
#endif
constexpr int NUMBER_OF_VARIABLES = 2;
constexpr int NUMBER_OF_PIECES    = 2;

class resettable_bitarray_stack {};

typedef uint32_t move_representation;

struct move
{
  move_representation mr;

  #ifndef MONOTONIC
  move_representation pos;
  #endif

  move(void) = default;
  #ifdef MONOTONIC
  constexpr move(const move_representation& mv) : mr(mv) {};
  #else
  constexpr move(const move_representation& mv, const move_representation& _pos) : mr(mv), pos(_pos) {};
  #endif
  bool operator==(const move& rhs) const;
};

class game_state
{
  int current_player = 1;

  static constexpr int not_started =  1 << (BOARD_SIZE + 1);
  int exit = 0;
  int last_pos = not_started;

  int variables[NUMBER_OF_VARIABLES] = {50, 50};

  std::vector<move> empty_vec;

  uint32_t pieces[2] = {0, 0};

  static constexpr std::array<bool, (1 << BOARD_SIZE)> win_arr = []()
  {
    std::array<bool, (1 << BOARD_SIZE)> result{};

    for (int i=0;i<(1<<BOARD_SIZE);i++)
    {
      result[i] = (i & (0b010010010)) == 0b010010010 ||
                    (i & 0b100100100) == 0b100100100 ||
                    (i & 0b001001001) == 0b001001001 ||
                    (i & 0b111000000) == 0b111000000 ||
                    (i & 0b000111000) == 0b000111000 ||
                    (i & 0b000000111) == 0b000000111 ||
                    (i & 0b100010001) == 0b100010001 ||
                    (i & 0b001010100) == 0b001010100;
    }

    return result;
  }();

  static constexpr std::array<move, BOARD_SIZE> empty_arr = []()
  {
    std::array<move, BOARD_SIZE> result{};

    for (uint32_t i=0;i<BOARD_SIZE;i++)
    {
      #ifdef MONOTONIC
      result[i] = move(1<<i);
      #else
      result[i] = move(1<<i, i);
      #endif
    }

    return result;
  }();

public:
  int get_current_player(void) const;
  int get_player_score(int player_id) const;
  void apply_move(const move& m);
  void get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves);
  bool apply_any_move(resettable_bitarray_stack&);
  int get_monotonicity_class(void);
  bool is_legal(const move& m) const;
};
}