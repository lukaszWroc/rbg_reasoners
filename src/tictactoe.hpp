
#include <cstdint>
#include <vector>

namespace reasoner
{
constexpr int NUMBER_OF_PLAYERS   = 3;
constexpr int MONOTONIC_CLASSES   = 0;
constexpr int NUMBER_OF_VARIABLES = 2;
constexpr int NUMBER_OF_PIECES    = 2;

class resettable_bitarray_stack {};

typedef uint32_t move_representation;

struct move
{
  move_representation mr;
  move(void) = default;
  move(const move_representation& mv) : mr(mv) {};
  bool operator==(const move& rhs) const;
};

class game_state
{
  int current_player = 1;

  int exit_ = 0;

  int variables[NUMBER_OF_VARIABLES] = {50, 50};

  uint32_t pieces[2] = {0, 0};

  inline uint32_t msb(const uint32_t& pieces) const 
  {
    auto k = 31 - __builtin_clz(pieces);
    return 1u << k;
  }

  bool win_condition();

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