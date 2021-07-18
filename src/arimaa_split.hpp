
#include <cstdint>
#include <vector>
#include <array>
#include <iostream>
#include <set>

#define BOARD_ROWS 8
#define BOARD_SIZE BOARD_ROWS * BOARD_ROWS
#define TURN_LIMIT 200
#define OUTSIDE_BOARD 3

namespace reasoner
{

struct board_state
{
  board_state() {};
  board_state(uint64_t _rabbits, uint64_t _horses, uint64_t _cats, uint64_t _dogs,
      uint64_t _camel, uint64_t _elephant)
      : rabbits(_rabbits), horses(_horses), cats(_cats), dogs(_dogs), camel(_camel), elephant(_elephant)
    {};
  bool operator== (const board_state &b) const
  {
    return (this -> elephant == b.elephant) &&
          (this -> camel == b.camel) &&
          (this -> horses == b.horses) &&
          (this -> dogs == b.dogs) &&
          (this -> cats == b.cats) &&
          (this -> rabbits == b.rabbits);

  }

  bool operator< (const board_state &b) const
  {
    if (this -> elephant == b.elephant)
    {
      if (this -> camel == b.camel)
      {
        if (this -> horses == b.horses)
        {
          if (this -> dogs == b.dogs)
          {
            if (this -> cats == b.cats)
            {
              return this -> rabbits < b.rabbits;
            }

            return this -> cats < b.cats;
          }

          return this -> dogs < b.dogs;
        }

        return this -> horses < b.horses;
      }

      return this -> camel < b.camel;
    }

    return this -> elephant < b.elephant;
  }

  uint64_t rabbits;
  uint64_t horses;
  uint64_t cats;
  uint64_t dogs;
  uint64_t camel;
  uint64_t elephant;
};

struct full_state
{
  board_state gold, silver;

  full_state(){};
  full_state(board_state &_gold, board_state &_silver) : gold(_gold), silver(_silver)
  {}

  bool operator< (const full_state &f) const
  {
    if (this -> gold == f.gold)
    {
      return this -> silver < f.silver;
    }

    return this -> gold < f.gold;
  }

  bool operator== (const full_state &f) const
  {
    return (this -> gold == f.gold) && (this -> silver == f.silver);
  }
};

constexpr int PASS = BOARD_SIZE + 1;

constexpr int NUMBER_OF_PLAYERS   = 3;
constexpr int MONOTONIC_CLASSES   = 0;
constexpr int NUMBER_OF_VARIABLES = 2;
constexpr int NUMBER_OF_PIECES    = 2;

class resettable_bitarray_stack {};

struct move
{
  uint64_t mr, pos, oponent_from, oponent_to;
  move(void) = default;
  constexpr move(const uint64_t& mv) : mr(mv), pos(OUTSIDE_BOARD), oponent_from(OUTSIDE_BOARD), oponent_to(OUTSIDE_BOARD){};
  constexpr move(const uint64_t& mv, const uint64_t& _pos) : mr(mv), pos(_pos), oponent_from(OUTSIDE_BOARD), oponent_to(OUTSIDE_BOARD){};
  constexpr move(const uint64_t& mv, const uint64_t& _pos, const uint64_t& _oponent_from , const uint64_t& _oponent_to) :
    mr(mv), pos(_pos), oponent_from(_oponent_from), oponent_to(_oponent_to){};
  bool operator==(const move& rhs) const;
};

class game_state
{
  void print(uint64_t b)
  {
    std::cout << "\n\n";
    for(int i=0;i<8;i++)
    {
      for (int j=0;j<8;j++)
      {
        if ( ((uint64_t)1 << (i*8+j)) & b)
        {
          std::cout << "1";
        }
        else
        std::cout <<"0";

      }
      std::cout << "\n";
    }
        std::cout << "\n\n";

  }
  std::set<full_state> states;

  int current_player = 1;

  int exit       = 0;
  int turn_limit = 0;
  int setup_cnt  = 0;
  int move_cnt   = 0;
  int last_pos   = BOARD_SIZE;

  int variables[NUMBER_OF_VARIABLES] = {50, 50};

  uint32_t pieces[2] = {0, 0};

  full_state last_full_state;

  uint64_t goldPieces   = (uint64_t)0xffff << (6*8);
  uint64_t goldRabbits  = (uint64_t)0xff << (6*8);
  uint64_t goldHorses   = (uint64_t)0b10000001 << (7*8);
  uint64_t goldCats     = (uint64_t)0b01000010 << (7*8);
  uint64_t goldDogs     = (uint64_t)0b00100100 << (7*8);
  uint64_t goldCamel    = (uint64_t)0b00010000 << (7*8);
  uint64_t goldElephant = (uint64_t)0b00001000 << (7*8);
  uint64_t goldEndLine  = (uint64_t)0xff;

  uint64_t silverPieces   = (uint64_t)0xffff;
  uint64_t silverRabbits  = (uint64_t)0xff << 8;
  uint64_t silverHorses   = (uint64_t)0b10000001;
  uint64_t silverCats     = (uint64_t)0b01000010;
  uint64_t silverDogs     = (uint64_t)0b00100100;
  uint64_t silverCamel    = (uint64_t)0b00010000;
  uint64_t silverElephant = (uint64_t)0b00001000;
  uint64_t silverEndLine  = (uint64_t)0xff << (7*8);

  int elephantStr = 5;
  int camelStr    = 4;
  int horseStr    = 3;
  int dogStr      = 2;
  int catStr      = 1;
  int rabbitStr   = 0;

  bool repeted = false;


  uint64_t rightBoard = (uint64_t)0x7f7f7f7f7f7f7f7f;
  uint64_t leftBoard  = (uint64_t)0xfefefefefefefefe;

  uint64_t traps = ((uint64_t)0b00100100 << 16) | (((uint64_t)0b00100100) << 40);

  std::vector<move> silverSetup;
  std::vector<move> goldSetup;

  __attribute__((hot)) inline uint64_t msb(const uint64_t p)
  {
    return (uint64_t)1 << (63 - __builtin_clzl(p));
  }

  __attribute__((hot)) inline uint32_t getPos(const uint64_t &p)
  {
    return 63 - __builtin_clzl(p);
  }

  inline void insert(std::vector<move>& moves, uint64_t from1, uint64_t to1 = OUTSIDE_BOARD,
                        uint64_t from2 = OUTSIDE_BOARD, uint64_t to2 = OUTSIDE_BOARD);

  void setup(std::vector<move>& moves);
  void apply_setup(uint32_t m);
  void getMoves(uint64_t pieces, int str, std::vector<move>& moves);
  void addIfStronger(uint64_t pos, int str, uint64_t &res);
  void addIfWeaker(uint64_t pos, int str, uint64_t &res);

  void addMoves(uint64_t tmp, uint64_t oponentNeighbour, uint64_t freeSpace,
                  uint64_t mv, std::vector<move>& moves);

  bool frozzen(uint64_t pos, uint64_t my, uint64_t other, int str);
  bool protected1(uint64_t pos, uint64_t my);
  void update(uint64_t from, uint64_t to);
  board_state updateCopy(uint64_t from, uint64_t to);

  bool checkWin(uint32_t player);
  bool stateDifferent();

  void getGoldRabbitMoves(std::vector<move> &moves);
  void getSilverRabbitMoves(std::vector<move> &moves);
  void removeTraped(uint32_t player);

  __attribute__((hot)) inline uint64_t getFreeNeighbour(const uint64_t &pos);
  __attribute__((hot)) inline uint64_t getNeighbour(const uint64_t &pos, const uint64_t &oponent, int str);

  static constexpr std::array<uint64_t, 64> neighbour = []()
  {
    std::array<uint64_t, 64> tmp = {};

    for (int i=0;i<BOARD_ROWS;i++)
    {

      for (int j=0;j<BOARD_ROWS;j++)
      {
        uint64_t res = 0;

        if ((0 <= i && i < BOARD_ROWS) && (0 <=j-1 && j-1 <BOARD_ROWS))
        {
          res |= (uint64_t)1 << (i*BOARD_ROWS + j -1);
        }

        if ((0 <= i && i < BOARD_ROWS) && (0 <=j+1 && j+1 <BOARD_ROWS))
        {
          res |= (uint64_t)1 << (i*BOARD_ROWS + j + 1);
        }

        if ((0 <= i-1 && i-1 < BOARD_ROWS) && (0 <=j && j <BOARD_ROWS))
        {
          res |= (uint64_t)1 << ((i-1)*BOARD_ROWS + j);
        }

        if ((0 <= i+1 && i+1 < BOARD_ROWS) && (0 <=j && j <BOARD_ROWS))
        {
          res |= (uint64_t)1 << ((i+1)*BOARD_ROWS + j);
        }

        tmp[i*BOARD_ROWS+j] = res;
      }
    }

    return tmp;
  }();

  static constexpr std::array<move, 16> goldSetup_init = []()
  {
    std::array<move, 16> tmp = {};

    for (int i=0;i<16;i++)
    {
      tmp[i]=move(6*BOARD_ROWS+i,i);
    }

    return tmp;
  }();

  static constexpr std::array<move, 16> silverSetup_init = []()
  {
    std::array<move, 16> tmp = {};

    for (int i=0;i<16;i++)
    {
      tmp[i]=move(i,i);
    }

    return tmp;
  }();

public:
  int get_current_player(void) const;
  int get_player_score(int player_id) const;
  void apply_move(const move& m);
  void get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves);
  bool apply_any_move(resettable_bitarray_stack&);
  int get_monotonicity_class(void);
  bool is_legal(const move& m) const;

  game_state()
  {
    goldSetup.insert(goldSetup.begin(), std::begin(goldSetup_init), std::end(goldSetup_init));
    silverSetup.insert(silverSetup.begin(), std::begin(silverSetup_init), std::end(silverSetup_init));
    board_state gold(goldRabbits, goldHorses, goldCats, goldDogs, goldCamel, goldElephant);
    board_state silver(silverRabbits, silverHorses, silverCats, silverDogs, silverCamel, silverElephant);

    last_full_state = full_state(gold, silver);
    states.insert(full_state(gold, silver));
  }
};
}