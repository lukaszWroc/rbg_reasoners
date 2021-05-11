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

bool game_state::win_condition()
{
  uint32_t count = 1;

  uint32_t x = last_moved%BOARD_ROWS;
  uint32_t y = last_moved/BOARD_ROWS;

  for (int i=x-1;i>=0 && pieces[current_player - 1].get(y*BOARD_ROWS + i);i--)
  {
    count++;
  }

  for (int i=x+1;i<BOARD_ROWS && pieces[current_player - 1].get(y *BOARD_ROWS + i);i++)
  {
    count++;
  }

#ifdef STANDARD
  if (count == 5)
  {
    return true;
  }
#elif
  if (count > 4)
  {
    return true;
  }
#endif

  count = 1;

  for (int i=y-1;i>=0 && pieces[current_player - 1].get(i*BOARD_ROWS+x);i--)
  {
    count++;
  }

  for (int i=y+1;i<BOARD_ROWS && pieces[current_player - 1].get(i*BOARD_ROWS+x);i++)
  {
    count++;
  }

#ifdef STANDARD
  if (count == 5)
  {
    return true;
  }
#elif
  if (count > 4)
  {
    return true;
  }
#endif

  count = 1;

  int xt = x+1;
  int yt = y+1;

  while (xt < BOARD_ROWS && yt < BOARD_ROWS && pieces[current_player - 1].get(yt*BOARD_ROWS+xt))
  {
    count++;
    xt++; yt++;
  }

  xt = x-1;
  yt = y-1;

  while (xt >= 0 && yt >= 0 && pieces[current_player - 1].get(yt*BOARD_ROWS+xt))
  {
    count++;
    xt--; yt--;
  }

#ifdef STANDARD
  if (count == 5)
  {
    return true;
  }
#elif
  if (count > 4)
  {
    return true;
  }
#endif

  count = 1;

  xt = x+1;
  yt = y-1;

  while (xt < BOARD_ROWS && yt >= 0 && pieces[current_player - 1].get(yt*BOARD_ROWS+xt))
  {
    count++;
    xt++; yt--;
  }

  xt = x-1;
  yt = y+1;

  while (xt >= 0 && yt < BOARD_ROWS && pieces[current_player - 1].get(yt*BOARD_ROWS+xt))
  {
    count++;
    xt--; yt++;
  }

#ifdef STANDARD
  if (count == 5)
  {
    return true;
  }
#elif
  if (count > 4)
  {
    return true;
  }
#endif

  return false;
}

void game_state::apply_move(const move &m)
{
  pieces[current_player - 1].set(m.mr);

  last_moved = m.mr;
  last_pos   = m.pos;

  if (win_condition())
  {
    variables[current_player - 1] = 100;
    variables[(current_player ^ 0b11) - 1] = 0;
    exit = true;
  }

  current_player ^= 0b11;
}

void game_state::get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves)
{
  if (exit)
  {
    moves.clear();

    return;
  }

  if (moves.size() == 0)
  {
    moves.insert(moves.begin(), std::begin(empty), std::end(empty));

    return;
  }

  size_t cnt = moves.size()-1;

  moves[last_pos].mr ^= moves[cnt].mr;
  moves[cnt].mr      ^= moves[last_pos].mr;
  moves[last_pos].mr ^= moves[cnt].mr;

  moves.pop_back();
}
}