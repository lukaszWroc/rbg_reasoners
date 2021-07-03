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

bool game_state::win_condition_up_down()
{
  uint16_t mask = up_down[0];

  for (int i=1;i<BOARD_ROWS;i++)
  {
    mask = mask_to_mask[((uint32_t)mask << BOARD_ROWS) + up_down[i]];
  }

  return mask;
}


bool game_state::win_condition_left_right()
{
  uint16_t mask = left_right[0];

  for (int i=1;i<BOARD_ROWS;i++)
  {
    mask = mask_to_mask[((uint32_t)mask << BOARD_ROWS) + left_right[i]];
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
  moves.clear();

  if (exit)
  {
    return;
  }

  if (last_pos == 100)
  {
    moves.insert(moves.begin(), std::begin(empty_array), std::end(empty_array));
    return;
  }

  size_t cnt = empty.size()-1;

  empty[last_pos].mr ^= empty[cnt].mr;
  empty[cnt].mr      ^= empty[last_pos].mr;
  empty[last_pos].mr ^= empty[cnt].mr;
  empty.pop_back();

  moves.insert(moves.begin(), empty.begin(), empty.end());
}
}