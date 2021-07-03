#include "reasoner.hpp"

namespace reasoner
{

int game_state::get_current_player(void) const
{
  return current_player;
}

int game_state::get_player_score(int player_id) const
{
  return variables[player_id-1];
}

bool game_state::apply_any_move(resettable_bitarray_stack&)
{
  return false;
}

int game_state::get_monotonicity_class(void)
{
  return -1;
}

bool game_state::is_legal([[maybe_unused]] const move& m) const
{
  return false;
}

void game_state::apply_move(const move &m)
{
  movesrule++;

  if (m.special == 1)
  {
    whiteKingMoved = true;
    whiteFigures   ^= whiteKing;
    whiteKing      = whiteKing >> 2;
    whiteFigures   |= whiteKing;
    whiteRooks     ^= ((uint64_t)0b00000001 << (7*8));
    whiteFigures   ^= ((uint64_t)0b00000001 << (7*8));
    whiteFigures   |= ((uint64_t)0b00000100 << (7*8));
    whiteRooks     |= ((uint64_t)0b00000100 << (7*8));
  }
  else if (m.special ==  2)
  {
    whiteKingMoved = true;
    whiteFigures   ^= whiteKing;
    whiteKing      = whiteKing << 2;
    whiteFigures   |= whiteKing;
    whiteRooks     ^= ((uint64_t)0b10000000 << (7*8));
    whiteFigures   ^= ((uint64_t)0b10000000 << (7*8));
    whiteFigures   |= ((uint64_t)0b00010000 << (7*8));
    whiteRooks     |= ((uint64_t)0b00010000 << (7*8));
  }
  else if (m.special ==  3)
  {
    blackKingMoved = true;
    blackFigures   ^= blackKing;
    blackKing      = blackKing >> 2;
    blackFigures   |= blackKing;
    blackRooks     ^= (uint64_t)0b00000001;
    blackFigures   ^= (uint64_t)0b00000001;
    blackFigures   |= (uint64_t)0b00000100;
    blackRooks     |= (uint64_t)0b00000100;
  }
  else if (m.special ==  4)
  {
    blackKingMoved = true;
    blackFigures   ^= blackKing;
    blackKing       =  blackKing << 2;
    blackFigures   |= blackKing;
    blackRooks     ^= (uint64_t)0b10000000;
    blackFigures   ^= (uint64_t)0b10000000;
    blackFigures   |= (uint64_t)0b00010000;
    blackRooks     |= (uint64_t)0b00010000;
  }
  else if (current_player == 1)
  {
    whiteFigures ^= (m.start | m.end);

    if (whitePawns & m.start)
    {
      movesrule = 0;

      if (m.special > 4)
      {
        whitePawns ^= m.start;

        if (m.special == 5)
        {
          whiteRooks |= m.end;
        }
        else if (m.special == 6)
        {
          whiteBishops |= m.end;
        }
        else if (m.special == 7)
        {
          whiteKnights |= m.end;
        }
        else if (m.special == 8)
        {
          whiteQueens |= m.end;
        }
      }
      else
      {
        if (m.end == (m.start >> 16))
        {
          whiteEP = m.end;
        }

        if (blackEP & ( m.end << 8))
        {
          blackFigures ^= blackEP;
          blackPawns   ^= blackEP;
        }

        whitePawns ^= (m.end | m.start);
      }
    }
    else if (whiteRooks & m.start)
    {
      if (m.start  & whiteLeftRook)
      {
        whiteLeftRookMoved=true;
      }

      if (m.start & whiteRightRook)
      {
        whiteRightRookMoved = true;
      }

      whiteRooks ^= (m.end | m.start);
    }
    else if (whiteKnights & m.start)
    {
      whiteKnights ^= (m.end | m.start);
    }
    else if (whiteBishops & m.start)
    {
      whiteBishops ^= (m.end | m.start);
    }
    else if (whiteQueens & m.start)
    {
      whiteQueens ^= (m.end | m.start);
    }
    else if (whiteKing & m.start)
    {
      if (whiteKing & m.start)
      {
        whiteKingMoved = true;
      }

      whiteKing ^= (m.end | m.start);
    }

    if (blackFigures & m.end)
    {
      movesrule = 0;

      blackFigures ^= m.end;

      if (blackPawns & m.end)
      {
        blackPawns ^= m.end;
      }
      else if (blackRooks & m.end)
      {
        if (m.end & blackRightRook)
        {
          blackRightRookMoved = true;
        }

        if (m.end & blackLeftRook)
        {
          blackLeftRookMoved = true;
        }

        blackRooks ^= m.end;
      }
      else if (blackKnights & m.end)
      {
        blackKnights ^= m.end;
      }
      else if (blackBishops & m.end)
      {
        blackBishops ^= m.end;
      }
      else if (blackQueens & m.end)
      {
        blackQueens ^= m.end;
      }
    }

    blackEP = 0;
  }
  else
  {
    blackFigures ^= (m.start | m.end);

    if (blackPawns & m.start)
    {
      movesrule = 0;

      if (m.special > 4)
      {
        blackPawns ^= m.start;

        if (m.special == 5)
        {
          blackRooks |= m.end;
        }
        else if (m.special == 6)
        {
          blackBishops |= m.end;
        }
        else if (m.special == 7)
        {
          blackKnights |= m.end;
        }
        else if (m.special == 8)
        {
          blackQueens |= m.end;
        }
      }
      else
      {
        if (m.end == (m.start << 16))
        {
          blackEP = m.end;
        }

        if (whiteEP & ( m.end >> 8))
        {
          whiteFigures ^= whiteEP;
          whitePawns   ^= whiteEP;
        }

        blackPawns ^= (m.end | m.start);
      }
    }
    else if (blackRooks & m.start)
    {
      if (m.start  & blackLeftRook)
      {
        blackLeftRookMoved = true;
      }

      if (m.start & blackRightRook)
      {
        blackRightRookMoved = true;
      }

      blackRooks ^= (m.end | m.start);
    }
    else if (blackKnights & m.start)
    {
      blackKnights ^= (m.end | m.start);
    }
    else if (blackBishops & m.start)
    {
      blackBishops ^= (m.end | m.start);
    }
    else if (blackQueens & m.start)
    {
      blackQueens ^= (m.end | m.start);
    }
    else if (blackKing & m.start)
    {
      blackKingMoved = true;
      blackKing      ^= (m.end | m.start);
    }

    if (whiteFigures & m.end)
    {
      movesrule = 0;

      whiteFigures ^=  m.end;

      if (whitePawns & m.end)
      {
        whitePawns ^= m.end;
      }
      else if (whiteRooks & m.end)
      {
        if (m.end & whiteRightRook)
        {
          whiteRightRookMoved = true;
        }

        if (m.end & whiteLeftRook)
        {
          whiteLeftRookMoved = true;
        }

        whiteRooks ^= m.end;
      }
      else if (whiteKnights & m.end)
      {
        whiteKnights ^= m.end;
      }
      else if (whiteBishops & m.end)
      {
        whiteBishops ^= m.end;
      }
      else if (whiteQueens & m.end)
      {
        whiteQueens ^= m.end;
      }
    }

    whiteEP = 0;
  }

  if (movesrule == 49)
  {
    variables[current_player - 1] = 50;
    variables[(current_player ^ 0b11) - 1] = 50;
    exit = true;
  }

  current_player ^= 0b11;
}

void game_state::get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves)
{
  moves.clear();

  if (exit)
  {
    return;
  }

  if (current_player == 1)
  {
    getWhitePawnsMoves(moves);
    getKnightMoves(moves, whiteKnights);
    getDiagonalMoves(moves, whiteBishops | whiteQueens);
    getStraightMoves(moves, whiteRooks  | whiteQueens);
    getKingMoves(moves, whiteKing);
  }
  else
  {
    getBlackPawnsMoves(moves);
    getKnightMoves(moves, blackKnights);
    getDiagonalMoves(moves, blackBishops | blackQueens);
    getStraightMoves(moves, blackRooks  | blackQueens);
    getKingMoves(moves, blackKing);
  }

  if (moves.size() == 0)
  {
    int win = check();

    if (win == 1)
    {
      variables[current_player - 1] = 0;
      variables[(current_player ^ 0b11) - 1] = 100;
      exit = true;
    }
    else if (win == 0)
    {
      variables[current_player - 1] = 50;
      variables[(current_player ^ 0b11) - 1] = 50;
      exit = true;
    }
  }
}

void game_state::upgradeWhite(uint64_t figures, int shift,std::vector<move>& moves)
{
  while (figures)
  {
    uint64_t mv = msb(figures);

    if (!check(mv, mv << shift))
    {
      for (int i=0;i<4;i++)
      {
        moves.emplace_back(mv, mv << shift,5+i);
      }
    }

    figures ^= mv;
  }
}

void game_state::upgradeBlack(uint64_t figures, int shift,std::vector<move>& moves)
{
  while (figures)
  {
    uint64_t mv = msb(figures);

    if (!check(mv, mv >> shift))
    {
      for (int i=0;i<4;i++)
      {
        moves.emplace_back(mv, mv >> shift,5+i);
      }
    }

    figures ^= mv;
  }
}

void game_state::getWhitePawnsMoves(std::vector<move>& moves)
{
  uint64_t freeSpace = ~(whiteFigures | blackFigures);
  uint64_t moveUp    = freeSpace & (whitePawns >> 8);
  uint64_t promoted  = moveUp & whitePromotionLine;

  moveUp = moveUp ^ promoted;

  upgradeWhite(promoted, 8, moves);
  addMovesRight(moves, moveUp, 8);

  uint64_t attackLeft   = (blackFigures ^ blackKing) & ((whitePawns & leftBoard) >> 9);
  uint64_t attackRight = (blackFigures ^ blackKing) & ((whitePawns & rightBoard) >> 7);

  addMovesRight(moves, attackLeft ^ (attackLeft & whitePromotionLine), 9);
  addMovesRight(moves, attackRight ^ (attackRight & whitePromotionLine), 7);

  upgradeWhite(attackLeft & whitePromotionLine,9,moves);
  upgradeWhite(attackRight & whitePromotionLine,7,moves);


  uint64_t moveUp2   = freeSpace & ((whitePawns & whiteFirstLine) >> 16);
  uint64_t enPassant = moveUp2 ^ (moveUp2 & (((~freeSpace) & whiteSecondLine) >> 8));

  addMovesRight(moves, enPassant, 16);

  if (blackEP)
  {
    uint64_t attackLeft2 = (blackEP & ((whitePawns & rightBoard) << 1)) >> 1;

    if ((attackLeft2 >> 7) & freeSpace)
    {
      addMovesRight(moves, attackLeft2 >> 7, 7);
    }

    uint64_t attackRight2 = (blackEP & ((whitePawns & leftBoard) >> 1)) << 1;

    if ((attackRight2 >> 9) & freeSpace)
    {
      addMovesRight(moves, attackRight2 >> 9, 9);
    }
  }
}

void game_state::getBlackPawnsMoves(std::vector<move>& moves)
{
  uint64_t freeSpace = ~(whiteFigures | blackFigures);
  uint64_t moveUp    = freeSpace & (blackPawns << 8);
  uint64_t promoted  = moveUp & blackPromotionLine;

  moveUp = moveUp ^ promoted;

  upgradeBlack(promoted, 8, moves);
  addMovesLeft(moves, moveUp, 8);

  uint64_t attackLeft  = (whiteFigures ^ whiteKing) & ((blackPawns & rightBoard) << 9);
  uint64_t attackRight = (whiteFigures ^ whiteKing) & ((blackPawns & leftBoard) << 7);

  addMovesLeft(moves,  attackLeft ^ (attackLeft & blackPromotionLine), 9);
  addMovesLeft(moves, attackRight ^ (attackRight & blackPromotionLine), 7);

  upgradeBlack(attackLeft & blackPromotionLine,9,moves);
  upgradeBlack(attackRight & blackPromotionLine,7,moves);

  uint64_t moveUp2   = freeSpace & ((blackPawns & blackFirstLine) << 16);
  uint64_t enPassant = moveUp2 ^ (moveUp2 & (((~freeSpace) & blackSecondLine) << 8));

  addMovesLeft(moves, enPassant, 16);

  if (whiteEP)
  {
    uint64_t attackLeft2 = (whiteEP & ((blackPawns & leftBoard) >> 1)) << 1;

    if ((attackLeft2 << 7) & freeSpace)
    {
      addMovesLeft(moves, attackLeft2 << 7, 7);
    }

    uint64_t attackRight2 = (whiteEP & ((blackPawns & rightBoard) << 1)) >> 1;

    if ((attackRight2 << 9) & freeSpace)
    {
      addMovesLeft(moves, attackRight2 << 9, 9);
    }
  }
}

void game_state::addMovesRight(std::vector<move>& moves, uint64_t figures, int shift)
{
  while (figures)
  {
    uint64_t mv = msb(figures);

    if (!check(mv, mv << shift))
    {
      moves.emplace_back(mv, mv << shift);
    }

    figures ^= mv;
  }
}

void game_state::addMovesLeft(std::vector<move>& moves, uint64_t figures, int shift)
{
  while (figures)
  {
    uint64_t mv = msb(figures);

    if (!check(mv, mv >> shift))
    {
      moves.emplace_back(mv, mv >> shift);
    }

    figures ^= mv;
  }
}

bool game_state::checkOther(uint64_t king, uint64_t otherQueens, uint64_t otherRooks, uint64_t otherBishops, uint64_t freeSpace)
{
  uint64_t mv;

  bool res = true;

  mv = king;
  mv = mv >> 8;

  if (mv & (otherQueens | otherRooks))
  {
    goto fail;
  }

  while (freeSpace & mv)
  {
    mv = mv >> 8;

    if (mv & (otherQueens | otherRooks))
    {
      goto fail;
    }

    mv &= freeSpace;
  }

  mv = king;
  mv = mv << 8;

  if (mv & (otherQueens | otherRooks))
  {
    goto fail;
  }

  while (freeSpace & mv)
  {
    mv = mv << 8;

    if (mv & (otherQueens | otherRooks))
    {
      goto fail;
    }

    mv &= freeSpace;
  }

  mv = king & leftBoard;
  mv = mv >> 1;

  if (mv & (otherQueens | otherRooks))
  {
    goto fail;
  }

  while (freeSpace & (mv & leftBoard))
  {
    mv = mv >> 1;

    if (mv & (otherQueens | otherRooks))
    {
      goto fail;
    }

    mv &= freeSpace;
  }

  mv = king & leftBoard;
  mv = mv >> 9;

  if (mv & (otherQueens | otherBishops))
  {
    goto fail;
  }

  while (freeSpace & (mv & leftBoard))
  {
    mv = mv >> 9;

    if (mv & (otherQueens | otherBishops))
    {
      goto fail;
    }

    mv &= freeSpace;
  }

  mv = king & leftBoard;
  mv = mv << 7;

  if (mv & (otherQueens | otherBishops))
  {
    goto fail;
  }

  while (freeSpace & (mv & leftBoard))
  {
    mv = mv << 7;

    if (mv & (otherQueens | otherBishops))
    {
      goto fail;
    }

    mv &= freeSpace;
  }

  mv = king & rightBoard;
  mv = mv << 1;

  if (mv & (otherQueens | otherRooks))
  {
    goto fail;
  }

  while (freeSpace & (mv & rightBoard))
  {
    mv = mv << 1;

    if (mv & (otherQueens | otherRooks))
    {
      goto fail;
    }

    mv &= freeSpace;
  }

  mv = king & rightBoard;
  mv = mv << 9;

  if (mv & (otherQueens | otherBishops))
  {
    goto fail;
  }

  while (freeSpace & (mv & rightBoard))
  {
    mv = mv << 9;

    if (mv & (otherQueens | otherBishops))
    {
      goto fail;
    }

    mv &= freeSpace;
  }

  mv = king & rightBoard;
  mv = mv >> 7;

  if (mv & (otherQueens | otherBishops))
  {
    goto fail;
  }

  while (freeSpace & (mv & rightBoard))
  {
    mv = mv >> 7;

    if (mv & (otherQueens | otherBishops))
    {
      goto fail;
    }

    mv &= freeSpace;
  }

  res = false;

fail:

  return res;
}

bool game_state::check(uint64_t end, uint64_t start)
{
  uint64_t my;
  uint64_t other;
  uint64_t otherQueens;
  uint64_t otherRooks;
  uint64_t otherBishops;
  uint64_t otherKnights;
  uint64_t otherPawns;
  uint64_t king;
  uint64_t otherKing;

  if (current_player == 1)
  {
    my           = whiteFigures;
    other        = blackFigures;
    otherQueens  = blackQueens;
    otherRooks   = blackRooks;
    otherBishops = blackBishops;
    otherKnights = blackKnights;
    otherPawns   = blackPawns;
    king         = whiteKing;
    otherKing    = blackKing;
  }
  else
  {
    my           = blackFigures;
    other        = whiteFigures;
    otherQueens  = whiteQueens;
    otherRooks   = whiteRooks;
    otherBishops = whiteBishops;
    otherKnights = whiteKnights;
    otherPawns   = whitePawns;
    king         = blackKing;
    otherKing    = whiteKing;
  }

  my ^= (end | start);

  if (current_player == 1)
  {
    if ((whitePawns & start) && (blackEP & ( end << 8)))
    {
      other      ^= blackEP;
      otherPawns ^= blackEP;
    }
  }
  else
  {
    if ((blackPawns & start) && (whiteEP & (end >> 8)))
    {
      other      ^= whiteEP;
      otherPawns ^= whiteEP;
    }
  }

  bool res = true;

  if (end & other)
  {
    other ^= end;

    if (end & otherKnights)
    {
      otherKnights ^= end;
    }
    else if (end & otherBishops)
    {
      otherBishops ^= end;
    }
    else if (end & otherQueens)
    {
      otherQueens ^= end;
    }
    else if (end & otherRooks)
    {
      otherRooks ^= end;
    }
    else if (end & otherPawns)
    {
      otherPawns ^= end;
    }
  }

  if (start & king)
  {
    king = end;
  }

  uint64_t freeSpace = ~(my | other);

  if (checkKing(king, otherKing))
  {
    goto fail;
  }

  if (checkKinghts(king, otherKnights))
  {
    goto fail;
  }

  if (checkPawns(king, otherPawns))
  {
    goto fail;
  }

  if (checkOther(king, otherQueens, otherRooks, otherBishops, freeSpace))
  {
    goto fail;
  }

  res = false;

  fail:

  return res;
}

bool game_state::check()
{
  uint64_t my;
  uint64_t other;
  uint64_t otherQueens;
  uint64_t otherRooks;
  uint64_t otherBishops;
  uint64_t otherKnights;
  uint64_t otherPawns;
  uint64_t king;

  if (current_player == 1)
  {
    my           = whiteFigures;
    other        = blackFigures;
    otherQueens  = blackQueens;
    otherRooks   = blackRooks;
    otherBishops = blackBishops;
    otherKnights = blackKnights;
    otherPawns   = blackPawns;
    king         = whiteKing;
  }
  else
  {
    my           = blackFigures;
    other        = whiteFigures;
    otherQueens  = whiteQueens;
    otherRooks   = whiteRooks;
    otherBishops = whiteBishops;
    otherKnights = whiteKnights;
    otherPawns   = whitePawns;
    king         = blackKing;
  }

  bool res = true;

  uint64_t freeSpace = ~(my | other);

  if (checkKinghts(king, otherKnights))
  {
    goto fail;
  }

  if (checkPawns(king, otherPawns))
  {
    goto fail;
  }

  if (checkOther(king, otherQueens, otherRooks, otherBishops, freeSpace))
  {
    goto fail;
  }

  res = false;

  fail:

  return res;
}

bool game_state::checkKing(uint64_t king, uint64_t otherKing)
{
  bool res = true;

  uint64_t tmp = king >> 8;

  if (tmp & otherKing)
  {
    goto fail;
  }

  tmp = king << 8;

  if (tmp & otherKing)
  {
    goto fail;
  }

  tmp = (king & leftBoard) >> 1;

  if (tmp & otherKing)
  {
    goto fail;
  }

  tmp = (king & rightBoard) << 1;

  if (tmp & otherKing)
  {
    goto fail;
  }

  tmp = (king & leftBoard) >> 9;

  if (tmp & otherKing)
  {
    goto fail;
  }

  tmp = (king & rightBoard) >> 7;

  if (tmp & otherKing)
  {
    goto fail;
  }

  tmp = (king & leftBoard) << 7;

  if (tmp & otherKing)
  {
    goto fail;
  }

  tmp = (king & rightBoard) << 9;

  if (tmp & otherKing)
  {
    goto fail;
  }

  res = false;

  fail:

  return res;
}

bool game_state::checkKinghts(uint64_t king, uint64_t knights)
{
  uint64_t tmp;

  bool res = true;

  tmp = (knights & leftBoard) >> (16 + 1);

  if (tmp & king)
  {
    goto fail;  
  }

  tmp = (knights & rightBoard) << (16 + 1);

  if (tmp & king)
  {
    goto fail;
  }

  tmp = (knights & rightBoard) >> (16 - 1);

  if (tmp & king)
  {
    goto fail;
  }

  tmp = (knights & leftBoard) << (16 - 1);

  if (tmp & king)
  {
    goto fail;
  }

  tmp = (knights & rightBoard2) << (8 + 2);

  if (tmp & king)
  {
    goto fail;
  }

  tmp = (knights & leftBoard2) >> (8 + 2);

  if (tmp & king)
  {
    goto fail;
  }

  tmp = (knights & rightBoard2) >> (8 - 2);

  if (tmp & king)
  {
    goto fail;
  }

  tmp = (knights & leftBoard2) << (8 - 2);

  if (tmp & king)
  {
    goto fail;
  }

  res = false;

fail:

  return res;
}


bool game_state::checkPawns(uint64_t king, uint64_t otherPawns)
{
  if (current_player == 1)
  {
    if (((king & leftBoard) >> 9) & otherPawns)
    {
      return true;
    }

    if (((king & rightBoard) >> 7) & otherPawns)
    {
      return true;
    }
  }
  else
  {
    if (((king & rightBoard) << 9) & otherPawns)
    {
      return true;
    }

    if (((king & leftBoard) << 7) & otherPawns)
    {
      return true;
    }
  }

  return false;
}

void game_state::getKnightMoves(std::vector<move>& moves, uint64_t figures)
{
  uint64_t freeSpace = ~(whiteFigures | blackFigures);
  uint64_t oponent;

  if (current_player == 1)
  {
    oponent = blackFigures ^ blackKing;
  }
  else
  {
    oponent = whiteFigures ^ whiteKing;
  }

  getMoveRight(moves, figures, oponent, leftBoard, freeSpace, 17);
  getMoveLeft(moves, figures, oponent, rightBoard, freeSpace, 17);
  getMoveRight(moves, figures, oponent, rightBoard, freeSpace, 15);
  getMoveLeft(moves, figures, oponent, leftBoard, freeSpace, 15);
  getMoveRight(moves, figures, oponent, leftBoard2, freeSpace, 10);
  getMoveLeft(moves, figures, oponent, rightBoard2, freeSpace, 10);
  getMoveRight(moves, figures, oponent, rightBoard2, freeSpace, 6);
  getMoveLeft(moves, figures, oponent, leftBoard2, freeSpace, 6);
}

void game_state::getKingMoves(std::vector<move>& moves, uint64_t figures)
{
  uint64_t freeSpace = ~(whiteFigures | blackFigures);
  uint64_t oponent;

  if (current_player == 1)
  {
    oponent = blackFigures ^ blackKing;
  }
  else
  {
    oponent = whiteFigures ^ whiteKing;
  }

  getMoveRight(moves, figures, oponent, freeSpace, 8);
  getMoveLeft(moves, figures, oponent, freeSpace, 8);
  getMoveRight(moves, figures, oponent, leftBoard, freeSpace, 1);
  getMoveLeft(moves, figures, oponent, rightBoard, freeSpace, 1);
  getMoveRight(moves, figures, oponent, leftBoard, freeSpace, 9);
  getMoveRight(moves, figures, oponent, rightBoard, freeSpace, 7);
  getMoveLeft(moves, figures, oponent, leftBoard, freeSpace, 7);
  getMoveLeft(moves, figures, oponent, rightBoard, freeSpace, 9);


    if (current_player == 1)
    {
      if (!whiteKingMoved)
      {
        if (!whiteRightRookMoved)
        {
          if ((freeSpace & whiteCastlingRight2) && (freeSpace &whiteCastlingRight1) &&
                !check() && !check(whiteKing >> 1, whiteKing) && !check(whiteKing >> 2, whiteKing))
          {
            moves.emplace_back(1,1,1);
          }
        }

        if (!whiteLeftRookMoved)
        {
          if ((freeSpace & whiteCastlingLeft2) && (freeSpace & whiteCastlingLeft1) && (freeSpace & whiteCastlingLeft3) &&
                !check() && !check(whiteKing << 1, whiteKing) && !check(whiteKing << 2, whiteKing))
          {
            moves.emplace_back(1,1,2);
          }
        }
      }
    }
    else
    {
      if (!blackKingMoved)
      {
        if (!blackRightRookMoved)
        {
          if ((freeSpace & blackCastlingRight2) && (freeSpace &blackCastlingRight1) &&
                !check() && !check(blackKing >> 1, blackKing) && !check(blackKing >> 2, blackKing))
          {
            moves.emplace_back(1,1,3);
          }
        }

        if (!blackLeftRookMoved)
        {
          if ((freeSpace & blackCastlingLeft2) && (freeSpace & blackCastlingLeft1) && ( freeSpace & blackCastlingLeft3) &&
                !check() && !check(blackKing << 1, blackKing)  && !check(blackKing << 2, blackKing))
          {
            moves.emplace_back(1,1,4);
          }
        }
      }
    }
}

void game_state::getDiagonalMoves(std::vector<move>& moves, uint64_t figures)
{
  uint64_t freeSpace = ~(whiteFigures | blackFigures);
  uint64_t oponent;

  if (current_player == 1)
  {
    oponent = blackFigures ^ blackKing;
  }
  else
  {
    oponent = whiteFigures ^ whiteKing;
  }

  getMovesRight(moves, figures, oponent, leftBoard,  freeSpace, 9);
  getMovesRight(moves, figures, oponent, rightBoard, freeSpace, 7);
  getMovesLeft(moves,  figures, oponent, rightBoard, freeSpace, 9);
  getMovesLeft(moves,  figures, oponent, leftBoard,  freeSpace, 7);
}


void game_state::getStraightMoves(std::vector<move>& moves, uint64_t figures)
{
  uint64_t freeSpace = ~(whiteFigures | blackFigures);
  uint64_t oponent;

  if (current_player == 1)
  {
    oponent = blackFigures ^ blackKing;
  }
  else
  {
    oponent = whiteFigures ^ whiteKing;
  }

  getMovesRight(moves, figures, oponent, freeSpace, 8);
  getMovesLeft(moves,  figures, oponent, freeSpace, 8);
  getMovesRight(moves, figures, oponent, leftBoard,  freeSpace, 1);
  getMovesLeft(moves,  figures, oponent, rightBoard, freeSpace, 1);
}

void game_state::getMovesRight(std::vector<move>& moves, uint64_t figures, uint64_t oponent, uint64_t side, uint64_t freeSpace, int shift)
{
  int i = shift;

  uint64_t figuresTmp = figures & side;

  figuresTmp = (figuresTmp >> shift);

  if (figuresTmp & oponent)
  {
    addMovesRight(moves, figuresTmp & oponent, i);    
  }

  figuresTmp = figuresTmp & freeSpace;

  while (figuresTmp)
  {
    addMovesRight(moves, figuresTmp & freeSpace, i);

    i += shift;

    figuresTmp = figuresTmp & side;
    figuresTmp = (figuresTmp >> shift);

    if (figuresTmp & oponent)
    {
      addMovesRight(moves, figuresTmp & oponent, i);    
    }

    figuresTmp = figuresTmp & freeSpace;
  }
}

void game_state::getMovesLeft(std::vector<move>& moves, uint64_t figures, uint64_t oponent, uint64_t side, uint64_t freeSpace, int shift)
{
  int i = shift;

  uint64_t figuresTmp = figures & side;

  figuresTmp = (figuresTmp << shift);

  if (figuresTmp & oponent)
  {
    addMovesLeft(moves, figuresTmp & oponent, i);
  }

  figuresTmp = figuresTmp & freeSpace;

  while (figuresTmp)
  {
    addMovesLeft(moves, figuresTmp & freeSpace, i);

    i += shift;

    figuresTmp = figuresTmp & side;
    figuresTmp = (figuresTmp << shift);

    if (figuresTmp & oponent)
    {
      addMovesLeft(moves, figuresTmp & oponent, i);
    }

    figuresTmp = figuresTmp & freeSpace;
  }
}

void game_state::getMovesRight(std::vector<move>& moves, uint64_t figures, uint64_t oponent, uint64_t freeSpace, int shift)
{
  int i = shift;

  uint64_t figuresTmp = figures;

  figuresTmp = (figuresTmp >> shift);

  if (figuresTmp & oponent)
  {
    addMovesRight(moves, figuresTmp & oponent, i);
  }

  figuresTmp = figuresTmp & freeSpace;

  while (figuresTmp)
  {
    addMovesRight(moves, figuresTmp & freeSpace, i);

    i += shift;

    figuresTmp = (figuresTmp >> shift);

    if (figuresTmp & oponent)
    {
      addMovesRight(moves, figuresTmp & oponent, i);
    }

    figuresTmp = figuresTmp & freeSpace;
  }
}

void game_state::getMovesLeft(std::vector<move>& moves, uint64_t figures, uint64_t oponent, uint64_t freeSpace, int shift)
{
  int i = shift;

  uint64_t figuresTmp = figures;

  figuresTmp = (figuresTmp << shift);

  if (figuresTmp & oponent)
  {
    addMovesLeft(moves, figuresTmp & oponent, i);
  }

  figuresTmp = figuresTmp & freeSpace;

  while (figuresTmp)
  {
    addMovesLeft(moves, figuresTmp & freeSpace, i);

    i += shift;

    figuresTmp = (figuresTmp << shift);

    if (figuresTmp & oponent)
    {
      addMovesLeft(moves, figuresTmp & oponent, i);
    }

    figuresTmp = figuresTmp & freeSpace;
  }
}

void game_state::getMoveRight(std::vector<move>& moves, uint64_t figures, uint64_t oponent, uint64_t side, uint64_t freeSpace, int shift)
{
  uint64_t tmp = (figures & side) >> shift;

  if (tmp & freeSpace)
  {
    addMovesRight(moves, tmp & freeSpace, shift);
  }

  if (tmp & oponent)
  {
    addMovesRight(moves, tmp & oponent, shift);
  }
}

void game_state::getMoveLeft(std::vector<move>& moves, uint64_t figures, uint64_t oponent, uint64_t side, uint64_t freeSpace, int shift)
{
  uint64_t tmp = (figures & side) << shift;

  if (tmp & freeSpace)
  {
    addMovesLeft(moves, tmp & freeSpace, shift);
  }

  if (tmp & oponent)
  {
    addMovesLeft(moves, tmp & oponent, shift);
  }
}

void game_state::getMoveRight(std::vector<move>& moves, uint64_t figures, uint64_t oponent, uint64_t freeSpace, int shift)
{
  uint64_t tmp = figures >> shift;

  if (tmp & freeSpace)
  {
    addMovesRight(moves, tmp & freeSpace, shift);
  }

  if (tmp & oponent)
  {
    addMovesRight(moves, tmp & oponent, shift);
  }
}

void game_state::getMoveLeft(std::vector<move>& moves, uint64_t figures, uint64_t oponent, uint64_t freeSpace, int shift)
{
  uint64_t tmp = figures << shift;

  if (tmp & freeSpace)
  {
    addMovesLeft(moves, tmp & freeSpace, shift);
  }

  if (tmp & oponent)
  {
    addMovesLeft(moves, tmp & oponent, shift);
  }
}
}