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

inline bool game_state::check(uint32_t pos, uint32_t player)
{
  board &b = pieces[player];

  uint32_t x = pos%BOARD_ROWS;
  uint32_t y = pos/BOARD_ROWS;

  if (b.get(pos))
  {
    return false;
  }

  if (b.get(y*BOARD_ROWS+x+1) &&
      b.get(y*BOARD_ROWS+x-1) &&
      b.get((y+1)*BOARD_ROWS+x) &&
      b.get((y-1)*BOARD_ROWS+x) &&
      b.get((y+1)*BOARD_ROWS+x+1) &&
      b.get((y-1)*BOARD_ROWS+x-1) &&
      b.get((y+1)*BOARD_ROWS+x-1) &&
      b.get((y-1)*BOARD_ROWS+x+1))
  {
    return true;
  }

  return false;
}

bool game_state::win_condition(uint32_t player)
{
  while (rings[player].size())
  {
    if (check(rings[player].back(),player))
    {
      return false;
    }

    rings[player].pop_back();
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

  uint32_t x = pos%BOARD_ROWS;
  uint32_t y = pos/BOARD_ROWS;

  if (b.get(y*BOARD_ROWS+x+1))
  {
    b.reset(y*BOARD_ROWS+x+1);
  }

  if (b.get(y*BOARD_ROWS+x-1))
  {
    b.reset(y*BOARD_ROWS+x-1);
  }

  if (b.get((y+1)*BOARD_ROWS+x))
  {
    b.reset((y+1)*BOARD_ROWS+x);
  }

  if (b.get((y-1)*BOARD_ROWS+x))
  {
    b.reset((y-1)*BOARD_ROWS+x);
  }

  if (b.get((y+1)*BOARD_ROWS+x+1))
  {
    b.reset((y+1)*BOARD_ROWS+x+1);
  }

  if (b.get((y-1)*BOARD_ROWS+x-1))
  {
    b.reset((y-1)*BOARD_ROWS+x-1);
  }

  if (b.get((y+1)*BOARD_ROWS+x-1))
  {
    b.reset((y+1)*BOARD_ROWS+x-1);
  }

  if (b.get((y-1)*BOARD_ROWS+x+1))
  {
    b.reset((y-1)*BOARD_ROWS+x+1);
  }
}

void game_state::set_pos(uint32_t start, uint32_t end, board &bcp)
{
  board &b = pieces[current_player-1];

  if (bcp.get(start))
  {
    b.set(end);
  }

  uint32_t x = end%BOARD_ROWS;
  uint32_t y = end/BOARD_ROWS;

  uint32_t xs = start%BOARD_ROWS;
  uint32_t ys = start/BOARD_ROWS;

  if (x+1<(BOARD_ROWS-2) && bcp.get(ys*BOARD_ROWS+xs+1))
  {
    b.set(y*BOARD_ROWS+x+1);
  }

  if (x-1>1 && bcp.get(ys*BOARD_ROWS+xs-1))
  {
    b.set(y*BOARD_ROWS+x-1);
  }

  if (y+1<(BOARD_ROWS-2) && bcp.get((ys+1)*BOARD_ROWS+xs))
  {
    b.set((y+1)*BOARD_ROWS+x);
  }

  if (y-1>1 && bcp.get((ys-1)*BOARD_ROWS+xs))
  {
    b.set((y-1)*BOARD_ROWS+x);
  }

  if ( y+1<(BOARD_ROWS-2) && x+1<(BOARD_ROWS-2) && bcp.get((ys+1)*BOARD_ROWS+xs+1))
  {
    b.set((y+1)*BOARD_ROWS+x+1);
  }

  if (y-1>1 && x-1>1  && bcp.get((ys-1)*BOARD_ROWS+xs-1))
  {
    b.set((y-1)*BOARD_ROWS+x-1);
  }

  if (y+1<(BOARD_ROWS-2) && x-1>1 && bcp.get((ys+1)*BOARD_ROWS+xs-1))
  {
    b.set((y+1)*BOARD_ROWS+x-1);
  }

  if (y-1>1 && x+1<(BOARD_ROWS-2) &&  bcp.get((ys-1)*BOARD_ROWS+xs+1))
  {
    b.set((y-1)*BOARD_ROWS+x+1);
  }
}

void game_state::update_ring(uint32_t pos)
{
  uint32_t x = pos%BOARD_ROWS;
  uint32_t y = pos/BOARD_ROWS;

  for (int i=-2;i<=2;i++)
  {
    for (int j=-2;j<=2;j++)
    {
      if (check((y+i)*BOARD_ROWS+x+j, current_player-1))
      {
        rings[current_player-1].push_back((y+i)*BOARD_ROWS+x+j);
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
  update_ring(m.end);

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

  uint32_t x = fp.pos%BOARD_ROWS;
  uint32_t y = fp.pos/BOARD_ROWS;

  if (fp.u)
  {
    uint32_t cnt = fp.cnt;
    uint32_t yt = y;
    uint32_t xt = x;

    while (cnt-- && ++yt < (BOARD_ROWS-2))
    {
      moves.emplace_back(fp.pos, yt*BOARD_ROWS+xt);

      if (b.get((yt+1)*BOARD_ROWS+xt) || b.get((yt+1)*BOARD_ROWS+xt-1) || b.get((yt+1)*BOARD_ROWS+xt+1) ||
        bo.get((yt+1)*BOARD_ROWS+xt) || bo.get((yt+1)*BOARD_ROWS+xt-1) || bo.get((yt+1)*BOARD_ROWS+xt+1))
      {
        break;
      }
    }
  }

  if (fp.d)
  {
    uint32_t cnt = fp.cnt;
    uint32_t yt = y;
    uint32_t xt = x;

    while (cnt-- && --yt >1)
    {
      moves.emplace_back(fp.pos, yt*BOARD_ROWS+xt);

      if (b.get((yt-1)*BOARD_ROWS+xt) || b.get((yt-1)*BOARD_ROWS+xt-1) || b.get((yt-1)*BOARD_ROWS+xt+1) ||
        bo.get((yt-1)*BOARD_ROWS+xt) || bo.get((yt-1)*BOARD_ROWS+xt-1) || bo.get((yt-1)*BOARD_ROWS+xt+1))
      {
        break;
      }
    }
  }

  if (fp.l)
  {
    uint32_t cnt = fp.cnt;
    uint32_t yt = y;
    uint32_t xt = x;

    while (cnt-- && --xt >1)
    {
      moves.emplace_back(fp.pos, yt*BOARD_ROWS+xt);

      if (b.get(yt*BOARD_ROWS+xt-1) || b.get((yt-1)*BOARD_ROWS+xt-1) || b.get((yt+1)*BOARD_ROWS+xt-1) ||
        bo.get(yt*BOARD_ROWS+xt-1) || bo.get((yt-1)*BOARD_ROWS+xt-1) || bo.get((yt+1)*BOARD_ROWS+xt-1))
      {
        break;
      }
    }
  }

  if (fp.r)
  {
    uint32_t cnt = fp.cnt;
    uint32_t yt = y;
    uint32_t xt = x;

    while (cnt-- && ++xt < (BOARD_ROWS-2))
    {
      moves.emplace_back(fp.pos, yt*BOARD_ROWS+xt);

      if (b.get(yt*BOARD_ROWS+xt+1) || b.get((yt-1)*BOARD_ROWS+xt+1) || b.get((yt+1)*BOARD_ROWS+xt+1) ||
        bo.get(yt*BOARD_ROWS+xt+1) || bo.get((yt-1)*BOARD_ROWS+xt+1) || bo.get((yt+1)*BOARD_ROWS+xt+1))
      {
        break;
      }
    }
  }

  if (fp.ur)
  {
    uint32_t cnt = fp.cnt;
    uint32_t yt = y;
    uint32_t xt = x;

    while (cnt-- && ++xt < (BOARD_ROWS-2) && ++yt <(BOARD_ROWS-2))
    {
      moves.emplace_back(fp.pos, yt*BOARD_ROWS+xt);

      if (b.get(yt*BOARD_ROWS+xt+1) || b.get((yt-1)*BOARD_ROWS+xt+1) || b.get((yt+1)*BOARD_ROWS+xt+1) ||
        bo.get(yt*BOARD_ROWS+xt+1) || bo.get((yt-1)*BOARD_ROWS+xt+1) || bo.get((yt+1)*BOARD_ROWS+xt+1) ||
        b.get((yt+1)*BOARD_ROWS+xt) || b.get((yt+1)*BOARD_ROWS+xt-1) || b.get((yt+1)*BOARD_ROWS+xt+1) ||
        bo.get((yt+1)*BOARD_ROWS+xt) || bo.get((yt+1)*BOARD_ROWS+xt-1) || bo.get((yt+1)*BOARD_ROWS+xt+1))
      {
        break;
      }
    }
  }

  if (fp.ul)
  {
    uint32_t cnt = fp.cnt;
    uint32_t yt = y;
    uint32_t xt = x;

    while (cnt-- && --xt >1 && ++yt <(BOARD_ROWS-2))
    {
      moves.emplace_back(fp.pos, yt*BOARD_ROWS+xt);

      if (b.get(yt*BOARD_ROWS+xt-1) || b.get((yt-1)*BOARD_ROWS+xt-1) || b.get((yt+1)*BOARD_ROWS+xt-1) ||
        bo.get(yt*BOARD_ROWS+xt-1) || bo.get((yt-1)*BOARD_ROWS+xt-1) || bo.get((yt+1)*BOARD_ROWS+xt-1) ||
        b.get((yt+1)*BOARD_ROWS+xt) || b.get((yt+1)*BOARD_ROWS+xt-1) || b.get((yt+1)*BOARD_ROWS+xt+1) ||
        bo.get((yt+1)*BOARD_ROWS+xt) || bo.get((yt+1)*BOARD_ROWS+xt-1) || bo.get((yt+1)*BOARD_ROWS+xt+1))
      {
        break;
      }
    }
  }

  if (fp.dr)
  {
    uint32_t cnt = fp.cnt;
    uint32_t yt = y;
    uint32_t xt = x;

    while (cnt-- && ++xt < (BOARD_ROWS-2) && --yt >1)
    {
      moves.emplace_back(fp.pos, yt*BOARD_ROWS+xt);

      if (b.get(yt*BOARD_ROWS+xt+1) || b.get((yt-1)*BOARD_ROWS+xt+1) || b.get((yt+1)*BOARD_ROWS+xt+1) ||
        bo.get(yt*BOARD_ROWS+xt+1) || bo.get((yt-1)*BOARD_ROWS+xt+1) || bo.get((yt+1)*BOARD_ROWS+xt+1) ||
        b.get((yt-1)*BOARD_ROWS+xt) || b.get((yt-1)*BOARD_ROWS+xt-1) || b.get((yt-1)*BOARD_ROWS+xt+1) ||
        bo.get((yt-1)*BOARD_ROWS+xt) || bo.get((yt-1)*BOARD_ROWS+xt-1) || bo.get((yt-1)*BOARD_ROWS+xt+1))
      {
        break;
      }

    }
  }

  if (fp.dl)
  {
    uint32_t cnt = fp.cnt;
    uint32_t yt = y;
    uint32_t xt = x;

    while (cnt-- && --xt >1  && --yt >1)
    {
      moves.emplace_back(fp.pos, yt*BOARD_ROWS+xt);

      if (b.get(yt*BOARD_ROWS+xt-1) || b.get((yt-1)*BOARD_ROWS+xt-1) || b.get((yt+1)*BOARD_ROWS+xt-1) ||
        bo.get(yt*BOARD_ROWS+xt-1) || bo.get((yt-1)*BOARD_ROWS+xt-1) || bo.get((yt+1)*BOARD_ROWS+xt-1) ||
        b.get((yt-1)*BOARD_ROWS+xt) || b.get((yt-1)*BOARD_ROWS+xt-1) || b.get((yt-1)*BOARD_ROWS+xt+1) ||
        bo.get((yt-1)*BOARD_ROWS+xt) || bo.get((yt-1)*BOARD_ROWS+xt-1) || bo.get((yt-1)*BOARD_ROWS+xt+1))
      {
        break;
      }
    }
  }
}

void game_state::create_moves(std::vector<move>& moves)
{
  for(uint32_t i=0;i<visited_cnt;i++)
  {
    visited[visited_tab[i]] = false;
  }

  visited_cnt = 0;

  for (size_t i=0;i<pieces[current_player-1].s;i++)
  {
    uint64_t all = pieces[current_player-1].date[i];

    while(all)
    {
      uint32_t mv = msb(all);

      uint32_t pos= (i << 6) + mv;

      uint32_t x = pos%BOARD_ROWS;
      uint32_t y = pos/BOARD_ROWS;

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

inline void game_state::start(uint32_t pos, std::vector<move> &moves)
{
  if (visited[pos])
  {
    return;
  }

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

  uint32_t x = pos%BOARD_ROWS;
  uint32_t y = pos/BOARD_ROWS;

  return b.get(y*BOARD_ROWS+x+1) || b.get(y*BOARD_ROWS+x-1) || b.get((y+1)*BOARD_ROWS+x) || b.get((y-1)*BOARD_ROWS+x) ||
    b.get((y+1)*BOARD_ROWS+x+1) || b.get((y+1)*BOARD_ROWS+x-1) || b.get((y-1)*BOARD_ROWS+x+1) || b.get((y-1)*BOARD_ROWS+x-1);
}

void game_state::create_footprint(uint32_t pos, footprint &fp)
{
  board &b = pieces[current_player-1];

  fp.pos = pos;

  if (b.get(pos))
  {
    fp.cnt=BOARD_ROWS;
  }

  uint32_t x = pos%BOARD_ROWS;
  uint32_t y = pos/BOARD_ROWS;

  fp.r = b.get(y*BOARD_ROWS+x+1);
  fp.l = b.get(y*BOARD_ROWS+x-1);
  fp.u = b.get((y+1)*BOARD_ROWS+x);
  fp.d = b.get((y-1)*BOARD_ROWS+x);
  fp.ur = b.get((y+1)*BOARD_ROWS+x+1);
  fp.dl = b.get((y-1)*BOARD_ROWS+x-1);
  fp.ul = b.get((y+1)*BOARD_ROWS+x-1);
  fp.dr = b.get((y-1)*BOARD_ROWS+x+1);
}
}