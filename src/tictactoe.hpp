
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
constexpr int NUMBER_OF_VARIABLES = 3;
constexpr int NUMBER_OF_PIECES    = 2;

class resettable_bitarray_stack {};

typedef uint32_t move_representation;

struct move
{
  move_representation mr;

  move(void) = default;
  constexpr move(const move_representation& mv) : mr(mv) {};
  bool operator==(const move& rhs) const;
};

class game_state
{
  int current_player = 1;

  int exit = 0;

  int variables[NUMBER_OF_VARIABLES] = {0, 50, 50};

  uint32_t pieces[3] = {0, 0, 0};

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

  static constexpr std::array<move, (1 << BOARD_SIZE) * BOARD_SIZE> all_moves = []()
  {
    std::array<move, (1 << BOARD_SIZE) * BOARD_SIZE> result{};

    for (uint32_t i=0;i<(1<<BOARD_SIZE);i++)
    {
      uint32_t x = ~i & 0b111111111;

      int cnt = 0;

      while (x)
      {
        uint32_t mv = (uint32_t)1 << (31 - __builtin_clz(x));
        result[i*BOARD_SIZE+cnt] = move(mv);
        x ^= mv;
        cnt++;
      }

      for (int j=cnt;j<BOARD_SIZE;j++)
      {
        result[i*BOARD_SIZE+j] = 0;
      }
    }

    return result;
  }();

  static constexpr std::array<uint32_t, (1 << BOARD_SIZE)> all_moves_cnt = []()
  {
    std::array<uint32_t, (1 << BOARD_SIZE)> result{};

    for (uint32_t i=0;i<(1<<BOARD_SIZE);i++)
    {
      uint32_t x = ~i & 0b111111111;

      int cnt = 0;

      while (x)
      {
        x ^= (uint32_t)1 << (31 - __builtin_clz(x));
        cnt++;
      }

      result[i] = cnt;
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