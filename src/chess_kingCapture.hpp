
#include <array>
#include <cstdint>
#include <vector>

#undef LIMIT_TURNS

namespace reasoner
{
#ifdef LIMIT_TURNS
constexpr int TURN_LIMIT = 200;
#endif

constexpr int NUMBER_OF_PLAYERS   = 3;
constexpr int MONOTONIC_CLASSES   = 0;
constexpr int NUMBER_OF_VARIABLES = 2;
constexpr int NUMBER_OF_PIECES    = 2;

class resettable_bitarray_stack {};

struct move
{
  uint64_t end, start;
  uint16_t special;
  // 0 none
  // 1 castling white right
  // 2 castling white left
  // 3 castling black right
  // 4 castling black left
  // 5 promotion to knight
  // 6 promotion to queen
  // 7 promotion to rook
  // 8 promotion to bishop

  move(void) = default;
  move(uint64_t _end, uint64_t _start) : end(_end), start(_start), special(0) {};
  move(uint64_t _end, uint64_t _start, uint64_t _special) : end(_end), start(_start), special(_special) {};
  bool operator==(const move& rhs) const;
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
  uint64_t blackFigures        = (uint64_t)0xffff;
  uint64_t blackPawns          = (uint64_t)0xff << 8;
  uint64_t blackRooks          = (uint64_t)0b10000001;
  uint64_t blackKnights        = (uint64_t)0b01000010;
  uint64_t blackBishops        = (uint64_t)0b00100100;
  uint64_t blackQueens         = (uint64_t)0b00010000;
  uint64_t blackKing           = (uint64_t)0b00001000;
  uint64_t blackPromotionLine  = (uint64_t)0xff << (7*8);
  uint64_t blackFirstLine      = (uint64_t)0xff << 8;
  uint64_t blackSecondLine     = (uint64_t)0xff << 16;
  uint64_t blackLeftRook       = (uint64_t)0b10000000;
  uint64_t blackRightRook      = (uint64_t)0b00000001;
  uint64_t blackCastlingRight1 = (uint64_t)0b00000010;
  uint64_t blackCastlingRight2 = (uint64_t)0b00000100;
  uint64_t blackCastlingLeft1  = (uint64_t)0b01000000;
  uint64_t blackCastlingLeft2  = (uint64_t)0b00100000;
  uint64_t blackCastlingLeft3  = (uint64_t)0b00010000;
  bool blackKingMoved          = 0;
  bool blackLeftRookMoved      = 0;
  bool blackRightRookMoved     = 0;

  uint64_t whiteFigures        = (uint64_t)0xffff << (6*8);
  uint64_t whitePawns          = (uint64_t)0xff << (6*8);
  uint64_t whiteRooks          = (uint64_t)0b10000001 << (7*8);
  uint64_t whiteKnights        = (uint64_t)0b01000010 << (7*8);
  uint64_t whiteBishops        = (uint64_t)0b00100100 << (7*8);
  uint64_t whiteQueens         = (uint64_t)0b00010000 << (7*8);
  uint64_t whiteKing           = (uint64_t)0b00001000 << (7*8);
  uint64_t whitePromotionLine  = (uint64_t)0xff;
  uint64_t whiteFirstLine      = (uint64_t)0xff << (6*8);
  uint64_t whiteSecondLine     = (uint64_t)0xff << (5*8);
  uint64_t whiteLeftRook       = (uint64_t)0b10000000 << (7*8);
  uint64_t whiteRightRook      = (uint64_t)0b00000001 << (7*8);
  uint64_t whiteCastlingRight1 = (uint64_t)0b00000010<< (7*8);
  uint64_t whiteCastlingRight2 = (uint64_t)0b00000100<< (7*8);
  uint64_t whiteCastlingLeft1  = (uint64_t)0b01000000<< (7*8);
  uint64_t whiteCastlingLeft2  = (uint64_t)0b00100000<< (7*8);
  uint64_t whiteCastlingLeft3  = (uint64_t)0b00010000<< (7*8);
  bool whiteKingMoved          = 0;
  bool whiteLeftRookMoved      = 0;
  bool whiteRightRookMoved     = 0;

  uint64_t rightBoard = (uint64_t)0x7f7f7f7f7f7f7f7f;
  uint64_t leftBoard  = (uint64_t)0xfefefefefefefefe;

  uint64_t rightBoard2 = rightBoard ^ 0x4040404040404040;
  uint64_t leftBoard2  = leftBoard  ^ 0x0202020202020202;

  uint64_t blackEP = 0;
  uint64_t whiteEP = 0;

  uint32_t current_player = 1;
  uint32_t movesrule      = 0;

  #ifdef LIMIT_TURNS
  uint32_t turnCnt = 0;
  #endif

  bool exit = false;

  uint32_t variables[NUMBER_OF_VARIABLES] = {50, 50};

  void upgradeWhite(uint64_t fig, int shift,std::vector<move>& moves);
  void upgradeBlack(uint64_t fig, int shift,std::vector<move>& moves);
  void getStraightMoves(std::vector<move>& moves, uint64_t figures);
  void getDiagonalMoves(std::vector<move>& moves, uint64_t figures);
  void getKnightMoves(std::vector<move>& moves, uint64_t figures);
  void getKingMoves(std::vector<move>& moves, uint64_t figures);
  void getBlackPawnsMoves(std::vector<move>& moves);
  void getWhitePawnsMoves(std::vector<move>& moves);

  inline void addMovesLeft(std::vector<move>& moves, uint64_t figures, int shift);
  inline void addMovesRight(std::vector<move>& moves, uint64_t figures, int shift);
  inline void getMovesRight(std::vector<move>& moves, uint64_t figures, uint64_t oponent,
                              uint64_t side, uint64_t freeSpace, int shift);
  inline void getMovesLeft(std::vector<move>& moves, uint64_t figures, uint64_t oponent,
                              uint64_t side, uint64_t freeSpace, int shift);
  inline void getMovesRight(std::vector<move>& moves, uint64_t figures, uint64_t oponent,
                              uint64_t freeSpace, int shift);
  inline void getMovesLeft(std::vector<move>& moves, uint64_t figures, uint64_t oponent,
                              uint64_t freeSpace, int shift);
  inline void getMoveRight(std::vector<move>& moves, uint64_t figures, uint64_t oponent,
                              uint64_t side, uint64_t freeSpace, int shift);
  inline void getMoveLeft(std::vector<move>& moves, uint64_t figures, uint64_t oponent,
                              uint64_t side, uint64_t freeSpace, int shift);
  inline void getMoveRight(std::vector<move>& moves, uint64_t figures, uint64_t oponent,
                              uint64_t freeSpace, int shift);
  inline void getMoveLeft(std::vector<move>& moves, uint64_t figures, uint64_t oponent,
                              uint64_t freeSpace, int shift);

  inline uint64_t msb(const uint64_t p)
  {
    return (uint64_t)1 << (63 - __builtin_clzl(p));
  }
};
}