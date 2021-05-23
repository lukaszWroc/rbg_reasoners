
#include <array>
#include <cstdint>
#include <vector>

#define STANDARD

#define BOARD_ROWS 20

#define BOARD_SIZE (BOARD_ROWS*BOARD_ROWS)

namespace reasoner
{
constexpr int NUMBER_OF_PLAYERS   = 3;
constexpr int MONOTONIC_CLASSES   = 0;
constexpr int NUMBER_OF_VARIABLES = 2;
constexpr int NUMBER_OF_PIECES    = 2;
constexpr uint64_t bit_mask = (1 << 6) -1;
class resettable_bitarray_stack {};

constexpr int TMP = (BOARD_SIZE >> 6)+1;
struct footprint
{
  bool u,d,l,r,ul,ur,dl,dr;
  uint32_t cnt,pos;
  footprint() :
    u(false),d(false),l(false),r(false),ul(false),ur(false),dl(false),dr(false),cnt(4),pos(0){};
};

struct move
{
  uint32_t start, end;

  move(void) = default;
  constexpr move(uint32_t _start, uint32_t _end) : start(_start), end(_end) {};
  bool operator==(const move& rhs) const;
};

template<uint32_t N> struct subset
{
  uint64_t date[N] = {0};
  int s = N;

   __attribute__((hot)) inline uint64_t get(uint32_t x)
  {
    return (date[x >> 6] & ((uint64_t)1 << (x & bit_mask)));
  }

  __attribute__((hot)) inline void set(uint32_t x)
  {
    date[x>> 6] |= ((uint64_t)1 << (x & bit_mask));
  }

  __attribute__((hot)) inline void reset(uint32_t x)
  {
    date[x>> 6] ^= ((uint64_t)1 << (x & bit_mask));
  }
};

typedef subset<TMP> board;

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

  game_state()
  {
    for (int i=0;i<43;i++)
    {
      pieces[0].set(initial[0][i]);
      pieces[1].set(initial[1][i]);
    }

    for (int i=0;i<BOARD_SIZE;i++)
    {
      visited[i] = false;
    }
  }

private:
  bool visited[BOARD_SIZE];
  int visited_tab[BOARD_SIZE];
  int visited_cnt = 0;

  board pieces[2];
  std::vector<int> king[2]= {{51}, {351}};
  int initial[2][43] =
    {{22,24,26,27,28,29,30,31,32,33,35,37,
      41,42,43,45,47,48,49,50,52,54,56,57,58,
      62,64,66,67,68,69,70,71,72,73,75,77,
      122,125,128,131,134,137},
    {262,265,268,271,274,277,
      322,324,326,327,328,329,330,331,332,333,335,337,
      341,342,343,345,347,348,349,350,352,354,356,357,358,
      362,364,366,367,368,369,370,371,372,373,375,377
    }};

  uint32_t current_player = 2;

  bool exit = false;

  uint32_t variables[NUMBER_OF_VARIABLES] = {50, 50};

  bool win_condition(uint32_t player);
  bool check_footprint(uint32_t pos);
  inline bool check(uint32_t pos, uint32_t player);

  void create_footprint(uint32_t pos, footprint &fp);
  void add_moves(footprint &fp, std::vector<move>& moves);
  void create_moves(std::vector<move>& moves);
  void clear_pos(uint32_t pos, uint32_t player);
  void set_pos(uint32_t start,uint32_t pos, board bcp);
  void update_king(int pos);
  inline void start(int pos, std::vector<move> &moves);

  inline int msb(const uint64_t p)
  {
    return 63 - __builtin_clzl(p);
  }
};
}