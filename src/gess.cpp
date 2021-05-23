#include "reasoner.hpp"

namespace reasoner
{

inline int min(int a, int b)
{
  if (a < b)
    return a;
  return b;
}

inline int max(int a, int b)
{
  if (a < b)
    return b;
  return a;
}

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

inline bool game_state::check(uint32_t pos, uint32_t player)
{
  board &b = pieces[player];

  int x = pos%BOARD_ROWS;
  int y = pos/BOARD_ROWS;

  if (b.get(pos))
  {
    return false;
  }

  if ((x+1<(BOARD_ROWS-1) && b.get(y*BOARD_ROWS+x+1)) &&
      (x-1>0 && b.get(y*BOARD_ROWS+x-1)) &&
      (y+1<(BOARD_ROWS-1) && b.get((y+1)*BOARD_ROWS+x)) &&
      (y-1>0 && b.get((y-1)*BOARD_ROWS+x)) &&
      (y+1<(BOARD_ROWS-1) && x+1<(BOARD_ROWS-1) && b.get((y+1)*BOARD_ROWS+x+1)) &&
      (y-1>0 && x-1>0 && b.get((y-1)*BOARD_ROWS+x-1)) &&
      (y+1<(BOARD_ROWS-1) && x-1>0 && b.get((y+1)*BOARD_ROWS+x-1)) &&
      (y-1>0 && x+1<(BOARD_ROWS-1) && b.get((y-1)*BOARD_ROWS+x+1)))
  {
    return true;
  }

  return false;
}

bool game_state::win_condition(uint32_t player)
{
  while (king[player].size())
  {
    if (check(king[player].back(),player))
    {
      return false;
    }

    king[player].pop_back();
  }

  return true;
}

void game_state::clear_pos(uint32_t pos, uint32_t player)
{
  board &b = pieces[player];

  if (b.get(pos))
  {
    b.reset(pos);
  }

  int x = pos%BOARD_ROWS;
  int y = pos/BOARD_ROWS;

  if (x+1<(BOARD_ROWS-1) && b.get(y*BOARD_ROWS+x+1))
  {
    b.reset(y*BOARD_ROWS+x+1);
  }
  if (x-1>0 && b.get(y*BOARD_ROWS+x-1))
  {
    b.reset(y*BOARD_ROWS+x-1);
  }

  if (y+1<(BOARD_ROWS-1) && b.get((y+1)*BOARD_ROWS+x))
  {
    b.reset((y+1)*BOARD_ROWS+x);
  }
  if (y-1>0 && b.get((y-1)*BOARD_ROWS+x))
  {
    b.reset((y-1)*BOARD_ROWS+x);
  }

  if (y+1<(BOARD_ROWS-1) && x+1<(BOARD_ROWS-1) && b.get((y+1)*BOARD_ROWS+x+1))
  {
    b.reset((y+1)*BOARD_ROWS+x+1);
  }
  if (y-1>0 && x-1>0 && b.get((y-1)*BOARD_ROWS+x-1))
  {
    b.reset((y-1)*BOARD_ROWS+x-1);
  }

  if (y+1<(BOARD_ROWS-1) && x-1>0 && b.get((y+1)*BOARD_ROWS+x-1))
  {
    b.reset((y+1)*BOARD_ROWS+x-1);
  }
  if (y-1>0 && x+1<(BOARD_ROWS-1) && b.get((y-1)*BOARD_ROWS+x+1))
  {
    b.reset((y-1)*BOARD_ROWS+x+1);
  }
}

void game_state::set_pos(uint32_t start, uint32_t pos, board bcp)
{
  board &b = pieces[current_player-1];

  if (bcp.get(start))
  {
    b.set(pos);
  }

  int x = pos%BOARD_ROWS;
  int y = pos/BOARD_ROWS;

  int xs = start%BOARD_ROWS;
  int ys = start/BOARD_ROWS;

  if (x+1<(BOARD_ROWS-1) && xs+1<(BOARD_ROWS-1) && bcp.get(ys*BOARD_ROWS+xs+1))
  {
    b.set(y*BOARD_ROWS+x+1);
  }

  if (x-1>0 &&  xs-1>0 &&  bcp.get(ys*BOARD_ROWS+xs-1))
  {
    b.set(y*BOARD_ROWS+x-1);
  }

  if (y+1<(BOARD_ROWS-1) && ys+1<(BOARD_ROWS-1) && bcp.get((ys+1)*BOARD_ROWS+xs))
  {
    b.set((y+1)*BOARD_ROWS+x);
  }

  if (y-1>0 &&  ys-1>0 &&  bcp.get((ys-1)*BOARD_ROWS+xs))
  {
    b.set((y-1)*BOARD_ROWS+x);
  }

  if ( y+1<(BOARD_ROWS-1) && x+1<(BOARD_ROWS-1) &&  ys+1<(BOARD_ROWS-1) && xs+1<(BOARD_ROWS-1) && bcp.get((ys+1)*BOARD_ROWS+xs+1))
  {
    b.set((y+1)*BOARD_ROWS+x+1);
  }

  if (y-1>0 && x-1>0  && ys-1>0 && xs-1>0 && bcp.get((ys-1)*BOARD_ROWS+xs-1))
  {
    b.set((y-1)*BOARD_ROWS+x-1);
  }

  if (y+1<(BOARD_ROWS-1) && x-1>0 && ys+1<(BOARD_ROWS-1) && xs-1>0 && bcp.get((ys+1)*BOARD_ROWS+xs-1))
  {
    b.set((y+1)*BOARD_ROWS+x-1);
  }

  if (y-1>0 && x+1<(BOARD_ROWS-1) &&  ys-1>0 && xs+1<(BOARD_ROWS-1) &&  bcp.get((ys-1)*BOARD_ROWS+xs+1))
  {
    b.set((y-1)*BOARD_ROWS+x+1);
  }
}

void game_state::update_king(int pos)
{
  int x = pos%BOARD_ROWS;
  int y = pos/BOARD_ROWS;

  for (int i=-3;i<=3;i++)
  {
    for (int j=-3;j<=3;j++)
    {
      if ( 0 < y +i && y + i< BOARD_ROWS && 0 < x +j && x +j < BOARD_ROWS)
      {
        if (check((y+i)*BOARD_ROWS + x + j, current_player-1))
        {
          king[current_player-1].push_back((y+i)*BOARD_ROWS + x + j);
        }
      }
    }
  }
}

void game_state::apply_move(const move &m)
{
  board bcp = pieces[current_player-1];
  clear_pos(m.end, (current_player ^ 0b11) - 1);
  clear_pos(m.end, current_player-1);
  clear_pos(m.start, current_player-1);

  set_pos(m.start, m.end, bcp);
  update_king(m.end);

  if (win_condition((current_player ^ 0b11) - 1))
  {
    variables[current_player - 1] = 100;
    variables[(current_player ^ 0b11) - 1] = 0;
    exit = true;
    return;
  }

  if (win_condition(current_player -1))
  {
    variables[current_player -1] = 0;
    variables[(current_player ^ 0b11) - 1] = 100;
    exit = true;
    return;
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

  create_moves(moves);
}


void game_state::add_moves(footprint &fp, std::vector<move>& moves)
{
  board &b = pieces[current_player -1];
  board &bo = pieces[(current_player ^ 0b11) - 1];

  int x = fp.pos%BOARD_ROWS;
  int y = fp.pos/BOARD_ROWS;

  if (fp.u)
  {
    int cnt = fp.cnt;
    int yt = y;
    int xt = x;

    while (--cnt && ++yt < (BOARD_ROWS-1))
    {
      moves.emplace_back(fp.pos, yt*BOARD_ROWS+xt);

      if (yt + 1 < (BOARD_ROWS-1))
      {
        if (b.get((yt+1)*BOARD_ROWS+xt) || b.get((yt+1)*BOARD_ROWS+max(1,xt-1)) || b.get((yt+1)*BOARD_ROWS+min(xt+1,BOARD_ROWS-1)))
        {
          break;
        }
        if (bo.get((yt+1)*BOARD_ROWS+xt) || bo.get((yt+1)*BOARD_ROWS+max(1,xt-1)) || bo.get((yt+1)*BOARD_ROWS+min(xt+1,BOARD_ROWS-1)))
        {
          break;
        }
      }
    }
  }

  if (fp.d)
  {
    int cnt = fp.cnt;
    int yt = y;
    int xt = x;

    while (--cnt && --yt >0)
    {
      moves.emplace_back(fp.pos, yt*BOARD_ROWS+xt);
      if (yt -1 >0)
      {
        if (b.get((yt-1)*BOARD_ROWS+xt) || b.get((yt-1)*BOARD_ROWS+max(1,xt-1)) || b.get((yt-1)*BOARD_ROWS+min(xt+1,BOARD_ROWS-1)))
        {
          break;
        }
        if (bo.get((yt-1)*BOARD_ROWS+xt) || bo.get((yt-1)*BOARD_ROWS+max(1,xt-1)) || bo.get((yt-1)*BOARD_ROWS+min(xt+1,BOARD_ROWS-1)))
        {
          break;
        }
      }
    }
  }

  if (fp.l)
  {
    int cnt = fp.cnt;
    int yt = y;
    int xt = x;

    while (--cnt && --xt >0)
    {
      moves.emplace_back(fp.pos, yt*BOARD_ROWS+xt);
      if (xt -1 >0)
      {
        if (b.get(yt*BOARD_ROWS+xt-1) || b.get(max(yt-1,1)*BOARD_ROWS+xt-1) || b.get(min(yt+1,BOARD_ROWS-1)*BOARD_ROWS+xt-1))
        {
          break;
        }
        if (bo.get(yt*BOARD_ROWS+xt-1) || bo.get(max(yt-1,1)*BOARD_ROWS+xt-1) || bo.get(min(yt+1,BOARD_ROWS-1)*BOARD_ROWS+xt-1))
        {
          break;
        }
      }
    }
  }

  if (fp.r)
  {
    int cnt = fp.cnt;
    int yt = y;
    int xt = x;

    while (--cnt && ++xt < (BOARD_ROWS-1))
    {
      moves.emplace_back(fp.pos, yt*BOARD_ROWS+xt);
      if (xt+1 < (BOARD_ROWS-1))
      {
        if (b.get(yt*BOARD_ROWS+xt+1) || b.get(max(yt-1,1)*BOARD_ROWS+xt+1) || b.get(min(yt+1,BOARD_ROWS-1)*BOARD_ROWS+xt+1))
        {
          break;
        }
        if (bo.get(yt*BOARD_ROWS+xt+1) || bo.get(max(yt-1,1)*BOARD_ROWS+xt+1) || bo.get(min(yt+1,BOARD_ROWS-1)*BOARD_ROWS+xt+1))
        {
          break;
        }
      }
    }
  }

  if (fp.ur)
  {
    int cnt = fp.cnt;
    int yt = y;
    int xt = x;
    while (--cnt && ++xt < (BOARD_ROWS-1) && ++yt <(BOARD_ROWS-1))
    {
      moves.emplace_back(fp.pos, yt*BOARD_ROWS+xt);
      if (xt+1 < (BOARD_ROWS-1))
      {
        if (b.get(yt*BOARD_ROWS+xt+1) || b.get(max(yt-1,1)*BOARD_ROWS+xt+1) || b.get(min(yt+1,BOARD_ROWS-1)*BOARD_ROWS+xt+1))
        {
          break;
        }
        if (bo.get(yt*BOARD_ROWS+xt+1) || bo.get(max(yt-1,1)*BOARD_ROWS+xt+1) || bo.get(min(yt+1,BOARD_ROWS-1)*BOARD_ROWS+xt+1))
        {
          break;
        }
      }
      if (yt + 1 < (BOARD_ROWS-1))
      {
        if (b.get((yt+1)*BOARD_ROWS+xt) || b.get((yt+1)*BOARD_ROWS+max(1,xt-1)) || b.get((yt+1)*BOARD_ROWS+min(xt+1,BOARD_ROWS-1)))
        {
          break;
        }
        if (bo.get((yt+1)*BOARD_ROWS+xt) || bo.get((yt+1)*BOARD_ROWS+max(1,xt-1)) || bo.get((yt+1)*BOARD_ROWS+min(xt+1,BOARD_ROWS-1)))
        {
          break;
        }
      }

    }
  }

  if (fp.ul)
  {
    int cnt = fp.cnt;
    int yt = y;
    int xt = x;

    while (--cnt && --xt >0 && ++yt <(BOARD_ROWS-1))
    {
      moves.emplace_back(fp.pos, yt*BOARD_ROWS+xt);
      if (xt -1 >0)
      {
        if (b.get(yt*BOARD_ROWS+xt-1) || b.get(max(yt-1,1)*BOARD_ROWS+xt-1) || b.get(min(yt+1,BOARD_ROWS-1)*BOARD_ROWS+xt-1))
        {
          break;
        }
        if (bo.get(yt*BOARD_ROWS+xt-1) || bo.get(max(yt-1,1)*BOARD_ROWS+xt-1) || bo.get(min(yt+1,BOARD_ROWS-1)*BOARD_ROWS+xt-1))
        {
          break;
        }
      }
      if (yt + 1 < (BOARD_ROWS-1))
      {
        if (b.get((yt+1)*BOARD_ROWS+xt) || b.get((yt+1)*BOARD_ROWS+max(1,xt-1)) || b.get((yt+1)*BOARD_ROWS+min(xt+1,BOARD_ROWS-1)))
        {
          break;
        }
        if (bo.get((yt+1)*BOARD_ROWS+xt) || bo.get((yt+1)*BOARD_ROWS+max(1,xt-1)) || bo.get((yt+1)*BOARD_ROWS+min(xt+1,BOARD_ROWS-1)))
        {
          break;
        }
      }
    }
  }

  if (fp.dr)
  {
    int cnt = fp.cnt;
    int yt = y;
    int xt = x;
    while (--cnt && ++xt < (BOARD_ROWS-1) && --yt >0)
    {
      moves.emplace_back(fp.pos, yt*BOARD_ROWS+xt);
      if (xt+1 < (BOARD_ROWS-1))
      {
        if (b.get(yt*BOARD_ROWS+xt+1) || b.get(max(yt-1,1)*BOARD_ROWS+xt+1) || b.get(min(yt+1,BOARD_ROWS-1)*BOARD_ROWS+xt+1))
        {
          break;
        }
        if (bo.get(yt*BOARD_ROWS+xt+1) || bo.get(max(yt-1,1)*BOARD_ROWS+xt+1) || bo.get(min(yt+1,BOARD_ROWS-1)*BOARD_ROWS+xt+1))
        {
          break;
        }
      }
      if (yt -1 >0)
      {
        if (b.get((yt-1)*BOARD_ROWS+xt) || b.get((yt-1)*BOARD_ROWS+max(1,xt-1)) || b.get((yt-1)*BOARD_ROWS+min(xt+1,BOARD_ROWS-1)))
        {
          break;
        }
        if (bo.get((yt-1)*BOARD_ROWS+xt) || bo.get((yt-1)*BOARD_ROWS+max(1,xt-1)) || bo.get((yt-1)*BOARD_ROWS+min(xt+1,BOARD_ROWS-1)))
        {
          break;
        }
      }
    }
  }

  if (fp.dl)
  {
    int cnt = fp.cnt;
    int yt = y;
    int xt = x;
    while (--cnt && --xt >0  && --yt >0)
    {
      moves.emplace_back(fp.pos, yt*BOARD_ROWS+xt);
      if (xt -1 >0)
      {
        if (b.get(yt*BOARD_ROWS+xt-1) || b.get(max(yt-1,1)*BOARD_ROWS+xt-1) || b.get(min(yt+1,BOARD_ROWS-1)*BOARD_ROWS+xt-1))
        {
          break;
        }
        if (bo.get(yt*BOARD_ROWS+xt-1) || bo.get(max(yt-1,1)*BOARD_ROWS+xt-1) || bo.get(min(yt+1,BOARD_ROWS-1)*BOARD_ROWS+xt-1))
        {
          break;
        }
      }

      if (yt -1 >0)
      {
        if (b.get((yt-1)*BOARD_ROWS+xt) || b.get((yt-1)*BOARD_ROWS+max(1,xt-1)) || b.get((yt-1)*BOARD_ROWS+min(xt+1,BOARD_ROWS-1)))
        {
          break;
        }
        if (bo.get((yt-1)*BOARD_ROWS+xt) || bo.get((yt-1)*BOARD_ROWS+max(1,xt-1)) || bo.get((yt-1)*BOARD_ROWS+min(xt+1,BOARD_ROWS-1)))
        {
          break;
        }
      }
    }
  }
}

void game_state::create_moves(std::vector<move>& moves)
{
  for(int i=0;i<visited_cnt;i++)
  {
    visited[visited_tab[i]] = false;
  }

  visited_cnt = 0;

  for (int i=0;i<pieces[current_player-1].s;i++)
  {
    uint64_t all = pieces[current_player-1].date[i];

    while(all)
    {
      int mv = msb(all);

      int pos= (i << 6) + mv;

      int x = pos%BOARD_ROWS;
      int y = pos/BOARD_ROWS;

      start(pos,moves);
      start(y*BOARD_ROWS+x+1,moves);
      start(y*BOARD_ROWS+x-1,moves);
      start((y+1)*BOARD_ROWS+x,moves);
      start((y-1)*BOARD_ROWS+x,moves);
      start((y+1)*BOARD_ROWS+x-1,moves);
      start((y+1)*BOARD_ROWS+x+1,moves);
      start((y-1)*BOARD_ROWS+x+1,moves);
      start((y-1)*BOARD_ROWS+x-1,moves);


      all ^= (uint64_t)1 << mv;
    }
  }
}

inline void game_state::start(int pos, std::vector<move> &moves)
{
  if (visited[pos])
    return;
  visited[pos] = true;
  visited_tab[visited_cnt++]=pos;
  footprint fp;
  create_footprint(pos,fp);

  if ((fp.u || fp.d || fp.ul || fp.ur || fp.dl || fp.dr || fp.l || fp.r) && !check_footprint(pos))
  {
    add_moves(fp, moves);
  }
}

bool game_state::check_footprint(uint32_t pos)
{
  board &b = pieces[(current_player^0b11)-1];

  if (b.get(pos))
  {
    return true;
  }

  int x = pos%BOARD_ROWS;
  int y = pos/BOARD_ROWS;

  if (x+1<(BOARD_ROWS-1) && b.get(y*BOARD_ROWS+x+1))
  {
    return true;
  }

  if (x-1>0 && b.get(y*BOARD_ROWS+x-1))
  {
    return true;
  }

  if (y+1<(BOARD_ROWS-1) && b.get((y+1)*BOARD_ROWS+x))
  {
    return true;
  }

  if (y-1>0 && b.get((y-1)*BOARD_ROWS+x))
  {
    return true;
  }

  if (y+1<(BOARD_ROWS-1) && x+1<(BOARD_ROWS-1) && b.get((y+1)*BOARD_ROWS+x+1))
  {
    return true;
  }

  if (y-1>0 && x-1>0 && b.get((y-1)*BOARD_ROWS+x-1))
  {
    return true;

  }

  if (y+1<(BOARD_ROWS-1) && x-1>0 && b.get((y+1)*BOARD_ROWS+x-1))
  {
    return true;
  }

  if (y-1>0 && x+1<(BOARD_ROWS-1) && b.get((y-1)*BOARD_ROWS+x+1))
  {
    return true;
  }

  return false;
}

void game_state::create_footprint(uint32_t pos, footprint &fp)
{
  board &b = pieces[current_player-1];

  fp.pos = pos;

  if (b.get(pos))
  {
    fp.cnt=20;
  }

  int x = pos%BOARD_ROWS;
  int y = pos/BOARD_ROWS;

  if (x+1<(BOARD_ROWS-1) && b.get(y*BOARD_ROWS+x+1))
  {
    fp.r = true;
  }

  if (x-1>0 && b.get(y*BOARD_ROWS+x-1))
  {
    fp.l = true;
  }

  if (y+1<(BOARD_ROWS-1) && b.get((y+1)*BOARD_ROWS+x))
  {
    fp.u = true;
  }

  if (y-1>0 && b.get((y-1)*BOARD_ROWS+x))
  {
    fp.d = true;
  }

  if (y+1<(BOARD_ROWS-1) && x+1<(BOARD_ROWS-1) && b.get((y+1)*BOARD_ROWS+x+1))
  {
    fp.ur = true;
  }

  if (y-1>0 && x-1>0 && b.get((y-1)*BOARD_ROWS+x-1))
  {
    fp.dl = true;
  }

  if (y+1<(BOARD_ROWS-1) && x-1>0 && b.get((y+1)*BOARD_ROWS+x-1))
  {
    fp.ul = true;
  }

  if (y-1>0 && x+1<(BOARD_ROWS-1) && b.get((y-1)*BOARD_ROWS+x+1))
  {
    fp.dr = true;
  }
}
}