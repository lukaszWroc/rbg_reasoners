
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

static constexpr std::array<uint32_t, BOARD_SIZE2> fill_vis()
{
  std::array<uint32_t, BOARD_SIZE2> result{};


  for (uint32_t i=0;i<BOARD_SIZE2;i++)
  {
    result[i] = 0;
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

  game_state() : vis(vis_ini), leader(leader_init), range(range_init),
    firstEdgeArray(edgeArray_init), secondEdgeArray(edgeArray_init)
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

  static constexpr std::array<uint32_t, BOARD_SIZE2> vis_ini = fill_vis();

  std::array<uint32_t, BOARD_SIZE2> vis;

  std::array<uint32_t, BOARD_SIZE2> leader;
  std::array<uint32_t, BOARD_SIZE2> range;
  std::array<board, BOARD_SIZE2> taken_board;
  std::array<bool, BOARD_SIZE2> firstEdgeArray;
  std::array<bool, BOARD_SIZE2> secondEdgeArray;

  void win_condition_blue(uint32_t m);
  void win_condition_red(uint32_t m);

  inline void fix_win_condition(uint32_t pos, uint32_t &last_leader);

  inline void union1(uint32_t a, uint32_t b)
  {
    leader[a] = b;
    range[b] += range[a];
  }

  inline uint32_t find(uint32_t a)
  {
    if(a == leader[a])
        return a;
    leader[a] = find(leader[a]);
    return leader[a];
  }

  inline void merge1(board &right, board &left)
  {
    for(int i=0;i<right.N;i++)
    {
      right.date[i] |= left.date[i];
    }
  }

  static constexpr std::array<uint32_t, BOARD_SIZE2> range_init = []()
  {
    std::array<uint32_t, BOARD_SIZE2> tmp = {};

    for (int i=0;i<BOARD_SIZE2;i++)
    {
      tmp[i]=1;
    }
    return tmp;
  }();

  static constexpr std::array<uint32_t, BOARD_SIZE2> leader_init = []()
  {
    std::array<uint32_t, BOARD_SIZE2> tmp = {};

    for (uint32_t i=0;i<BOARD_SIZE2;i++)
    {
      tmp[i] = i;
    }

    return tmp;
  }();

  static constexpr std::array<bool, BOARD_SIZE2> edgeArray_init = []()
  {
    std::array<bool, BOARD_SIZE2> tmp = {};

    for (uint32_t i=0;i<BOARD_SIZE2;i++)
    {
      tmp[i] = false;
    }

    return tmp;
  }();
};
}