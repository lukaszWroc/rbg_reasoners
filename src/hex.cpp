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
  return !redBoard.get(m.mr) && !blueBoard.get(m.mr) && !exit;
  #else
  return -1;
  #endif
}

void game_state::fix_win_condition(uint32_t pos, uint32_t &last_leader)
{
  uint32_t lead = find(pos);

  if (range[lead] > range[last_leader])
  {
    last_leader ^= lead;
    lead        ^= last_leader;
    last_leader ^= lead;
  }

  merge1(taken_board[last_leader], taken_board[lead]);
  union1(lead, last_leader);

  firstEdgeArray[last_leader]  = firstEdgeArray[last_leader]  || firstEdgeArray[lead];
  secondEdgeArray[last_leader] = secondEdgeArray[last_leader] || secondEdgeArray[lead];
}

void game_state::win_condition_blue(uint32_t m)
{
  uint32_t c = m/BOARD_ROWS2;

  uint32_t last_leader = m;

  taken_board[m].set(m);

  if (c == 1)
  {
    firstEdgeArray[m] = true;
  }
  else if (c == BOARD_ROWS2-2)
  {
    secondEdgeArray[m] = true;
  }

  if (blueBoard.get(m-1))
  {
    fix_win_condition(m-1, last_leader);
  }

  if (blueBoard.get(m+1))
  {
    fix_win_condition(m+1, last_leader);
  }

  if (blueBoard.get(m+BOARD_ROWS2))
  {
    fix_win_condition(m+BOARD_ROWS2, last_leader);
  }

  if (blueBoard.get(m-BOARD_ROWS2))
  {
    fix_win_condition(m-BOARD_ROWS2, last_leader);
  }

  if (blueBoard.get(m+BOARD_ROWS2+1))
  {
    fix_win_condition(m+BOARD_ROWS2+1, last_leader);
  }

  if (blueBoard.get(m-BOARD_ROWS2-1))
  {
    fix_win_condition(m-BOARD_ROWS2-1, last_leader);
  }

  firstEdge  = firstEdgeArray[last_leader];
  secondEdge = secondEdgeArray[last_leader];
}

void game_state::win_condition_red(uint32_t m)
{
  uint32_t c = m%BOARD_ROWS2;

  uint32_t last_leader = m;

  taken_board[m].set(m);

  if (c == 1)
  {
    firstEdgeArray[m] = true;
  }
  else if (c == BOARD_ROWS2-2)
  {
    secondEdgeArray[m] = true;
  }

  if (redBoard.get(m-1))
  {
    fix_win_condition(m-1, last_leader);
  }

  if (redBoard.get(m+1))
  {
    fix_win_condition(m+1, last_leader);
  }

  if (redBoard.get(m+BOARD_ROWS2))
  {
    fix_win_condition(m+BOARD_ROWS2, last_leader);
  }

  if (redBoard.get(m-BOARD_ROWS2))
  {
    fix_win_condition(m-BOARD_ROWS2, last_leader);
  }

  if (redBoard.get(m+BOARD_ROWS2+1))
  {
    fix_win_condition(m+BOARD_ROWS2+1, last_leader);
  }

  if (redBoard.get(m-BOARD_ROWS2-1))
  {
    fix_win_condition(m-BOARD_ROWS2-1, last_leader);
  }

  firstEdge  = firstEdgeArray[last_leader];
  secondEdge = secondEdgeArray[last_leader];
}

void game_state::apply_move(const move &m)
{
  if (current_player == 1)
  {
    blueBoard.set(m.mr);

    firstEdge  = false;
    secondEdge = false;

    win_condition_blue(m.mr);

    if (firstEdge && secondEdge)
    {
      variables[0] = 100;
      variables[1] = 0;
      exit = true;
      return;
    }
  }
  else
  {
    redBoard.set(m.mr);

    firstEdge  = false;
    secondEdge = false;

    win_condition_red(m.mr);

    if (firstEdge && secondEdge)
    {
      variables[1] = 100;
      variables[0] = 0;
      exit = true;
      return;
    }
  }

  #ifndef MONOTONIC
  last_pos = m.pos;
  #endif

  current_player ^= 0b11;
}

void game_state::get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves)
{
  moves.clear();

  if (exit)
  {
    return;
  }

  if (last_pos == BOARD_SIZE2 + 1)
  {
    moves.insert(moves.begin(), std::begin(empty_array), std::end(empty_array));
    return;
  }

  #ifndef MONOTONIC
  size_t cnt = empty.size()-1;

  empty[last_pos].mr ^= empty[cnt].mr;
  empty[cnt].mr      ^= empty[last_pos].mr;
  empty[last_pos].mr ^= empty[cnt].mr;
  empty.pop_back();

  moves.insert(moves.begin(), empty.begin(), empty.end());
  #endif
}
}