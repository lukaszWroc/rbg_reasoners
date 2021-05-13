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

void game_state::fill_mask(uint32_t &mask, uint32_t p)
{
  bool in = 0;

  for (int j=0;j<9;j++)
  {
    if (mask & ((uint32_t)1 << j))
    {
      in = 1;
    }
    else if ((p & ((uint32_t)1 << j)) == (uint32_t)0)
    {
      in = 0;
    }

    if (in)
    {
      mask |= ((uint32_t)1 << j);
    }
  }

  in = 0;

  for(int j=8;j>=0;j--)
  {
    if (mask & ((uint32_t)1 << j))
    {
      in = 1;
    }
    else if ((p & ((uint32_t)1 << j)) == (uint32_t)0)
    {
      in = 0;
    }

    if (in)
    {
      mask |= ((uint32_t)1 << j);
    }
  }
}

bool game_state::win_condition_up_down()
{
  uint32_t mask = up_down[0];

  for (int i=1;i<9;i++)
  {
    mask = (mask & up_down[i]) | (((mask << (uint32_t)1) & (uint32_t)0b111111111) & up_down[i]);

    fill_mask(mask, up_down[i]);
  }

  return mask;
}


bool game_state::win_condition_left_right()
{
  uint32_t mask = left_right[0];

  for (int i=1;i<9;i++)
  {
    mask = (mask & left_right[i]) | (((mask << (uint32_t)1) & (uint32_t)0b111111111) & left_right[i]);

    fill_mask(mask, left_right[i]);

  }

  return mask;
}

void game_state::apply_move(const move &m)
{
  uint32_t x = m.mr%BOARD_ROWS;
  uint32_t y = m.mr/BOARD_ROWS;

  if (current_player == 1)
  {
    left_right[x] |= ((uint32_t)1 << (uint32_t)y);

    if (win_condition_left_right())
    {
      variables[0] = 100;
      variables[1] = 0;
      exit = true;
      return;
    }
  }
  else
  {
    up_down[y] |= ((uint32_t)1 << (uint32_t)x);

    if (win_condition_up_down())
    {
      variables[1] = 100;
      variables[0] = 0;
      exit = true;
      return;
    }
  }

  last_pos = m.pos;

  current_player ^= 0b11;
}

void game_state::get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves)
{
  if (exit)
  {
    moves.clear();
    return;
  }

  if (moves.size() == 0 && exit != true)
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