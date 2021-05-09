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
  empty = array_mask[m.cnt] ^ index_to_mask[m.y1*BOARD_ROWS+m.x1] ^ index_to_mask[m.y2*BOARD_ROWS+m.x2];

  pieces[current_player - 1][array_idx[m.cnt]] = array_pos[m.cnt];

  current_player ^= 0b11;
}

void game_state::get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves)
{
  moves.clear();

  array_cnt = 0;

  get_moves(pieces[current_player -1][0], 0, moves);
  get_moves(pieces[current_player -1][1], 1, moves);
  get_moves(pieces[current_player -1][2], 2, moves);
  get_moves(pieces[current_player -1][3], 3, moves);

  if (moves.size() == 0)
  {
    variables[current_player - 1] = 0;
    variables[(current_player ^ 0b11) - 1] = 100;
  }
}

void game_state::get_moves(uint32_t piece, uint32_t idx, std::vector<move>& moves)
{
  uint32_t y = piece/BOARD_ROWS;
  uint32_t x = piece%BOARD_ROWS;

  for (int i=y-1;i>=0 && !(empty & index_to_mask[i*BOARD_ROWS+x]);i--)
  {
    get_moves(i, x, idx, x,  y, moves);
  }

  for (int i=y+1;i<BOARD_ROWS && !(empty & index_to_mask[i*BOARD_ROWS+x]);i++)
  {
    get_moves(i, x, idx, x,  y, moves);
  }

  for (int i=x-1;i>=0 && !(empty & index_to_mask[y*BOARD_ROWS+i]);i--)
  {
    get_moves(y, i, idx, x, y, moves);
  }

  for (int i=x+1;i<BOARD_ROWS && !(empty & index_to_mask[y*BOARD_ROWS+i]);i++)
  {
    get_moves(y, i, idx, x, y, moves);
  }

  int xt = x+1;
  int yt = y+1;

  while (xt < BOARD_ROWS && yt < BOARD_ROWS && !(empty & index_to_mask[yt*BOARD_ROWS+xt]))
  {
    get_moves(yt, xt, idx, x, y, moves);
    xt++; yt++;
  }

  xt = x+1;
  yt = y-1;

  while (xt < BOARD_ROWS && yt >= 0 && !(empty & index_to_mask[yt*BOARD_ROWS+xt]))
  {
    get_moves(yt, xt, idx, x, y, moves);
    xt++; yt--;
  }

  xt = x-1;
  yt = y-1;

  while (xt >= 0 && yt >= 0 && !(empty & index_to_mask[yt*BOARD_ROWS+xt]))
  {
    get_moves(yt, xt, idx, x, y, moves);
    xt--; yt--;
  }

  xt = x-1;
  yt = y+1;

  while (xt >= 0 && yt < BOARD_ROWS && !(empty & index_to_mask[yt*BOARD_ROWS+xt]))
  {
    get_moves(yt, xt, idx, x, y, moves);
    xt--; yt++;
  }
}

void game_state::get_moves(uint32_t y, uint32_t x, uint32_t idx, uint32_t x1, uint32_t y1, std::vector<move>& moves)
{
  empty ^= index_to_mask[y1*BOARD_ROWS+x1];

  array_cnt++;

  array_mask[array_cnt] = empty;
  array_pos[array_cnt]  = y*BOARD_ROWS+x;
  array_idx[array_cnt]  = idx;

  for (int i=x-1;i>=0 && !(empty & index_to_mask[y*BOARD_ROWS+i]);i--)
  {
    moves.emplace_back(array_cnt,x,y,i,y);
  }

  for (int i=x+1;i<BOARD_ROWS && !(empty & index_to_mask[y*BOARD_ROWS+i]);i++)
  {
    moves.emplace_back(array_cnt,x,y,i,y);
  }

  for (int i=y-1;i>=0 && !(empty & index_to_mask[i*BOARD_ROWS+x]);i--)
  {
    moves.emplace_back(array_cnt,x,y,x,i);
  }

  for (int i=y+1;i<BOARD_ROWS && !(empty & index_to_mask[i*BOARD_ROWS+x]);i++)
  {
    moves.emplace_back(array_cnt,x,y,x,i);
  }

  int xt = x+1;
  int yt = y+1;

  while (xt < BOARD_ROWS && yt < BOARD_ROWS && !(empty & index_to_mask[yt*BOARD_ROWS+xt]))
  {
    moves.emplace_back(array_cnt,x,y,xt,yt);
    xt++; yt++;
  }

  xt = x+1;
  yt = y-1;

  while (xt < BOARD_ROWS && yt >= 0 && !(empty & index_to_mask[yt*BOARD_ROWS+xt]))
  {
    moves.emplace_back(array_cnt,x,y,xt,yt);
    xt++; yt--;
  }

  xt = x-1;
  yt = y-1;

  while (xt >= 0 && yt >= 0 && !(empty & index_to_mask[yt*BOARD_ROWS+xt]))
  {
    moves.emplace_back(array_cnt,x,y,xt,yt);
    xt--; yt--;
  }

  xt = x-1;
  yt = y+1;

  while (xt >= 0 && yt < BOARD_ROWS && !(empty & index_to_mask[yt*BOARD_ROWS+xt]))
  {
    moves.emplace_back(array_cnt,x,y,xt,yt);
    xt--; yt++;
  }

  empty ^= index_to_mask[y1*BOARD_ROWS+x1];
}
}