
#include <array>
#include <cstdint>
#include <vector>

#define STANDARD

#define BOARD_ROWS 9

#define BOARD_SIZE BOARD_ROWS*BOARD_ROWS

#include <iostream>

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

constexpr std::array<move, BOARD_SIZE> fill_empty()
{
  std::array<move, BOARD_SIZE> result{};

  for (uint32_t i=0;i<BOARD_SIZE;i++)
  {
    result[i] = move(i,i);
  }

  return result;
}

typedef __uint128_t board;

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

  void fill_mask(uint32_t &mask, uint32_t tab);


private:
  // board pieces[2] =  {(__uint128_t)0, (__uint128_t)0};

  uint32_t up_down[BOARD_ROWS]    = {0};
  uint32_t left_right[BOARD_ROWS] = {0};


  uint32_t current_player = 1;
  uint32_t last_moved     = 0;
  uint32_t last_pos       = 0;

  bool exit = false;

  // void print(uint32_t mask, int x = 0)
  // {
  //   for (int i=0;i<(9-x);i++)
  //   std::cout << " ";
  //   for (int i=0;i<9;i++)
  //   {

  //     if(mask & ((uint32_t)1 << i))
  //     std::cout << "1";
  //     else
  //     std::cout << "0";
  //   }
  //       std::cout << "\n";
  // }

  //   void print2(uint32_t *tab)
  // {
  //   for (int i=0;i<9;i++)
  //   {
  //     print(tab[i], i);
  //   }
  //   std::cout << "\n\n";
  // }

  uint32_t variables[NUMBER_OF_VARIABLES] = {50, 50};

  const std::array<move, BOARD_SIZE> empty = fill_empty();
};
}