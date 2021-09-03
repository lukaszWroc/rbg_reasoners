
#include <array>
#include <cstdint>
#include <vector>

//
// BOARD_ROWS can by any positive integer
//
#define BOARD_ROWS 10

#if BOARD_ROWS == 10
  #define STANDARD
#endif

#define BOARD_SIZE BOARD_ROWS*BOARD_ROWS

namespace reasoner
{
constexpr int PIECE_NUMBER            = BOARD_ROWS + (BOARD_ROWS % 2);
constexpr int PIECE_NUMBER_FOR_PLAYER = PIECE_NUMBER/2;

constexpr int NUMBER_OF_PLAYERS   = 3;
constexpr int MONOTONIC_CLASSES   = 0;
constexpr int NUMBER_OF_VARIABLES = 2;
constexpr int NUMBER_OF_PIECES    = 2;
constexpr uint64_t bit_mask = (1 << 6) -1;

class resettable_bitarray_stack {};

#ifdef STANDARD
typedef __int128_t board;
#else
template<uint32_t N> struct subset
{
  uint64_t date[N] = {0};

   __attribute__((hot)) inline uint64_t get(uint32_t x)
  {
    return (date[x >> 6] & ((uint64_t)1 << (x & bit_mask)));
  }

  __attribute__((hot)) constexpr inline void set(uint32_t x)
  {
    date[x>> 6] |= ((uint64_t)1 << (x & bit_mask));
  }

  __attribute__((hot)) inline void reset(uint32_t x)
  {
    date[x>> 6] ^= ((uint64_t)1 << (x & bit_mask));
  }
};

typedef subset<(BOARD_SIZE >> 6)+1> board;
#endif

struct move
{
  uint32_t idx,x1,y1;

  move(void) = default;
  move(uint32_t _idx, uint32_t _x1, uint32_t _y1) :
    idx(_idx), x1(_x1), y1(_y1) {};
  move(uint32_t _x1, uint32_t _y1) :
    x1(_x1), y1(_y1) {};
  bool operator==(const move& rhs) const;
};

#ifdef STANDARD
constexpr std::array<board, BOARD_SIZE> fill_index_to_mask()
{
  std::array<board, BOARD_SIZE> result{};

  for (int i=0;i<BOARD_SIZE;i++)
  {
    result[i] = (board)1 << i;
  }

  return result;
}
#endif

#ifndef STANDARD
constexpr board empty_tmp = []()
  {
    board tmp = board();

    int sum = PIECE_NUMBER_FOR_PLAYER;

    for (int i=1;i<BOARD_ROWS/2 && sum;i+=4)
    {
      tmp.set(0*BOARD_ROWS+i);
      tmp.set((BOARD_ROWS-1)*BOARD_ROWS+i);
      sum-=2;
    }

    for (int i=1;i<BOARD_ROWS && sum;i+=2)
    {
      tmp.set(i*BOARD_ROWS);
      sum--;
    }

    sum = PIECE_NUMBER_FOR_PLAYER;

    for (int i=BOARD_ROWS/2+1;i<BOARD_ROWS && sum;i+=4)
    {
      tmp.set(0*BOARD_ROWS+i);
      tmp.set((BOARD_ROWS-1)*BOARD_ROWS+i);
      sum-=2;
    }

    for (int i=1;i<BOARD_ROWS && sum;i+=2)
    {
      tmp.set(i*BOARD_ROWS+BOARD_ROWS);
      sum--;
    }

    return tmp;
  }();

constexpr std::array<uint32_t, PIECE_NUMBER> pieces_tmp = []()
  {
    std::array<uint32_t, PIECE_NUMBER> tmp = {};
    int cnt = 0;
    int sum = PIECE_NUMBER_FOR_PLAYER;

    for (int i=1;i<BOARD_ROWS/2 && sum;i+=4)
    {
      tmp[cnt++] = 0*BOARD_ROWS+i;
      tmp[cnt++] = (BOARD_ROWS-1)*BOARD_ROWS+i;
      sum-=2;
    }

    for (int i=1;i<BOARD_ROWS && sum;i+=2)
    {
      tmp[cnt++] = i*BOARD_ROWS;
      sum--;
    }

    sum = PIECE_NUMBER_FOR_PLAYER;

    for (int i=BOARD_ROWS/2+1;i<BOARD_ROWS && sum;i+=4)
    {
      tmp[cnt++] = 0*BOARD_ROWS+i;
      tmp[cnt++] = (BOARD_ROWS-1)*BOARD_ROWS+i;
      sum-=2;
    }

    for (int i=1;i<BOARD_ROWS && sum;i+=2)
    {
      tmp[cnt++] = i*BOARD_ROWS+BOARD_ROWS;
      sum--;
    }

    return tmp;
  }();
#endif

class game_state
{
  void get_moves_arrow(uint32_t piece, std::vector<move>& moves);
  void get_moves_piece(uint32_t piece, uint32_t idx, std::vector<move>& moves);

#ifdef STANDARD
  board empty = ((board)4831838240 << 64) +  (board)1152922055436402760;
  uint32_t pieces[2][4] = {{3, 6, 30, 39}, {60, 69, 93, 96}};
#else
  board empty;
  std::array<uint32_t, PIECE_NUMBER> pieces;
#endif

  uint32_t current_player = 1;
  uint32_t last_moved     = 0;

  bool piece_move = true;

  uint32_t variables[NUMBER_OF_VARIABLES] = {50, 50};

#ifdef STANDARD
  static constexpr std::array<board, BOARD_SIZE> index_to_mask = fill_index_to_mask();
#endif

public:
  int get_current_player(void) const;
  int get_player_score(int player_id) const;
  int get_monotonicity_class(void);

  void apply_move(const move& m);
  void get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves);

  bool apply_any_move(resettable_bitarray_stack&);
  bool is_legal(const move& m) const;

#ifndef STANDARD
  game_state() : empty(empty_tmp), pieces(pieces_tmp){}
#endif
};
}