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
  if (piece_move)
  {
    piece_move = false;
    last_moved = m.idx;
    #ifdef STANDARD
    empty ^= index_to_mask[pieces[current_player - 1][m.idx]] | index_to_mask[m.y1*BOARD_ROWS+m.x1];
    pieces[current_player - 1][m.idx] = m.y1*BOARD_ROWS+m.x1;
    #else
    empty.reset(pieces[(current_player - 1)*2+m.idx]);
    empty.set(m.y1*BOARD_ROWS+m.x1);
    pieces[(current_player - 1)*2 +m.idx] = m.y1*BOARD_ROWS+m.x1;
    #endif

  }
  else
  {
    #ifdef STANDARD
    empty ^= index_to_mask[m.y1*BOARD_ROWS+m.x1];
    #else
    empty.set(m.y1*BOARD_ROWS+m.x1);
    #endif

    current_player ^= 0b11;
    piece_move = true;
  }
}

void game_state::get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves)
{
  moves.clear();

  if (piece_move)
  {
    #ifdef STANDARD
    get_moves_piece(pieces[current_player -1][0], 0, moves);
    get_moves_piece(pieces[current_player -1][1], 1, moves);
    get_moves_piece(pieces[current_player -1][2], 2, moves);
    get_moves_piece(pieces[current_player -1][3], 3, moves);
    #else
    for (int i=0;i<PIECE_NUMBER_FOR_PLAYER;i++)
    {
      get_moves_piece(pieces[(current_player -1)*2+i], i, moves);
    }
    #endif

    if (moves.size() == 0)
    {
      variables[current_player - 1] = 0;
      variables[(current_player ^ 0b11) - 1] = 100;
    }
  }
  else
  {
    #ifdef STANDARD
    get_moves_arrow(pieces[current_player -1][last_moved], moves);
    #else
    get_moves_arrow(pieces[(current_player -1)*2+last_moved], moves);
    #endif
  }
}

#ifdef STANDARD
void game_state::get_moves_piece(uint32_t piece, uint32_t idx, std::vector<move>& moves)
{
  uint32_t y = piece/BOARD_ROWS;
  uint32_t x = piece%BOARD_ROWS;

  for (int i=x-1;i>=0 && !(empty & index_to_mask[y*BOARD_ROWS+i]);i--)
  {
    moves.emplace_back(idx,i,y);
  }

  for (uint32_t i=x+1;i<BOARD_ROWS && !(empty & index_to_mask[y*BOARD_ROWS+i]);i++)
  {
    moves.emplace_back(idx,i,y);
  }

  for (int i=y-1;i>=0 && !(empty & index_to_mask[i*BOARD_ROWS+x]);i--)
  {
    moves.emplace_back(idx,x,i);
  }

  for (uint32_t i=y+1;i<BOARD_ROWS && !(empty & index_to_mask[i*BOARD_ROWS+x]);i++)
  {
    moves.emplace_back(idx,x,i);
  }

  int xt = x+1;
  int yt = y+1;

  while (xt < BOARD_ROWS && yt < BOARD_ROWS && !(empty & index_to_mask[yt*BOARD_ROWS+xt]))
  {
    moves.emplace_back(idx,xt,yt);
    xt++; yt++;
  }

  xt = x+1;
  yt = y-1;

  while (xt < BOARD_ROWS && yt >= 0 && !(empty & index_to_mask[yt*BOARD_ROWS+xt]))
  {
    moves.emplace_back(idx,xt,yt);
    xt++; yt--;
  }

  xt = x-1;
  yt = y-1;

  while (xt >= 0 && yt >= 0 && !(empty & index_to_mask[yt*BOARD_ROWS+xt]))
  {
    moves.emplace_back(idx,xt,yt);
    xt--; yt--;
  }

  xt = x-1;
  yt = y+1;

  while (xt >= 0 && yt < BOARD_ROWS && !(empty & index_to_mask[yt*BOARD_ROWS+xt]))
  {
    moves.emplace_back(idx,xt,yt);
    xt--; yt++;
  }
}

void game_state::get_moves_arrow(uint32_t piece, std::vector<move>& moves)
{
  uint32_t y = piece/BOARD_ROWS;
  uint32_t x = piece%BOARD_ROWS;

  for (int i=x-1;i>=0 && !(empty & index_to_mask[y*BOARD_ROWS+i]);i--)
  {
    moves.emplace_back(i,y);
  }

  for (int i=x+1;i<BOARD_ROWS && !(empty & index_to_mask[y*BOARD_ROWS+i]);i++)
  {
    moves.emplace_back(i,y);
  }

  for (int i=y-1;i >= 0 && !(empty & index_to_mask[i*BOARD_ROWS+x]);i--)
  {
    moves.emplace_back(x,i);
  }

  for (int i=y+1;i<BOARD_ROWS && !(empty & index_to_mask[i*BOARD_ROWS+x]);i++)
  {
    moves.emplace_back(x,i);
  }

  int xt = x+1;
  int yt = y+1;

  while (xt < BOARD_ROWS && yt < BOARD_ROWS && !(empty & index_to_mask[yt*BOARD_ROWS+xt]))
  {
    moves.emplace_back(xt,yt);
    xt++; yt++;
  }

  xt = x+1;
  yt = y-1;

  while (xt < BOARD_ROWS && yt >= 0 && !(empty & index_to_mask[yt*BOARD_ROWS+xt]))
  {
    moves.emplace_back(xt,yt);
    xt++; yt--;
  }

  xt = x-1;
  yt = y-1;

  while (xt >= 0 && yt >= 0 && !(empty & index_to_mask[yt*BOARD_ROWS+xt]))
  {
    moves.emplace_back(xt,yt);
    xt--; yt--;
  }

  xt = x-1;
  yt = y+1;

  while (xt >= 0 && yt < BOARD_ROWS && !(empty & index_to_mask[yt*BOARD_ROWS+xt]))
  {
    moves.emplace_back(xt,yt);
    xt--; yt++;
  }
}
#else
void game_state::get_moves_piece(uint32_t piece, uint32_t idx, std::vector<move>& moves)
{
  uint32_t y = piece/BOARD_ROWS;
  uint32_t x = piece%BOARD_ROWS;

  for (int i=x-1;i>=0 && !(empty.get(y*BOARD_ROWS+i));i--)
  {
    moves.emplace_back(idx,i,y);
  }

  for (uint32_t i=x+1;i<BOARD_ROWS && !(empty.get(y*BOARD_ROWS+i));i++)
  {
    moves.emplace_back(idx,i,y);
  }

  for (int i=y-1;i>=0 && !(empty.get(i*BOARD_ROWS+x));i--)
  {
    moves.emplace_back(idx,x,i);
  }

  for (uint32_t i=y+1;i<BOARD_ROWS && !(empty.get(i*BOARD_ROWS+x));i++)
  {
    moves.emplace_back(idx,x,i);
  }

  int xt = x+1;
  int yt = y+1;

  while (xt < BOARD_ROWS && yt < BOARD_ROWS && !(empty.get(yt*BOARD_ROWS+xt)))
  {
    moves.emplace_back(idx,xt,yt);
    xt++; yt++;
  }

  xt = x+1;
  yt = y-1;

  while (xt < BOARD_ROWS && yt >= 0 && !(empty.get(yt*BOARD_ROWS+xt)))
  {
    moves.emplace_back(idx,xt,yt);
    xt++; yt--;
  }

  xt = x-1;
  yt = y-1;

  while (xt >= 0 && yt >= 0 && !(empty.get(yt*BOARD_ROWS+xt)))
  {
    moves.emplace_back(idx,xt,yt);
    xt--; yt--;
  }

  xt = x-1;
  yt = y+1;

  while (xt >= 0 && yt < BOARD_ROWS && !(empty.get(yt*BOARD_ROWS+xt)))
  {
    moves.emplace_back(idx,xt,yt);
    xt--; yt++;
  }
}

void game_state::get_moves_arrow(uint32_t piece, std::vector<move>& moves)
{
  uint32_t y = piece/BOARD_ROWS;
  uint32_t x = piece%BOARD_ROWS;

  for (int i=x-1;i>=0 && !(empty.get(y*BOARD_ROWS+i));i--)
  {
    moves.emplace_back(i,y);
  }

  for (int i=x+1;i<BOARD_ROWS && !(empty.get(y*BOARD_ROWS+i));i++)
  {
    moves.emplace_back(i,y);
  }

  for (int i=y-1;i >= 0 && !(empty.get(i*BOARD_ROWS+x));i--)
  {
    moves.emplace_back(x,i);
  }

  for (int i=y+1;i<BOARD_ROWS && !(empty.get(i*BOARD_ROWS+x));i++)
  {
    moves.emplace_back(x,i);
  }

  int xt = x+1;
  int yt = y+1;

  while (xt < BOARD_ROWS && yt < BOARD_ROWS && !(empty.get(yt*BOARD_ROWS+xt)))
  {
    moves.emplace_back(xt,yt);
    xt++; yt++;
  }

  xt = x+1;
  yt = y-1;

  while (xt < BOARD_ROWS && yt >= 0 && !(empty.get(yt*BOARD_ROWS+xt)))
  {
    moves.emplace_back(xt,yt);
    xt++; yt--;
  }

  xt = x-1;
  yt = y-1;

  while (xt >= 0 && yt >= 0 && !(empty.get(yt*BOARD_ROWS+xt)))
  {
    moves.emplace_back(xt,yt);
    xt--; yt--;
  }

  xt = x-1;
  yt = y+1;

  while (xt >= 0 && yt < BOARD_ROWS && !(empty.get(yt*BOARD_ROWS+xt)))
  {
    moves.emplace_back(xt,yt);
    xt--; yt++;
  }
}
#endif
}