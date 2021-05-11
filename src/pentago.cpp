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
bool game_state::rotation_win(uint32_t x)
{
  uint32_t tmp = last_moved;

  for(int i=0;i<9;i++)
  {
    if (pieces[(current_player ^ 0b11) - 1] & ((uint64_t)1 << index_pos[x][i]))
    {
      last_moved = index_pos[x][i];

      if (win_condition((current_player ^ 0b11) - 1))
      {
        last_moved = tmp;
        winner     = (current_player ^ 0b11);

        return true;
      }
    }

    if (pieces[current_player -1] & ((uint64_t)1 << index_pos[x][i]))
    {
      last_moved = index_pos[x][i];

      if (win_condition(current_player -1))
      {
        last_moved = tmp;
        winner     = current_player;

        return true;
      }
    }
  }

  last_moved = tmp;

  return false;
}

bool game_state::win_condition(uint32_t pl)
{
  uint32_t count = 1;

  uint32_t x = last_moved%BOARD_ROWS;
  uint32_t y = last_moved/BOARD_ROWS;

  for (int i=x-1;i>=0 && (pieces[pl] & ((uint64_t)1 << (y*BOARD_ROWS+i)));i--)
  {
    count++;
  }

  for (int i=x+1;i<BOARD_ROWS && (pieces[pl] & ((uint64_t)1 << (y*BOARD_ROWS+i)));i++)
  {
    count++;
  }

#ifdef STANDARD
  if (count == 5)
  {
    return true;
  }
#else
  if (count > 4)
  {
    return true;
  }
#endif

  count = 1;

  for (int i=y-1;i>=0 && (pieces[pl] & ((uint64_t)1 << (i*BOARD_ROWS+x)));i--)
  {
    count++;
  }

  for (int i=y+1;i<BOARD_ROWS && (pieces[pl] & ((uint64_t)1 << (i*BOARD_ROWS+x)));i++)
  {
    count++;
  }

#ifdef STANDARD
  if (count == 5)
  {
    return true;
  }
#else
  if (count > 4)
  {
    return true;
  }
#endif

  count = 1;

  int xt = x+1;
  int yt = y+1;

  while (xt < BOARD_ROWS && yt < BOARD_ROWS && (pieces[pl] & ((uint64_t)1 << (yt*BOARD_ROWS+xt))))
  {
    count++;
    xt++; yt++;
  }

  xt = x-1;
  yt = y-1;

  while (xt >= 0 && yt >= 0 && (pieces[pl] & ((uint64_t)1 << (yt*BOARD_ROWS+xt))))
  {
    count++;
    xt--; yt--;
  }

#ifdef STANDARD
  if (count == 5)
  {
    return true;
  }
#else
  if (count > 4)
  {
    return true;
  }
#endif

  count = 1;

  xt = x+1;
  yt = y-1;

  while (xt < BOARD_ROWS && yt >= 0 && (pieces[pl] & ((uint64_t)1 << (yt*BOARD_ROWS+xt))))
  {
    count++;
    xt++; yt--;
  }

  xt = x-1;
  yt = y+1;

  while (xt >= 0 && yt < BOARD_ROWS && (pieces[pl] & ((uint64_t)1 << (yt*BOARD_ROWS+xt))))
  {
    count++;
    xt--; yt++;
  }

#ifdef STANDARD
  if (count == 5)
  {
    return true;
  }
#else
  if (count > 4)
  {
    return true;
  }
#endif

  return false;
}

void game_state::rotate(int idx, uint32_t pos, uint32_t type)
{
  uint32_t tab[9];

  for (int i=0;i<9;i++)
  {
    tab[i] = index_pos[pos][i];
  }

  bool bool_tab[9];

  for (int i=0;i<9;i++)
  {
    bool_tab[i] = pieces[idx] & ((uint64_t)1 << tab[i]);
  }

  if (type)
  {
    rotate_right(bool_tab);
  }
  else
  {
    rotate_left(bool_tab);
  }

  for (int i=0;i<9;i++)
  {
    if (pieces[idx] & ((uint64_t)1 << tab[i]))
    {
      if (!bool_tab[i])
      {
        pieces[idx] ^= ((uint64_t)1 << tab[i]);
      }
    }
    else if (bool_tab[i])
    {
      pieces[idx] |= ((uint64_t)1 << tab[i]);
    }
  }
}

void game_state::rotate_all(uint32_t pos, uint32_t type)
{
  rotate(0,pos,type);
  rotate(1,pos,type);
}

void game_state::rotate_right(bool *tab)
{
  uint32_t t[9] = {6,3,0,7,4,1,8,5,2};

  bool tmp[9] = {0};

  for(int i=0;i<9;i++)
    tmp[i] = tab[i];

  for(int i=0;i<9;i++)
    tab[i] = tmp[t[i]];
}

void game_state::rotate_left(bool *tab)
{
  uint32_t t[9]  = {2,5,8,1,4,7,0,3,6};

  bool tmp[9] = {0};

  for(int i=0;i<9;i++)
    tmp[i] = tab[i];

  for(int i=0;i<9;i++)
    tab[i] = tmp[t[i]];
}

void game_state::rotate_right(uint32_t *tab)
{
  uint32_t t[9] = {6,3,0,7,4,1,8,5,2};

  uint32_t tmp[9] = {0};

  for(int i=0;i<9;i++)
    tmp[i] = tab[i];

  for(int i=0;i<9;i++)
    tab[i] = tmp[t[i]];
}

void game_state::rotate_left(uint32_t *tab)
{
  uint32_t t[9]  = {2,5,8,1,4,7,0,3,6};

  uint32_t tmp[9] = {0};

  for(int i=0;i<9;i++)
    tmp[i] = tab[i];

  for(int i=0;i<9;i++)
    tab[i] = tmp[t[i]];
}

int game_state::get_quarter(uint32_t pos)
{
  if (pos <=2 || (6 <= pos && pos <= 8) || (12 <= pos && pos <= 14))
    return 0;
  if ((3 <= pos && pos <= 5) || (9 <= pos && pos <= 11) || (15 <= pos && pos <= 17))
    return 1;
  if ((18 <= pos && pos <= 20) || (24 <= pos && pos <= 26) || (30 <= pos && pos <= 32))
    return 2;

  return 3;
}

uint32_t game_state::get_pos(uint32_t pos)
{
  int x   = get_quarter(pos);
  int cnt = rotates_cnt[x];

  uint32_t tab[9];

  for (int i=0;i<9;i++)
  {
    tab[i] = index_pos[x][i];
  }

  if (cnt > 0)
  {
    cnt = cnt&0b11;

    while (cnt)
    {
      rotate_right(tab);
      cnt--;
    }
  }
  else if (cnt < 0)
  {
    cnt  = (cnt*-1)&0b11;

    while (cnt)
    {
      rotate_left(tab);
      cnt--;
    }
  }

  for (int i=0;i<9;i++)
  {
    if (tab[i] == pos)
    {
      return index_pos[x][i];
    }
  }

  return 0;
}

void game_state::apply_move(const move &m)
{
  if (piece_move)
  {
    uint32_t new_pos = get_pos(m.mr);

    last_moved = new_pos;

    pieces[current_player - 1] ^= ((uint64_t)1 << new_pos);

    if (win_condition(current_player -1))
    {
      variables[current_player - 1] = 100;
      variables[(current_player ^ 0b11) - 1] = 0;
      exit = true;
      return;
    }

    last_pos   = m.pos;
    piece_move = false;
  }
  else
  {
    if (m.pos)
    {
      rotates_cnt[m.mr]++;
    }
    else
    {
      rotates_cnt[m.mr]--;
    }

    rotate_all(m.mr,m.pos);

    if (rotation_win(m.mr))
    {
      variables[winner - 1] = 100;
      variables[(winner ^ 0b11) - 1] = 0;
      exit = true;
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
    if (empty.size() == 1)
    {
      return;
    }

    if (played)
    {
      size_t cnt = empty.size()-1;

      if (last_pos != cnt)
      {
        empty[last_pos].mr ^= empty[cnt].mr;
        empty[cnt].mr      ^= empty[last_pos].mr;
        empty[last_pos].mr ^= empty[cnt].mr;

        empty[last_pos].pos = last_pos;
      }

      empty.pop_back();
    }
    else
    {
      empty.insert(empty.begin(), std::begin(empty_array), std::end(empty_array));
      played = true;
    }

    moves.insert(moves.begin(), std::begin(empty), std::end(empty));
  }
  else
  {
    moves.insert(moves.begin(), std::begin(rotates_vec), std::end(rotates_vec));
  }
}
}