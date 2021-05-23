
#include <array>
#include <cstdint>
#include <vector>

#define STANDARD

#define BOARD_ROWS 22

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
  int cnt,pos;
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
  std::vector<int> king[2]= {{78}, {408}};
  int initial[2][43] =
    {{47,49,51,52,53,54,55,56,57,58,60,62,68,69,70,72,74,75,76,77,79,81,83,84,85,91,93,95,96,97,98,99,100,101,102,104,106,157,160,163,166,169,172},
    {311,314,317,320,323,326,377,379,381,382,383,384,385,386,387,388,390,392,398,399,400,402,404,405,406,407,409,411,413,414,415,421,423,425,426,427,428,429,430,431,432,434,436}};

  uint32_t current_player = 2;

  bool exit = false;

  uint32_t variables[NUMBER_OF_VARIABLES] = {50, 50};

  bool win_condition(uint32_t player);
  bool check_footprint(int pos);
  inline bool check(int pos, uint32_t player);

  void create_footprint(int pos, footprint &fp);
  void add_moves(footprint &fp, std::vector<move>& moves);
  void create_moves(std::vector<move>& moves);
  void clear_pos(int pos, uint32_t player);
  void set_pos(int start, int pos, board &bcp);
  void update_king(int pos);
  inline void start(int pos, std::vector<move> &moves);

  inline int msb(const uint64_t p)
  {
    return 63 - __builtin_clzl(p);
  }
};
}