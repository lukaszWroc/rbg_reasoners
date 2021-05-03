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
  pieces[current_player - 1] |= m.mr;

  if (win_condition() && exit_ == 0)
  {
    variables[current_player - 1] = 100;
    variables[(current_player ^ 0b11) - 1] = 0;
    exit_ = 1;
  }

  current_player ^= 0b11;
}

bool game_state::win_condition()
{
  return (pieces[current_player  -1] & 0b010010010) == 0b010010010 ||
    (pieces[current_player -1] & 0b100100100) == 0b100100100 ||
    (pieces[current_player -1] & 0b001001001) == 0b001001001 ||
    (pieces[current_player -1] & 0b111000000) == 0b111000000 ||
    (pieces[current_player -1] & 0b000111000) == 0b000111000 ||
    (pieces[current_player -1] & 0b000000111) == 0b000000111 ||
    (pieces[current_player -1] & 0b100010001) == 0b100010001 ||
    (pieces[current_player -1] & 0b001010100) == 0b001010100;
}

void game_state::get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves)
{
  moves.clear();

  if (exit_)
  {
    return;
  }

  uint32_t allMoves = (~(pieces[0] | pieces[1])) & 0x000001FF;

  if (allMoves == 0)
  {
    return;
  }

  while (allMoves)
  {
    uint32_t mv = msb(allMoves);
    moves.push_back(mv);
    allMoves ^= mv;
  }
}
}