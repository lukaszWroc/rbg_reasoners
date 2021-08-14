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

void game_state::getFree(board &right, board &left, board &tmp)
{
  for (int i=0;i<right.N;i++)
  {
    tmp.date[i] = ~(right.date[i] | left.date[i]);
  }

  tmp.date[BOARD_SIZE >> 6] &= (((uint64_t)1 << (BOARD_SIZE & bit_mask)) - 1);
}

void game_state::delete1(uint32_t node)
{
  board &b = taken_board[node];

  board *my, *other;

  if (current_player == 2)
  {
    other = &whiteBoard;
    my    = &blackBoard;
  }
  else
  {
    other = &blackBoard;
    my    = &whiteBoard;
  }

  board fixed;

  for (int i=0;i<b.N;i++)
  {
    uint64_t &allMoves = b.date[i];

    while (allMoves)
    {
      uint64_t mv = msb(allMoves);

      uint32_t pos = getPos(mv, i);

      other -> unset(pos);

      leader[pos] = pos;
      range[pos]  = 1;

      allMoves ^= mv;

      pos = (pos << 2);

      for (int j=0;j<4;j++)
      {
        uint32_t tmpPos = neighbors[pos + j];

        if (tmpPos == BLOCK)
        {
          break;
        }

        if (my -> get(tmpPos))
        {
          liberty_board[find(tmpPos)].set(pos >> 2);
          fixed.set(find(tmpPos));
        }
      }
    }
  }

  for (int i=0;i<fixed.N;i++)
  {
    uint64_t &allMoves = fixed.date[i];

    while (allMoves)
    {
      uint64_t mv = msb(allMoves);
      liberty[getPos(mv, i)] = count_pieces(liberty_board[getPos(mv, i)]);
      allMoves ^= mv;
    }
  }
}

uint32_t game_state::count_pieces(board &b)
{
  uint32_t res = 0;

  for (int i=0;i<b.N;i++)
  {
    res += __builtin_popcountll(b.date[i]);
  }

  return res;
}

void game_state::apply_move(const move &m)
{
  if (m.mr == PASS)
  {
    passed[current_player] = true;
    current_player ^= 0b11;
    return;
  }

  passed[1] = false;
  passed[2] = false;

  if (current_player == 1)
  {
    update(whiteBoard, blackBoard, m);
  }
  else
  {
    update(blackBoard, whiteBoard, m);
  }

  liberty[find(m.mr)] = count_pieces(liberty_board[find(m.mr)]);

  current_player ^= 0b11;
}

void game_state::get_points()
{
  for (int i=0;i<BOARD_SIZE;i++)
  {
    if (!blackBoard.get(i) && !whiteBoard.get(i) && !vis[i])
    {
      dfs(i, i);
    }
  }

  for (int i=0;i<BOARD_SIZE;i++)
  {
    if (!blackBoard.get(i) && !whiteBoard.get(i))
    {
      if (visBlack[find(i)] && !visWhite[find(i)] && !alreadyCounted[find(i)])
      {
        variables[1] += count_pieces(taken_board[find(i)]);
        alreadyCounted[find(i)] = true;
      }
      else if (!visBlack[find(i)] && visWhite[find(i)] && !alreadyCounted[find(i)])
      {
        variables[0] += count_pieces(taken_board[find(i)]);
        alreadyCounted[find(i)] = true;
      }
    }
    else if (blackBoard.get(find(i)) && !alreadyCounted[find(i)])
    {
      variables[1] += count_pieces(taken_board[find(i)]);
      alreadyCounted[find(i)] = true;
    }
    else if (whiteBoard.get(find(i)) && !alreadyCounted[find(i)])
    {
      variables[0] += count_pieces(taken_board[find(i)]);
      alreadyCounted[find(i)] = true;
    }
  }
}

void game_state::prepare(std::vector<move>& moves, board &my, board& other)
{
  board free;

  getFree(my, other, free);

  for (int i=0;i<free.N;i++)
  {
    uint64_t allMoves = free.date[i];

    while (allMoves)
    {
      uint64_t mv = msb(allMoves);

      uint32_t pos = getPos(mv, i);

      for (int j=0;j<4;j++)
      {
        uint32_t tmp = neighbors[(pos << 2) + j];

        if (tmp == BLOCK)
        {
          break;
        }

        if (free.get(tmp))
        {
          moves.emplace_back(pos);
          break;
        }

        if (other.get(tmp) && liberty[find(tmp)] == 1)
        {
          moves.emplace_back(pos);
          break;
        }

        if (my.get(tmp))
        {
          if (liberty[find(tmp)] > 1)
          {
            moves.emplace_back(pos);
            break;
          }
        }
      }

      allMoves ^= mv;
    }
  }
}

void game_state::dfs(int node, int parent)
{
  vis[node] = true;

  taken_board[parent].set(node);

  for (int i=0;i<4;i++)
  {
    uint32_t pos = neighbors[(node << 2) + i];

    if (pos == BLOCK)
    {
      break;
    }

    if (blackBoard.get(pos))
    {
      visBlack[parent] = true;
    }
    else if (whiteBoard.get(pos))
    {
      visWhite[parent] = true;
    }
    else if (!vis[pos])
    {
      dfs(pos, parent);
    }
  }
}

void game_state::get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves)
{
  moves.clear();

  if (exit)
  {
    return;
  }

  if ((passed[1] && passed[2]) || turn_limit == TURN_LIMIT)
  {
    get_points();

    exit = true;

    return;
  }

  turn_limit++;

  if (current_player == 1)
  {
    prepare(moves, whiteBoard, blackBoard);
  }
  else
  {
    prepare(moves, blackBoard, whiteBoard);
  }

  #ifdef NO_PASS
  if (moves.size() == 0)
  {
    moves.emplace_back(PASS);
  }
  #else
  moves.emplace_back(PASS);
  #endif
}


void game_state::update(board &my, board &other, const move &m)
{
  my.set(m.mr);

  uint32_t tmp[4];
  size_t cnt = 0;

  uint32_t deleteTmp[4];
  size_t cntDelete = 0;

  board parents;
  board notTaken;

  for (int i=0;i<4;i++)
  {
    uint32_t pos = neighbors[(m.mr << 2) + i];

    if (pos == BLOCK)
    {
      break;
    }

    if (other.get(pos))
    {
      uint32_t fPos = find(pos);

      if (!parents.get(fPos))
      {
        liberty_board[fPos].unset(m.mr);
        liberty[fPos]--;
        parents.set(fPos);

        if (liberty[fPos] == 0)
        {
          deleteTmp[cntDelete++] = fPos;
        }
      }
    }
    else if (my.get(pos))
    {
      tmp[cnt++] = pos;
    }
    else
    {
      notTaken.set(pos);
    }
  }

  liberty_board[m.mr] = notTaken;

  if (cnt > 0)
  {
    uint32_t parent = find(tmp[0]);

    for (size_t i=1;i<cnt;i++)
    {
      uint32_t pp = find(tmp[i]);

      if (parent != pp)
      {
        if (range[pp] > range[parent])
        {
          pp     ^= parent;
          parent ^= pp;
          pp     ^= parent;
        }

        merge1(liberty_board[parent], liberty_board[pp]);
        merge1(taken_board[parent], taken_board[pp]);
        union1(pp, parent);
      }
    }

    merge1(liberty_board[parent], liberty_board[m.mr]);
    merge1(taken_board[parent], m.mr);
    union1(m.mr, parent);

    liberty_board[parent].unset(m.mr);
  }
  else
  {
    taken_board[m.mr].set(m.mr);
  }

  for (size_t i=0;i<cntDelete;i++)
  {
    delete1(find(deleteTmp[i]));
  }
}
}