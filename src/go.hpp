
#include <array>
#include <cstdint>
#include <vector>

#define BOARD_ROWS 19

namespace reasoner
{
constexpr int BOARD_SIZE = BOARD_ROWS * BOARD_ROWS;
constexpr int BLOCK      = BOARD_SIZE + 100;
constexpr int PASS       = BOARD_SIZE + 1;
constexpr int TURN_LIMIT =  (BOARD_SIZE * 2) + 1;

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
  constexpr move(uint32_t _mr) : mr(_mr) {};
  bool operator==(const move& rhs) const;
};

template<uint32_t X> struct subset
{
  uint64_t date[X] = {0};
  int N = X;

   __attribute__((hot)) inline uint64_t get(uint32_t x)
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

  game_state() : leader(leader_init), liberty(liberty_init), 
    liberty_board(libert_board_init), neighbors(neighbors_init), range(range_init)
  {}

private:
  uint32_t current_player = 1;
  uint32_t variables[NUMBER_OF_VARIABLES] = {0, 0};

  bool exit = false;

  std::array<uint32_t, BOARD_SIZE> leader;
  std::array<uint32_t, BOARD_SIZE> liberty;
  std::array<board, BOARD_SIZE> liberty_board;
  std::array<uint32_t, 4*BOARD_SIZE> neighbors;
  std::array<uint32_t, BOARD_SIZE> range;

  board blackBoard;
  board whiteBoard;

  int turn_limit = 0;
  bool passed[3] = {false, false, false};

  uint32_t fix_liberty(board &b);

  inline uint32_t count_pieces(board &b);
  inline void get_points();
  inline void prepare(std::vector<move>& moves, board &b, board &b2);
  inline void getFree(board &right, board &left, board &tmp);

  void update(board &my, board &other, const move &m);
  void dfs(int i, int j, board &b, board &cb);
  void delete1(uint32_t pos);

  int score(board &cb);

  inline void union1(uint32_t a, uint32_t b)
  {
    leader[a] = b;
    range[b] += range[a];
  }

  inline void merge1(board &right, board &left)
  {
    for(int i=0;i<right.N;i++)
    {
      right.date[i] |= left.date[i];
    }
  }

  inline void merge1(board &right, uint32_t pos)
  {
    right.set(pos);
  }

  inline uint32_t getPos(uint64_t mv, int i)
  {
    return i * 64 + (63 - __builtin_clzl(mv));
  }

  inline uint32_t find(uint32_t a)
  {
    if(a == leader[a])
        return a;
    leader[a] = find(leader[a]);
    return leader[a];
  }

  inline uint64_t msb(const uint64_t p)
  {
    return (uint64_t)1 << (63 - __builtin_clzl(p));
  }

  static constexpr std::array<uint32_t, BOARD_SIZE*4> neighbors_init = []()
  {
    std::array<uint32_t, BOARD_SIZE*4> tmp = {};

    for (int i=0;i<BOARD_ROWS;i++)
    {
      for (int j=0;j<BOARD_ROWS;j++)
      {
        int cnt = 0;

        if ((0 <= i && i < BOARD_ROWS) && (0 <=j-1 && j-1 <BOARD_ROWS))
        {
          tmp[((i*BOARD_ROWS+j)*4)+cnt] = i * BOARD_ROWS + j - 1;
          cnt++;
        }

        if ((0 <= i && i < BOARD_ROWS) && (0 <=j+1 && j+1 <BOARD_ROWS))
        {
          tmp[((i*BOARD_ROWS+j)*4)+cnt] = i * BOARD_ROWS + j + 1;
          cnt++;
        }

        if ((0 <= i-1 && i-1 < BOARD_ROWS) && (0 <=j && j <BOARD_ROWS))
        {
          tmp[((i*BOARD_ROWS+j)*4)+cnt] = (i-1) * BOARD_ROWS + j;
          cnt++;
        }

        if ((0 <= i+1 && i+1 < BOARD_ROWS) && (0 <=j && j <BOARD_ROWS))
        {
          tmp[((i*BOARD_ROWS+j)*4)+cnt] = (i+1) * BOARD_ROWS + j;
          cnt++;
        }

        for (;cnt<4;cnt++)
        {
          tmp[((i*BOARD_ROWS+j)*4)+cnt] = BLOCK;
        }
      }
    }
    return tmp;
  }();

  static constexpr std::array<uint32_t, BOARD_SIZE> liberty_init = []()
  {
    std::array<uint32_t, BOARD_SIZE> tmp = {};

    for (int i=0;i<BOARD_ROWS;i++)
    {
      for (int j=0;j<BOARD_ROWS;j++)
      {
        int cnt=0;
        if ((0 <= i && i < BOARD_ROWS) && (0 <=j-1 && j-1 <BOARD_ROWS))
        {
          cnt++;
        }

        if ((0 <= i && i < BOARD_ROWS) && (0 <=j+1 && j+1 <BOARD_ROWS))
        {
          cnt++;
        }

        if ((0 <= i-1 && i-1 < BOARD_ROWS) && (0 <=j && j <BOARD_ROWS))
        {
          cnt++;
        }

        if ((0 <= i+1 && i+1 < BOARD_ROWS) && (0 <=j && j <BOARD_ROWS))
        {
          cnt++;
        }

        tmp[i*BOARD_ROWS+j] = cnt;
      }
    }
    return tmp;
  }();

  static constexpr std::array<uint32_t, BOARD_SIZE> range_init = []()
  {
    std::array<uint32_t, BOARD_SIZE> tmp = {};

    for (int i=0;i<BOARD_SIZE;i++)
    {
      tmp[i]=1;
    }
    return tmp;
  }();

  static constexpr std::array<board, BOARD_SIZE> libert_board_init = []()
  {
    std::array<board, BOARD_SIZE> tmp = {};

    for (uint32_t i=0;i<BOARD_SIZE;i++)
    {
      board b;
      b.set(i);
      tmp[i] = b;
    }

    return tmp;
  }();

  static constexpr std::array<uint32_t, BOARD_SIZE> leader_init = []()
  {
    std::array<uint32_t, BOARD_SIZE> tmp = {};

    for (uint32_t i=0;i<BOARD_SIZE;i++)
    {
      tmp[i] = i;
    }

    return tmp;
  }();
};
}