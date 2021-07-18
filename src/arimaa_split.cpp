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

bool game_state::checkWin(uint32_t player)
{
  if (player == 1)
  {
    if (silverRabbits == 0)
    {
      return true;
    }

    if (goldRabbits & goldEndLine)
    {
      return true;
    }
  }
  else
  {
    if (goldRabbits == 0)
    {
      return true;
    }

    if (silverRabbits & silverEndLine)
    {
      return true;
    }
  }

  return false;
}

void game_state::apply_setup(uint32_t m)
{
  if (setup_cnt == 0)
  {
    goldElephant |= (uint64_t)1 << m;
  }
  else if (setup_cnt == 1)
  {
    goldCamel |= (uint64_t)1 << m;
  }
  else if (setup_cnt == 2 || setup_cnt == 3)
  {
    goldHorses |= (uint64_t)1 << m;
  }
  else if (setup_cnt == 4 || setup_cnt == 5)
  {
    goldDogs |= (uint64_t)1 << m;
  }
  else if (setup_cnt == 4 || setup_cnt == 5)
  {
    goldCats |= (uint64_t)1 << m;
  }
  else if (setup_cnt < 16)
  {
    goldRabbits |= (uint64_t)1 << m;
  }
  else if (setup_cnt == 16)
  {
    silverElephant |= (uint64_t)1 << m;
  }
  else if (setup_cnt == 17)
  {
    silverCamel |= (uint64_t)1 << m;
  }
  else if (setup_cnt == 18 || setup_cnt == 19)
  {
    silverHorses |= (uint64_t)1 << m;
  }
  else if (setup_cnt == 20 || setup_cnt == 21)
  {
    silverDogs |= (uint64_t)1 << m;
  }
  else if (setup_cnt == 22 || setup_cnt == 23)
  {
    silverCats |= (uint64_t)1 << m;
  }
  else
  {
    silverRabbits |= (uint64_t)1 << m;
  }

  if (setup_cnt == 31)
  {
    goldPieces   |= (goldElephant | goldCamel | goldHorses | goldDogs| goldCats | goldRabbits);
    silverPieces |= (silverElephant | silverCamel | silverHorses | silverDogs| silverCats | silverRabbits);
  }
}

void game_state::update(uint64_t from, uint64_t to)
{
  uint64_t *myPieces;
  uint64_t *myElephant;
  uint64_t *myCamel;
  uint64_t *myHorses;
  uint64_t *myDogs;
  uint64_t *myCats;
  uint64_t *myRabbits;

  if (current_player == 2)
  {
    myPieces   = &silverPieces;
    myElephant = &silverElephant;
    myCamel    = &silverCamel;
    myHorses   = &silverHorses;
    myDogs     = &silverDogs;
    myCats     = &silverCats;
    myRabbits  = &silverRabbits;
  }
  else
  {
    myPieces   = &goldPieces;
    myElephant = &goldElephant;
    myCamel    = &goldCamel;
    myHorses   = &goldHorses;
    myDogs     = &goldDogs;
    myCats     = &goldCats;
    myRabbits  = &goldRabbits;
  }

  (*myPieces) ^= from;

  bool pr = protected1(to & traps, *myPieces);

  uint64_t newPos = 0;

  if (pr)
  {
    newPos = to;
  }

  (*myPieces) |= newPos;

  uint64_t pos = from;

  if ((*myElephant) & pos)
  {
    (*myElephant) ^= pos;
    (*myElephant) |= newPos;
  }
  else if ((*myCamel) & pos)
  {
    (*myCamel) ^= pos;
    (*myCamel) |= newPos;
  }
  else if ((*myHorses) & pos)
  {
    (*myHorses) ^= pos;
    (*myHorses) |= newPos;
  }
  else if ((*myDogs) & pos)
  {
    (*myDogs) ^= pos;
    (*myDogs) |= newPos;
  }
  else if ((*myCats) & pos)
  {
    (*myCats)  ^= pos;
    (*myCats)  |= newPos;
  }
  else
  {
    (*myRabbits)  ^= pos;
    (*myRabbits) |= newPos;
  }
}

board_state game_state::updateCopy(uint64_t from, uint64_t to)
{
  uint64_t myPieces;
  uint64_t myElephant;
  uint64_t myCamel;
  uint64_t myHorses;
  uint64_t myDogs;
  uint64_t myCats;
  uint64_t myRabbits;

  if (current_player == 2)
  {
    myPieces   = silverPieces;
    myElephant = silverElephant;
    myCamel    = silverCamel;
    myHorses   = silverHorses;
    myDogs     = silverDogs;
    myCats     = silverCats;
    myRabbits  = silverRabbits;
  }
  else
  {
    myPieces   = goldPieces;
    myElephant = goldElephant;
    myCamel    = goldCamel;
    myHorses   = goldHorses;
    myDogs     = goldDogs;
    myCats     = goldCats;
    myRabbits  = goldRabbits;
  }

  myPieces ^= from;

  bool pr = protected1(to & traps, myPieces);

  uint64_t newPos = 0;

  if (pr)
  {
    newPos = to;
  }

  myPieces |= newPos;

  uint64_t pos = from;

  if (myElephant & pos)
  {
    myElephant ^= pos;
    myElephant |= newPos;
  }
  else if (myCamel & pos)
  {
    myCamel ^= pos;
    myCamel |= newPos;
  }
  else if (myHorses & pos)
  {
    myHorses ^= pos;
    myHorses |= newPos;
  }
  else if (myDogs & pos)
  {
    myDogs ^= pos;
    myDogs |= newPos;
  }
  else if (myCats & pos)
  {
    myCats ^= pos;
    myCats |= newPos;
  }
  else
  {
    myRabbits ^= pos;
    myRabbits |= newPos;
  }

  return board_state(myRabbits, myHorses, myCats, myDogs, myCamel, myElephant);
}

void game_state::removeTraped(uint32_t player)
{
  uint64_t *myPieces;
  uint64_t *myElephant;
  uint64_t *myCamel;
  uint64_t *myHorses;
  uint64_t *myDogs;
  uint64_t *myCats;
  uint64_t *myRabbits;

  if (player == 2)
  {
    myPieces   = &silverPieces;
    myElephant = &silverElephant;
    myCamel    = &silverCamel;
    myHorses   = &silverHorses;
    myDogs     = &silverDogs;
    myCats     = &silverCats;
    myRabbits  = &silverRabbits;
  }
  else
  {
    myPieces   = &goldPieces;
    myElephant = &goldElephant;
    myCamel    = &goldCamel;
    myHorses   = &goldHorses;
    myDogs     = &goldDogs;
    myCats     = &goldCats;
    myRabbits  = &goldRabbits;
  }

  uint64_t pieces = traps & (*myPieces);

  while (pieces)
  {
    uint64_t mv = msb(pieces);

    bool pr = protected1(mv, *myPieces);

    if (!pr)
    {
      (*myPieces) ^= mv;

      if ((*myElephant) & mv)
      {
        (*myElephant) ^= mv;
      }
      else if ((*myCamel) & mv)
      {
        (*myCamel) ^= mv;
      }
      else if ((*myHorses) & mv)
      {
        (*myHorses) ^= mv;
      }
      else if ((*myDogs) & mv)
      {
        (*myDogs) ^= mv;
      }
      else if ((*myCats) & mv)
      {
        (*myCats) ^= mv;
      }
      else
      {
        (*myRabbits) ^= mv;
      }
    }

    pieces ^= mv;
  }
}

void game_state::apply_move(const move &m)
{
  // if (setup_cnt < 32)
  // {
  //   apply_setup(m.mr);

  //   setup_cnt++;
  //   last_pos = m.pos;

  //   if (setup_cnt == 16)
  //   {
  //     current_player ^= 0b11;
  //   }
  //   return;
  // }

  if (m.mr == PASS)
  {
    move_cnt = 0;
    current_player ^= 0b11;

    board_state gold(goldRabbits, goldHorses, goldCats, goldDogs, goldCamel, goldElephant);
    board_state silver(silverRabbits, silverHorses, silverCats, silverDogs, silverCamel, silverElephant);

    states.clear();
    states.insert(full_state(gold, silver));

    return;
  }

  move_cnt++;

  update(m.mr, m.pos);

  if (m.oponent_from != OUTSIDE_BOARD)
  {
    move_cnt++;
    current_player ^= 0b11;
    update(m.oponent_from, m.oponent_to);
    current_player ^= 0b11;
  }

  removeTraped(1);
  removeTraped(2);

  if (checkWin(current_player))
  {
    variables[current_player - 1] = 100;
    variables[(current_player ^ 0b11) - 1] = 0;
    exit = true;
    return;
  }
  else if (checkWin(current_player ^ 0b11))
  {
    variables[current_player - 1] = 0;
    variables[(current_player ^ 0b11) - 1] = 100;
    exit = true;
    return;
  }

  if (move_cnt == 4)
  {
    states.clear();
    board_state gold(goldRabbits, goldHorses, goldCats, goldDogs, goldCamel, goldElephant);
    board_state silver(silverRabbits, silverHorses, silverCats, silverDogs, silverCamel, silverElephant);
    states.insert(full_state(gold, silver));
    move_cnt = 0;
    current_player ^= 0b11;
  }
}

void game_state::setup(std::vector<move>& moves)
{
  if (current_player == 1)
  {
    if (last_pos < BOARD_SIZE)
    {
      int cnt = goldSetup.size()-1;
      goldSetup[last_pos].mr ^= goldSetup[cnt].mr;
      goldSetup[cnt].mr      ^= goldSetup[last_pos].mr;
      goldSetup[last_pos].mr ^= goldSetup[cnt].mr;
      goldSetup.pop_back();
    }

    moves.insert(moves.begin(), std::begin(goldSetup), std::end(goldSetup));
  }
  else
  {
    if (last_pos < BOARD_SIZE)
    {
      int cnt = silverSetup.size()-1;
      silverSetup[last_pos].mr ^= silverSetup[cnt].mr;
      silverSetup[cnt].mr      ^= silverSetup[last_pos].mr;
      silverSetup[last_pos].mr ^= silverSetup[cnt].mr;
      silverSetup.pop_back();
    }

    moves.insert(moves.begin(), std::begin(silverSetup), std::end(silverSetup));
  }
}

void game_state::get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves)
{
  moves.clear();

  // if (setup_cnt < 31)
  // {
  //   setup(moves);

  //   return;
  // }

  if (exit || turn_limit == TURN_LIMIT)
  {
    return;
  }

  if (move_cnt == 0)
  {
    turn_limit++;
  }

  if (current_player == 1)
  {
    getMoves(goldElephant, elephantStr, moves);
    getMoves(goldCamel,    camelStr,    moves);
    getMoves(goldHorses,   horseStr,    moves);
    getMoves(goldDogs,     dogStr,      moves);
    getMoves(goldCats,     catStr,      moves);
    getGoldRabbitMoves(moves);
  }
  else
  {
    getMoves(silverElephant, elephantStr, moves);
    getMoves(silverCamel,    camelStr,    moves);
    getMoves(silverHorses,   horseStr,    moves);
    getMoves(silverDogs,     dogStr,      moves);
    getMoves(silverCats,     catStr,      moves);
    getSilverRabbitMoves(moves);
  }

  if (move_cnt > 0 && stateDifferent())
  {
    moves.emplace_back(PASS);
  }

  if (moves.size() == 0)
  {
    if (move_cnt == 0)
    {
      variables[current_player - 1] = 0;
      variables[(current_player ^ 0b11) - 1] = 100;
      exit = true;
      return;
    }
  }
}

bool game_state::stateDifferent()
{
  board_state gold(goldRabbits, goldHorses, goldCats, goldDogs, goldCamel, goldElephant);
  board_state silver(silverRabbits, silverHorses, silverCats, silverDogs, silverCamel, silverElephant);

  return states.find(full_state(gold, silver)) == states.end();
}

void game_state::getGoldRabbitMoves(std::vector<move> &moves)
{
  uint64_t freeSpace = ~(goldPieces | silverPieces);
  uint64_t pieces    = goldRabbits;
  uint64_t oponent   = silverPieces;
  uint64_t my        = goldPieces;

  while (pieces)
  {
    uint64_t mv = msb(pieces);

    if (frozzen(mv, my, oponent, rabbitStr))
    {
      pieces ^= mv;
      continue;
    }

    uint64_t tmp  = (mv & leftBoard) >> 1;

    if (tmp & freeSpace)
    {
      insert(moves, mv, tmp);
    }


    tmp  = (mv & rightBoard) << 1;

    if (tmp & freeSpace)
    {
      insert(moves, mv, tmp);
    }

    tmp  = mv >> 8;

    if (tmp & freeSpace)
    {
      insert(moves, mv, tmp);
    }

    pieces ^= mv;
  }
}

void game_state::getSilverRabbitMoves(std::vector<move> &moves)
{
  uint64_t freeSpace = ~(goldPieces | silverPieces);
  uint64_t pieces    = silverRabbits;
  uint64_t oponent   = goldPieces;
  uint64_t my        = silverPieces;

  while (pieces)
  {
    uint64_t mv = msb(pieces);

    if (frozzen(mv, my, oponent, rabbitStr))
    {
      pieces ^= mv;
      continue;
    }

    uint64_t tmp  = (mv & leftBoard) >> 1;

    if (tmp & freeSpace)
    {
      insert(moves, mv, tmp);
    }

    tmp  = (mv & rightBoard) << 1;

    if (tmp & freeSpace)
    {
      insert(moves, mv, tmp);
    }

    tmp  = mv << 8;

    if (tmp & freeSpace)
    {
      insert(moves, mv, tmp);
    }

    pieces ^= mv;
  }
}

void game_state::addIfStronger(uint64_t pos, int str, uint64_t &res)
{
  uint64_t otherElephant;
  uint64_t otherCamel;
  uint64_t otherHorses;
  uint64_t otherDogs;
  uint64_t otherCats;

  if (current_player == 1)
  {
    otherElephant = silverElephant;
    otherCamel    = silverCamel;
    otherHorses   = silverHorses;
    otherDogs     = silverDogs;
    otherCats     = silverCats;
  }
  else
  {
    otherElephant = goldElephant;
    otherCamel    = goldCamel;
    otherHorses   = goldHorses;
    otherDogs     = goldDogs;
    otherCats     = goldCats;
  }

  if (otherElephant & pos)
  {
    return;
  }
  else if (otherCamel & pos)
  {
    if (str > camelStr)
    {
      res |= pos;
    }
  }
  else if (otherHorses & pos)
  {
    if (str > horseStr)
    {
      res |= pos;
    }
  }
  else if (otherDogs & pos)
  {
    if (str > dogStr)
    {
      res |= pos;
    }
  }
  else if (otherCats & pos)
  {
    if (str > catStr)
    {
      res |= pos;
    }
  }
  else
  {
    if (str > rabbitStr)
    {
      res |= pos;
    }
  }
}

void game_state::addIfWeaker(uint64_t pos, int str, uint64_t &res)
{
  uint64_t otherElephant;
  uint64_t otherCamel;
  uint64_t otherHorses;
  uint64_t otherDogs;
  uint64_t otherCats;

  if (current_player == 1)
  {
    otherElephant = silverElephant;
    otherCamel    = silverCamel;
    otherHorses   = silverHorses;
    otherDogs     = silverDogs;
    otherCats     = silverCats;
  }
  else
  {
    otherElephant = goldElephant;
    otherCamel    = goldCamel;
    otherHorses   = goldHorses;
    otherDogs     = goldDogs;
    otherCats     = goldCats;
  }

  if (otherElephant & pos)
  {
    if (str < elephantStr)
    {
      res |= pos;
    }
  }
  else if (otherCamel & pos)
  {
    if (str < camelStr)
    {
      res |= pos;
    }
  }
  else if (otherHorses & pos)
  {
    if (str < horseStr)
    {
      res |= pos;
    }
  }
  else if (otherDogs & pos)
  {
    if (str < dogStr)
    {
      res |= pos;
    }
  }
  else if (otherCats & pos)
  {
    if (str < catStr)
    {
      res |= pos;
    }
  }
}

bool game_state::frozzen(uint64_t pos, uint64_t my, uint64_t other, int str)
{
  uint64_t tmp = pos << 8;

  bool res = false;

  uint64_t ans = 0;

  if (tmp & other)
  {
    addIfWeaker(tmp, str, ans);

    if (ans)
    {
      res = true;
    }
  }

  tmp = pos >> 8;

  if (tmp & other)
  {
    addIfWeaker(tmp, str, ans);

    if (ans)
    {
      res = true;
    }
  }

  tmp = (pos & leftBoard) >> 1;

  if (tmp & other)
  {
    addIfWeaker(tmp, str, ans);

    if (ans)
    {
      res = true;
    }
  }

  tmp = (pos & rightBoard) << 1;

  if (tmp & other)
  {
    addIfWeaker(tmp, str, ans);

    if (ans)
    {
      res = true;
    }
  }

  tmp = pos << 8;

  if (tmp & my)
  {
    return false;
  }

  tmp = pos >> 8;

  if (tmp & my)
  {
    return false;
  }

  tmp = (pos & leftBoard) >> 1;

  if (tmp & my)
  {
    return false;
  }

  tmp = (pos & rightBoard) << 1;

  if (tmp & my)
  {
    return false;
  }

  return res;
}

bool game_state::protected1(uint64_t pos, uint64_t my)
{
  if ((pos << 8) & my)
  {
    return true;
  }
  else if ((pos >> 8) & my)
  {
    return true;
  }
  else if ((pos >> 1) & my)
  {
    return true;
  }
  else if ((pos << 1) & my)
  {
    return true;
  }
  else if (pos)
  {
    return false;
  }

  return true;
}

uint64_t game_state::getFreeNeighbour(const uint64_t &pos)
{
  return neighbour[getPos(pos)] & (~(goldPieces | silverPieces));
}

uint64_t game_state::getNeighbour(const uint64_t &pos, const uint64_t &oponent, int str)
{
  uint64_t pieces = neighbour[getPos(pos)] & oponent;

  uint64_t res = 0;

  while (pieces)
  {
    uint64_t mv = msb(pieces);

    addIfStronger(mv, str, res);

    pieces ^= mv;
  }

  return res;
}

void game_state::addMoves(uint64_t tmp, uint64_t oponentNeighbour, uint64_t freeSpace,
  uint64_t mv, std::vector<move>& moves)
{
  if (move_cnt > 2)
  {
    return;
  }

  if (tmp & oponentNeighbour) // push
  {
    uint64_t pieces = getFreeNeighbour(tmp);

    while (pieces)
    {
      uint64_t mv1 = msb(pieces);

      insert(moves, mv, tmp, tmp, mv1);

      pieces ^= mv1;
    }
  }
  else if (tmp & freeSpace) // pull
  {
    uint64_t pieces = oponentNeighbour;

    while (pieces)
    {
      uint64_t mv1 = msb(pieces);

      insert(moves, mv, tmp, mv1, mv);

      pieces ^= mv1;
    }
  }
}

void game_state::insert(std::vector<move>& moves, uint64_t from1, uint64_t to1, uint64_t from2, uint64_t to2)
{
  board_state b1 = updateCopy(from1, to1);
  board_state b2;

  int cnt = 1;

  if (from2 != OUTSIDE_BOARD)
  {
    cnt++;
    current_player ^= 0b11;
    b2 = updateCopy(from2, to2);
    current_player ^= 0b11;
  }
  else
  {
    if (current_player == 1)
    {
      b2 = board_state(silverRabbits, silverHorses, silverCats, silverDogs, silverCamel, silverElephant);
    }
    else
    {
      b2 = board_state(goldRabbits, goldHorses, goldCats, goldDogs, goldCamel, goldElephant);
    }
  }

  if (current_player == 2)
  {
    std::swap(b1, b2);
  }

  if (states.find(full_state(b1, b2)) == states.end())
  {
    moves.emplace_back(from1, to1, from2, to2);
  }
  else if (move_cnt + cnt < 4)
  {
    moves.emplace_back(from1, to1, from2, to2);
  }
}

void game_state::getMoves(uint64_t pieces, int str, std::vector<move>& moves)
{
  uint64_t freeSpace = ~(goldPieces | silverPieces);
  uint64_t oponent, my;

  if (current_player == 1)
  {
    my      = goldPieces;
    oponent = silverPieces;
  }
  else
  {
    my      = silverPieces;
    oponent = goldPieces;
  }

  while (pieces)
  {
    uint64_t mv = msb(pieces);

    if (frozzen(mv, my, oponent, str))
    {
      pieces ^= mv;
      continue;
    }

    uint64_t oponentNeighbour = getNeighbour(mv, oponent, str);

    uint64_t tmp  = (mv & leftBoard) >> 1;

    if (tmp & freeSpace)
    {
      insert(moves, mv, tmp);
    }

    addMoves(tmp, oponentNeighbour, freeSpace, mv, moves);

    tmp  = (mv & rightBoard) << 1;

    if (tmp & freeSpace)
    {
      insert(moves, mv, tmp);
    }

    addMoves(tmp, oponentNeighbour, freeSpace, mv, moves);

    tmp  = mv << 8;

    if (tmp & freeSpace)
    {
      insert(moves, mv, tmp);
    }

    addMoves(tmp, oponentNeighbour, freeSpace, mv, moves);

    tmp  = mv >> 8;

    if (tmp & freeSpace)
    {
      insert(moves, mv, tmp);
    }

    addMoves(tmp, oponentNeighbour, freeSpace, mv, moves);

    pieces ^= mv;
  }
}
}