#include "reasoner.hpp"

namespace reasoner
{
int game_state::get_current_player(void) const
{
  return current_player;
}

int game_state::get_player_score(int player_id) const
{
  return variables[player_id];
}

bool game_state::apply_any_move(resettable_bitarray_stack&)
{
  return false;
}

int game_state::get_monotonicity_class(void)
{
  if (exit)
  {
    return -1;
  }

  return 0;
}

bool game_state::is_legal([[maybe_unused]] const move& m) const
{
  #ifdef MONOTONIC
  return !(pieces[2] & m.mr) && !(pieces[1] & m.mr) && !exit;
  #else
  return -1;
  #endif
}

void game_state::apply_move(const move &m)
{
  pieces[current_player] |= m.mr;

  if (win_arr[pieces[current_player]])
  {
    variables[current_player]        = 100;
    variables[current_player ^ 0b11] = 0;
    exit = 1;
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

  moves.insert(moves.begin(), &all_moves[(pieces[2] | pieces[1])*BOARD_SIZE],
    &all_moves[(pieces[2] | pieces[1])*BOARD_SIZE + all_moves_cnt[(pieces[2] | pieces[1])]]);
}
}