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

bool game_state::rotation_win()
{
  if (win_condition((current_player ^ 0b11) - 1))
  {
    variables[current_player - 1]          = 0;
    variables[(current_player ^ 0b11) - 1] = 100;
    exit = true;

    return true;
  }

  if (win_condition(current_player -1))
  {
    variables[current_player - 1]          = 100;
    variables[(current_player ^ 0b11) - 1] = 0;
    exit = true;

    return true;
  }

  return false;
}

bool game_state::win_condition(uint32_t pl)
{
  for (int i=0;i<32;i++)
  {
    if (__builtin_popcountll(pieces[pl] & win_arr[i]) >= 5)
    {
      return true;
    }
  }

  return false;
}

void game_state::rotate(int idx, uint32_t pos, uint32_t type)
{
  uint64_t tmp = (pieces[idx] & quatter[pos]) >> diff[pos];

  uint64_t rotateRes;

  if (type)
  {
    rotateRes = rotate_right_arr[transform(tmp)];
  }
  else
  {
    rotateRes = rotate_left_arr[transform(tmp)];
  }

  pieces[idx] ^= (pieces[idx] & quatter[pos]);
  pieces[idx] |= (rotateRes << diff[pos]);
}

void game_state::rotate_all(uint32_t pos, uint32_t type)
{
  rotate(0, pos, type);
  rotate(1, pos, type);
}

void game_state::apply_move(const move &m)
{
  if (piece_move)
  {
    pieces[current_player - 1] |= (uint64_t)1 << m.mr;

    if (win_condition(current_player-1))
    {
      variables[current_player - 1] = 100;
      variables[(current_player ^ 0b11) - 1] = 0;
      exit = true;

      return;
    }

    piece_move = false;
  }
  else
  {
    rotate_all(m.mr, m.pos);

    if (rotation_win())
    {
      return;
    }

    piece_move = true;

    current_player ^= 0b11;
  }
}

void game_state::get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves)
{
  moves.clear();

  if (exit)
  {
    return;
  }

  if (piece_move)
  {
    board allMoves = (~(pieces[0] | pieces[1])) & 0b111111111111111111111111111111111111;

    while (allMoves)
    {
      uint32_t mv = msb(allMoves);

      moves.emplace_back(mv);

      allMoves ^= (uint64_t)1 << mv;
    }
  }
  else
  {
    moves.insert(moves.begin(), std::begin(rotates_vec), std::end(rotates_vec));
  }
}
}