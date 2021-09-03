#include <array>
#include <cstdint>
#include <vector>

namespace reasoner
{
//
// BOARD_ROWS can't be changed
//
constexpr int BOARD_ROWS = 6;
constexpr int BOARD_SIZE = BOARD_ROWS * BOARD_ROWS;

constexpr int NUMBER_OF_PLAYERS   = 3;
constexpr int MONOTONIC_CLASSES   = 0;
constexpr int NUMBER_OF_VARIABLES = 2;
constexpr int NUMBER_OF_PIECES    = 2;

class resettable_bitarray_stack {};

struct move
{
  uint32_t mr;
  uint32_t pos;

  move(void) = default;
  constexpr move(uint32_t _mr, uint32_t _pos = 0) : mr(_mr), pos(_pos) {};
  bool operator==(const move& rhs) const;
};

typedef uint64_t board;

struct rotation
{
  static constexpr int mask = (1 << 9);

  static constexpr std::array<uint64_t, mask> rotate_arr(std::array<int, 9> t)
  {
    std::array<uint64_t, mask> result{};

    bool tab[9] = {};

    for (int k=0;k<mask;k++)
    {
      for (int j=0;j<9;j++)
      {
        if ((1 << j) & k)
        {
          tab[j] = 1;
        }
        else
        {
          tab[j] = 0;
        }
      }

      uint64_t wyn = 0;

      int d=0;

      for(int i=0;i<9;i++)
      {
        if (tab[t[i]])
        {
          wyn |= ((uint64_t)1<<(i+d));
        }

        if (i==2 || i == 5)
        {
          d += 3;
        }
      }

      result[k] = wyn;
    }

    return result;
  }
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
  board pieces[2] = {0, 0};

  int diff[4] {0,3,18,21};

  bool piece_move = true;
  bool exit       = false;

  uint32_t current_player = 1;

  uint32_t variables[NUMBER_OF_VARIABLES] = {50, 50};

  bool win_condition(uint32_t pl);
  bool rotation_win();

  void rotate(int x, uint32_t pos, uint32_t type);
  void rotate_all(uint32_t idx, uint32_t type);

  inline uint32_t msb(const uint64_t p)
  {
    return (63 - __builtin_clzl(p));
  }

  __attribute__((hot)) uint64_t inline transform(uint64_t x)
  {
    return ((x & 0b111) | ((x & 0b111000000) >> 3) | ((x & 0b111000000000000) >> 6));
  }

  static constexpr std::array<move, 8> rotates_vec = {move(0,0), move(0,1), move(1,0), move(1,1), move(2,0), move(2,1), move(3,0), move(3,1)};

  static constexpr uint64_t quatter[4] =
    {
      0b000111000111000111,
      0b111000111000111000,
      0b000111000111000111000000000000000000,
      0b111000111000111000000000000000000000
    };
    static constexpr int mask = (1 << 9);


  static constexpr std::array<uint64_t, rotation::mask> rotate_right_arr = rotation::rotate_arr({6,3,0,7,4,1,8,5,2});
  static constexpr std::array<uint64_t, rotation::mask> rotate_left_arr  = rotation::rotate_arr({2,5,8,1,4,7,0,3,6});

  static constexpr std::array<uint64_t, 32> win_arr = []()
  {
    std::array<uint64_t, 32> result{};

    result[0] = 0b000000010000001000000100000010000001;
    result[1] = 0b100000010000001000000100000010000000;
    result[2] = 0b000000100000010000001000000100000010;
    result[3] = 0b010000001000000100000010000001000000;
    result[4] = 0b000000000010000100001000010000100000;
    result[5] = 0b000001000010000100001000010000000000;
    result[6] = 0b000010000100001000010000100000000000;
    result[7] = 0b000000000001000010000100001000010000;

    uint64_t tmp = 0b000000100000100000100000100000100000;

    for (int i=0;i<6;i++)
    {
      result[i+8] = (tmp >> i);
    }

    tmp = 0b100000100000100000100000100000000000;

    for (int i=0;i<6;i++)
    {
      result[i+14] = (tmp >> i);
    }

    tmp = 0b000000000000000000000000000000111110;

    for (int i=0;i<6;i++)
    {
      result[i+20] = (tmp << (i*6));
    }

    tmp = 0b000000000000000000000000000000011111;

    for (int i=0;i<6;i++)
    {
      result[i+26] = (tmp << (i*6));
    }

    return result;
  }();
};
}