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

uint32_t game_state::fix_liberty(board &tmp)
{
  board free;

  getFree(whiteBoard, blackBoard, free);

  board freeTmp;

  for (int i=0;i<tmp.N;i++)
  {
    uint64_t allMoves = tmp.date[i];

    while (allMoves)
    {
      uint64_t mv = msb(allMoves);

      uint32_t pos(getPos(mv, i));

      pos = (pos << 2);

      for (int j=0;j<4;j++)
      {
        uint32_t tmpPos = neighbors[pos + j];

        if (tmpPos == BLOCK)
        {
          break;
        }

        if (free.get(tmpPos))
        {
          freeTmp.set(tmpPos);
        }
      }

      allMoves ^= mv;
    }
  }

  return count_pieces(freeTmp);
}

void game_state::delete1(uint32_t node)
{
  board b = liberty_board[find(node)];

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

  for (int i=0;i<b.N;i++)
  {
    uint64_t allMoves = b.date[i];

    while (allMoves)
    {
      uint64_t mv = msb(allMoves);

      uint32_t pos = getPos(mv, i);

      other -> unset(pos);

      liberty_board[pos] = libert_board_init[pos];

      leader[pos]    = pos;
      range[pos] = 1;

      allMoves ^= mv;
    }
  }

  board fixed;

  for (int i=0;i<b.N;i++)
  {
    uint64_t allMoves = b.date[i];

    while (allMoves)
    {
      uint64_t mv = msb(allMoves);

      uint32_t pos(getPos(mv, i));

      pos = (pos << 2);

      for (int j=0;j<4;j++)
      {
        uint32_t tmpPos = neighbors[pos + j];

        if (tmpPos == BLOCK)
        {
          break;
        }

        if (my -> get(tmpPos) && !fixed.get(find(tmpPos)))
        {
          liberty[find(tmpPos)] = fix_liberty(liberty_board[find(tmpPos)]);

          fixed.set(find(tmpPos));
        }
      }

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

  liberty[find(m.mr)] = fix_liberty(liberty_board[find(m.mr)]);

  current_player ^= 0b11;
}

void game_state::get_points()
{
  variables[0] = score(whiteBoard);
  variables[1] = score(blackBoard);
}

void game_state::prepare(std::vector<move>& moves, board &my, board& other)
{
  board free;

  getFree(my,other, free);

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

        if (tmp != BLOCK)
        {
          if (free.get(tmp))
          {
            moves.push_back(pos);
            break;
          }

          if (other.get(tmp) && liberty[find(tmp)] == 1)
          {
            moves.push_back(pos);
            break;
          }

          if (my.get(tmp))
          {
            board b = liberty_board[find(tmp)];

            if (fix_liberty(b) > 1)
            {
              move m(pos);
              moves.push_back(m);
              break;
            }
          }
        }
      }

      allMoves ^= mv;
    }
  }
}

void game_state::dfs(int i, int j, board &b, board &cb)
{
  if ((0 <= i && i < BOARD_ROWS) && (0 <=j-1 && j-1 <BOARD_ROWS))
  {
    if (!b.get(i*BOARD_ROWS+j-1) && !cb.get(i*BOARD_ROWS+j-1))
    {
      b.set(i*BOARD_ROWS+j-1);
      dfs(i,j-1,b,cb);
    }
  }

  if ((0 <= i && i < BOARD_ROWS) && (0 <=j+1 && j+1 <BOARD_ROWS))
  {
    if (!b.get(i*BOARD_ROWS+j+1) && !cb.get(i*BOARD_ROWS+j+1))
    {
      b.set(i*BOARD_ROWS+j+1);
      dfs(i,j+1,b,cb);
    }
  }

  if ((0 <= i-1 && i-1 < BOARD_ROWS) && (0 <=j && j <BOARD_ROWS))
  {
    if (!b.get((i-1)*BOARD_ROWS+j) && !cb.get((i-1)*BOARD_ROWS+j))
    {
      b.set((i-1)*BOARD_ROWS+j);
      dfs(i-1,j,b,cb);
    }
  }

  if ((0 <= i+1 && i+1 < BOARD_ROWS) && (0 <=j && j <BOARD_ROWS))
  {
    if (!b.get((i+1)*BOARD_ROWS+j) && !cb.get((i+1)*BOARD_ROWS+j))
    {
      b.set((i+1)*BOARD_ROWS+j);
      dfs(i+1,j,b,cb);
    }
  }
}

int game_state::score(board &cb)
{
  board &b = cb;
  board empty;
  board nb;

  getFree(b, empty, nb);

  for (int i=0;i<BOARD_ROWS;i++)
  {
    uint32_t pos = i;
    if (!empty.get(pos))
    {
      empty.set(pos);
      dfs(0, i, empty, cb);
    }
  }

  for (int i=0;i<BOARD_ROWS;i++)
  {
    uint32_t pos = ((BOARD_ROWS-1) * BOARD_ROWS) + i;
    if (!empty.get(pos))
    {
      empty.set(pos);
      dfs((BOARD_ROWS-1), i, empty, cb);
    }
  }

  for (int i=0;i<BOARD_ROWS;i++)
  {
    uint32_t pos = i*BOARD_ROWS;

    if (!empty.get(pos))
    {
      empty.set(pos);
      dfs(i, 0, empty, cb);
    }
  }

  for (int i=0;i<BOARD_ROWS;i++)
  {
    uint32_t pos = i*BOARD_ROWS+BOARD_ROWS-1;

    if (!empty.get(pos))
    {
      empty.set(pos);
      dfs(i, BOARD_ROWS-1, empty, cb);
    }
  }

  int numberOfPieces           = count_pieces(cb);
  int numberOfOutterAreaPieces = count_pieces(empty);
  int numberOfInnerAreaPieces  = BOARD_SIZE - numberOfPieces - numberOfOutterAreaPieces;

  return numberOfPieces + numberOfInnerAreaPieces;
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
    // get_points();
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
    moves.push_back(PASS);
  }
  #else
  moves.push_back(PASS);
  #endif
}


void game_state::update(board &my, board &other, const move &m)
{
  my.set(m.mr);

  uint32_t tmp[4];
  size_t cnt = 0;

  board parents;

  for (int i=0;i<4;i++)
  {
    uint32_t pos = neighbors[(m.mr << 2) + i];

    if (pos == BLOCK)
    {
      break;
    }

    uint32_t fPos = find(pos);

    if (other.get(pos))
    {
      if (!parents.get(fPos))
      {
        liberty[fPos]--;
        parents.set(fPos);

        if (liberty[fPos] == 0)
        {
          delete1(fPos);
        }
      }
    }
    else if (my.get(pos))
    {
      tmp[cnt++] = pos;
    }
  }

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
        union1(pp, parent);
      }
    }

    merge1(liberty_board[parent], m.mr);
    union1(m.mr, parent);
  }
  else
  {
    board tmp;
    tmp.set(m.mr);
    liberty_board[m.mr] = tmp;
  }
}
}