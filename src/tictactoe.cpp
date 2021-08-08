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
  if (exit)
  {
    return -1;
  }

  return 0;
}

bool game_state::is_legal([[maybe_unused]] const move& m) const
{
  #ifdef MONOTONIC
  return !(pieces[0] & m.mr) && !(pieces[1] & m.mr) && !exit;
  #else
  return -1;
  #endif
}

void game_state::apply_move(const move &m)
{
  pieces[current_player - 1] |= m.mr;


  #ifndef MONOTONIC
  last_pos = m.pos;
  #endif

  if (win_arr[pieces[current_player  -1]])
  {
    variables[current_player - 1] = 100;
    variables[(current_player ^ 0b11) - 1] = 0;
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

  if (last_pos == not_started)
  {
    moves.insert(moves.begin(), std::begin(empty_arr), std::end(empty_arr));
    #ifndef MONOTONIC
    empty_vec.insert(empty_vec.begin(), std::begin(empty_arr), std::end(empty_arr));
    #endif

    return;
  }

  #ifndef MONOTONIC
  if (empty_vec.size() == 1)
  {
    return;
  }

  empty_vec[last_pos].mr ^= empty_vec.back().mr;
  empty_vec.back().mr    ^= empty_vec[last_pos].mr;
  empty_vec[last_pos].mr ^= empty_vec.back().mr;

  empty_vec.pop_back();

  moves.insert(moves.begin(), std::begin(empty_vec), std::end(empty_vec));
  #endif
}
}