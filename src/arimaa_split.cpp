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
  else if (setup_cnt == 6 || setup_cnt == 7)
  {
    goldCats |= (uint64_t)1 << m;
  }
  else if (setup_cnt == 8)
  {
    goldRabbits = goldPieces ^ (goldElephant | goldCamel | goldHorses | goldDogs| goldCats);
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
    silverRabbits = silverPieces ^ (silverElephant | silverCamel | silverHorses | silverDogs| silverCats);
    setup_cnt = 32;
  }
}

template <> void game_state::update<1>(uint64_t from, uint64_t to)
{
  goldPieces ^= from;

  bool pr = protected1(to & traps, goldPieces);

  uint64_t pos = from;

  if (pr)
  {
    uint64_t newPos = to;

    goldPieces |= newPos;

    if (goldElephant & pos)
    {
      goldElephant ^= pos;
      goldElephant |= newPos;
    }
    else if (goldCamel & pos)
    {
      goldCamel ^= pos;
      goldCamel |= newPos;
    }
    else if (goldHorses & pos)
    {
      goldHorses ^= pos;
      goldHorses |= newPos;
    }
    else if (goldDogs & pos)
    {
      goldDogs ^= pos;
      goldDogs |= newPos;
    }
    else if (goldCats & pos)
    {
      goldCats ^= pos;
      goldCats |= newPos;
    }
    else
    {
      goldRabbits ^= pos;
      goldRabbits |= newPos;
    }
  }
  else
  {
    if (goldElephant & pos)
    {
      goldElephant ^= pos;
    }
    else if (goldCamel & pos)
    {
      goldCamel ^= pos;
    }
    else if (goldHorses & pos)
    {
      goldHorses ^= pos;
    }
    else if (goldDogs & pos)
    {
      goldDogs ^= pos;
    }
    else if (goldCats & pos)
    {
      goldCats ^= pos;
    }
    else
    {
      goldRabbits  ^= pos;
    }
  }
}

template <> void game_state::update<2>(uint64_t from, uint64_t to)
{
  silverPieces ^= from;

  bool pr = protected1(to & traps, silverPieces);

  uint64_t pos = from;

  if (pr)
  {
    uint64_t newPos = to;

    silverPieces |= newPos;

    if (silverElephant & pos)
    {
      silverElephant ^= pos;
      silverElephant |= newPos;
    }
    else if (silverCamel & pos)
    {
      silverCamel ^= pos;
      silverCamel |= newPos;
    }
    else if (silverHorses & pos)
    {
      silverHorses ^= pos;
      silverHorses |= newPos;
    }
    else if (silverDogs & pos)
    {
      silverDogs ^= pos;
      silverDogs |= newPos;
    }
    else if (silverCats & pos)
    {
      silverCats  ^= pos;
      silverCats  |= newPos;
    }
    else
    {
      silverRabbits  ^= pos;
      silverRabbits |= newPos;
    }
  }
  else
  {
    if (silverElephant & pos)
    {
      silverElephant ^= pos;
    }
    else if (silverCamel & pos)
    {
      silverCamel ^= pos;
    }
    else if (silverHorses & pos)
    {
      silverHorses ^= pos;
    }
    else if (silverDogs & pos)
    {
      silverDogs ^= pos;
    }
    else if (silverCats & pos)
    {
      silverCats  ^= pos;
    }
    else
    {
      silverRabbits  ^= pos;
    }
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

template<> void game_state::removeTraped<2>()
{
  uint64_t pieces = traps & silverPieces;

  while (pieces)
  {
    uint64_t mv = msb(pieces);

    bool pr = protected1(mv, silverPieces);

    if (!pr)
    {
      silverPieces ^= mv;

      if (silverElephant & mv)
      {
        silverElephant ^= mv;
      }
      else if (silverCamel & mv)
      {
        silverCamel ^= mv;
      }
      else if (silverHorses & mv)
      {
        silverHorses ^= mv;
      }
      else if (silverDogs & mv)
      {
        silverDogs ^= mv;
      }
      else if (silverCats & mv)
      {
        silverCats ^= mv;
      }
      else
      {
        silverRabbits ^= mv;
      }
    }

    pieces ^= mv;
  }
}

template<> void game_state::removeTraped<1>()
{
  uint64_t pieces = traps & goldPieces;

  while (pieces)
  {
    uint64_t mv = msb(pieces);

    bool pr = protected1(mv, goldPieces);

    if (!pr)
    {
      goldPieces ^= mv;

      if (goldElephant & mv)
      {
        goldElephant ^= mv;
      }
      else if (goldCamel & mv)
      {
        goldCamel ^= mv;
      }
      else if (goldHorses & mv)
      {
        goldHorses ^= mv;
      }
      else if (goldDogs & mv)
      {
        goldDogs ^= mv;
      }
      else if (goldCats & mv)
      {
        goldCats ^= mv;
      }
      else
      {
        goldRabbits ^= mv;
      }
    }

    pieces ^= mv;
  }
}

void game_state::apply_move(const move &m)
{
  if (setup_cnt < 32)
  {
    apply_setup(m.mr);

    last_pos = m.pos;

    if (setup_cnt == 8)
    {
      setup_cnt = 16;
      current_player ^= 0b11;
    }
    else
    {
      setup_cnt++;
    }

    return;
  }

  if (m.mr == PASS)
  {
    move_cnt = 0;
    current_player ^= 0b11;

    board_state gold(goldRabbits, goldHorses, goldCats, goldDogs, goldCamel, goldElephant);
    board_state silver(silverRabbits, silverHorses, silverCats, silverDogs, silverCamel, silverElephant);
    last_full_state = full_state(gold, silver);
    
    auto res = states.find(last_full_state);

    if (res == states.end())
    {
      states[last_full_state] = states_cnt;
      repetition[states_cnt++] = 1;
    }
    else
    {
      repetition[res -> second]++;
      if (repetition[res -> second] == 3) // state repeted 3rd time current player lose
      {
        variables[current_player - 1] = 0;
        variables[(current_player ^ 0b11) - 1] = 100;
        exit = true;
        return;
      }
    }

    return;
  }

  move_cnt++;

  if (current_player == 1)
  {
    update<1>(m.mr, m.pos);
  }
  else
  {
    update<2>(m.mr, m.pos);
  }

  if (m.oponent_from != OUTSIDE_BOARD)
  {
    move_cnt++;
    current_player ^= 0b11;
    if (current_player == 1)
    {
      update<2>(m.oponent_from, m.oponent_to);
    }
    else
    {
      update<1>(m.oponent_from, m.oponent_to);
    }
    current_player ^= 0b11;
  }

  removeTraped<1>();
  removeTraped<2>();

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
    board_state gold(goldRabbits, goldHorses, goldCats, goldDogs, goldCamel, goldElephant);
    board_state silver(silverRabbits, silverHorses, silverCats, silverDogs, silverCamel, silverElephant);
    last_full_state = full_state(gold, silver);
    auto res = states.find(last_full_state);

    if (res == states.end())
    {
      states[last_full_state] = states_cnt;
      repetition[states_cnt++] = 1;
    }
    else
    {
      repetition[res -> second]++;
      if (repetition[res -> second] == 3) // state repeted 3rd time current player lost
      {
        variables[current_player - 1] = 0;
        variables[(current_player ^ 0b11) - 1] = 100;
        exit = true;
        return;
      }
    }
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

  if (setup_cnt < 31)
  {
    setup(moves);

    return;
  }

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
    variables[current_player - 1] = 0;
    variables[(current_player ^ 0b11) - 1] = 100;
    exit = true;
    return;
  }
}

bool game_state::stateDifferent()
{
  board_state gold(goldRabbits, goldHorses, goldCats, goldDogs, goldCamel, goldElephant);
  board_state silver(silverRabbits, silverHorses, silverCats, silverDogs, silverCamel, silverElephant);

  return !(last_full_state.gold == gold && last_full_state.silver == silver);
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

bool game_state::addIfWeaker(uint64_t pos, int str)
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
      return true;
    }
  }
  else if (otherCamel & pos)
  {
    if (str < camelStr)
    {
      return true;
    }
  }
  else if (otherHorses & pos)
  {
    if (str < horseStr)
    {
      return true;
    }
  }
  else if (otherDogs & pos)
  {
    if (str < dogStr)
    {
      return true;
    }
  }
  else if (otherCats & pos)
  {
    if (str < catStr)
    {
      return true;
    }
  }

  return false;
}

bool game_state::frozzen(uint64_t pos, uint64_t my, uint64_t other, int str)
{
  uint64_t pieces = neighbour[getPos(pos)];

  bool blocked   = false;

  while (pieces)
  {
    uint64_t mv = msb(pieces);

    if (!blocked && (mv & other))
    {
      blocked = addIfWeaker(mv, str);
    }

    if (mv & my)
    {
      return false;
    }

    pieces ^= mv;
  }

  return blocked;
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
  int cnt = 1;

  if (current_player == 1)
  {
    board_state b1 = updateCopy(from1, to1);
    board_state b2;

    if (from2 != OUTSIDE_BOARD)
    {
      cnt++;
      current_player ^= 0b11;
      b2 = updateCopy(from2, to2);
      current_player ^= 0b11;
    }
    else
    {
      b2 = board_state(silverRabbits, silverHorses, silverCats, silverDogs, silverCamel, silverElephant);
    }

    if (!(last_full_state.gold == b1 && last_full_state.silver == b2))
    {
      moves.emplace_back(from1, to1, from2, to2);
    }
    else if (move_cnt + cnt < 4)
    {
      moves.emplace_back(from1, to1, from2, to2);
    }
  }
  else
  {
    board_state b2 = updateCopy(from1, to1);
    board_state b1;

    if (from2 != OUTSIDE_BOARD)
    {
      cnt++;
      current_player ^= 0b11;
      b1 = updateCopy(from2, to2);
      current_player ^= 0b11;
    }
    else
    {
      b1 = board_state(goldRabbits, goldHorses, goldCats, goldDogs, goldCamel, goldElephant);
    }

    if (!(last_full_state.gold == b1 && last_full_state.silver == b2))
    {
      moves.emplace_back(from1, to1, from2, to2);
    }
    else if (move_cnt + cnt < 4)
    {
      moves.emplace_back(from1, to1, from2, to2);
    }
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