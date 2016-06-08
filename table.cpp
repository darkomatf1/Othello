#include "table.hpp"

table::table(QWidget *parent)
    :QWidget(parent)
{
    setFixedSize(641, 641); //we set the board size
    //we set the game state to "ready to start"
    //note: pressing the button will cause _state = 0
    //for this reason, that line is not needed in reset()
    _state = 0;
    reset(); //this function will initialize/set the rest
}

void table::reset() {
    //we set the board fields to the starting position
    for(short i = 0; i < 8; i++)
      for(short j = 0; j < 8; j++)
        _fields[i][j] = '-';
    _fields[3][3] = 'W';
    _fields[3][4] = 'B';
    _fields[4][3] = 'B';
    _fields[4][4] = 'W';
    //black player goes first
    _player = 'B';
    //the initial value of this indicator is actually irrelevant
    //(moves are always possible at the start)
    _move_made = false;
    //initial value: computer player is not in the game
    //it will be changed when right_button is clicked.
    _ind_comp = false;

    //and we need to update the widget after making changes
    update();
}

//flanking to the north:
//starting from the disc immediately to the north of a 'c'-colored disc
//all discs are of the opposite color
//and the array of non-'c'-colored discs ends with a 'c'-colored disc
bool table::flank_n(char c, short i, short j) const {
  bool ind = false;
  while(_fields[--i][j] != '-' && _fields[i][j] != c && i >= 0)
    //a disc of the opposite color was found in the loop
    ind = true;
  //if the loop skips table bounds, there's no 'c'-colored disc in this direction
  if(i < 0)
    return false;
  //if we "skipped" discs of the opposite color in the loop
  //and bumped into a 'c'-colored disc
  //then it is possible to flank in this direction
  if(ind && _fields[i][j] == c)
    return true;
  return false;
}
//the remaining seven flanking methods follow the same fashion:
//check if the condition is met in the required direction
//and if the array ends in a 'c'-colored disc

//flanking to the northeast:
bool table::flank_ne(char c, short i, short j) const {
  bool ind = false;
  while(_fields[--i][++j] != '-' && _fields[i][j] != c && i >= 0 && j < 8)
    ind = true;
  if(i < 0 || j >= 8)
    return false;
  if(ind && _fields[i][j] == c)
    return true;
  return false;
}

//flanking to the east:
bool table::flank_e(char c, short i, short j) const {
  bool ind = false;
  while(_fields[i][++j] != '-' && _fields[i][j] != c && j < 8)
    ind = true;
  if(j >= 8)
    return false;
  if(ind && _fields[i][j] == c)
    return true;
  return false;
}

//flanking to the southeast:
bool table::flank_se(char c, short i, short j) const {
  bool ind = false;
  while(_fields[++i][++j] != '-' && _fields[i][j] != c && i < 8 && j < 8)
    ind = true;
  if(i >= 8 || j >= 8)
    return false;
  if(ind && _fields[i][j] == c)
    return true;
  return false;
}

//flanking to the south:
bool table::flank_s(char c, short i, short j) const {
  bool ind = false;
  while(_fields[++i][j] != '-' && _fields[i][j] != c && i < 8)
    ind = true;
  if(i >= 8)
    return false;
  if (ind && _fields[i][j] == c)
    return true;
  return false;
}

//flanking to the southwest:
bool table::flank_sw(char c, short i, short j) const {
  bool ind = false;
  while(_fields[++i][--j] != '-' && _fields[i][j] != c && j >= 0 && i < 8)
    ind = true;
  if(j < 0 || i >= 8)
    return false;
  if(ind && _fields[i][j] == c)
    return true;
  return false;
}

//flanking to the west:
bool table::flank_w(char c, short i, short j) const {
  bool ind = false;
  while(_fields[i][--j] != '-' && _fields[i][j] != c && j >= 0)
    ind = true;
  if(j < 0)
    return false;
  if(ind && _fields[i][j] == c)
    return true;
  return false;
}

//flanking to the northwest:
bool table::flank_nw(char c, short i, short j) const {
  bool ind = false;
  while(_fields[--i][--j] != '-' && _fields[i][j] != c && i >= 0 && j >= 0)
    ind = true;
  if(i < 0 || j < 0)
    return false;
  if(ind && _fields[i][j] == c)
    return true;
  return false;
}


//checks if the player using 'c'-colored discs can make a move
//a move can only be made if a newly placed disc
//flanks an enemy disc (or more) in any of 8 possible directions
//note: logical AND preceeds logical OR, hence the parantheses
bool table::playable(char c) const {
  for(short i = 0; i < 8; i++)
    for(short j = 0; j < 8; j++)
      if(_fields[i][j] == '-' && (
	 flank_n(c, i, j) || flank_ne(c, i, j) ||
	 flank_e(c, i, j) || flank_se(c, i, j) ||
	 flank_s(c, i, j) || flank_sw(c, i, j) ||
	 flank_w(c, i, j) || flank_nw(c, i, j)) )
	return true;
  return false;
}

bool table::place(char c, short i, short j) {
  //a disc can't be placed there if the position is occupied, obviously
  if(_fields[i][j] != '-')
    return false;
  //this indicator will be returned as a result of attempted move
  //true = the move was made, false = it was not made
  bool ind = false;
  
  //we now check if opposite color discs are flanked in any of the 8 directions

  //if opposite color disks are flanked to the north
  if(flank_n(c, i, j)) {
    //we set the indicator to true - the move can be made
    ind = true;
    //then we place a new disc to the desired position (i, j)
    _fields[i][j] = c;
    //and then we flip the flanked discs
    //starting from the one immediately to the north of (i, j)
    //note: the increment statement of a for loop is executed after the body of the loop
    //putting --i1 instead of i1-- changes nothing
    //also, putting i1 = --i instead of i1 = i - 1 was stupid
    //values i and j are used throughout the method, they must be preserved
    for(short i1 = i - 1; _fields[i1][j] != c; i1--)
      _fields[i1][j] = c;
  }
  //for the other 7 directions, we proceed in similar fashion
  //northeast:
  if(flank_ne(c, i, j)) {
    ind = true;
    _fields[i][j] = c;
    for(short i1 = i - 1, j1 = j + 1; _fields[i1][j1] != c; i1--, j1++)
      _fields[i1][j1] = c;
  }
  //east:
  if(flank_e(c, i, j)) {
    ind = true;
    _fields[i][j] = c;
    for(short j1 = j + 1; _fields[i][j1] != c; j1++)
      _fields[i][j1] = c;
  }
  //southeast:
  if(flank_se(c, i, j)) {
    ind = true;
    _fields[i][j] = c;
    for(short i1 = i + 1, j1 = j + 1; _fields[i1][j1] != c; i1++, j1++)
      _fields[i1][j1] = c;
  }
  //south:
  if(flank_s(c, i, j)) {
    ind = true;
    _fields[i][j] = c;
    for(short i1 = i + 1; _fields[i1][j] != c; i1++)
      _fields[i1][j] = c;
  }
  //southwest:
  if(flank_sw(c, i, j)) {
    ind = true;
    _fields[i][j] = c;
    for(short i1 = i + 1, j1 = j - 1; _fields[i1][j1] != c; i1++, j1--)
      _fields[i1][j1] = c;
  }
  //west:
  if(flank_w(c, i, j)) {
    ind = true;
    _fields[i][j] = c;
    for(short j1 = j - 1; _fields[i][j1] != c; j1--)
      _fields[i][j1] = c;
  }
  //northwest:
  if(flank_nw(c, i, j)) {
    ind = true;
    _fields[i][j] = c;
    for(short i1 = i - 1, j1 = j - 1; _fields[i1][j1] != c; i1--, j1--)
      _fields[i1][j1] = c;
  }

  //and we return the result:
  return ind;
}

short table::check_place(char c, short i, short j) {
  //a disc can't be placed there if the position is occupied, obviously
  if(_fields[i][j] != '-')
    return 0;
  //this indicator will be returned as a result of attempted move
  //true = the move was made, false = it was not made
  bool ind = false;
  //number of flanked discs
  short brojac=0;

  //we now check if opposite color discs are flanked in any of the 8 directions

  //if opposite color disks are flanked to the north
  if(flank_n(c, i, j)) {
    //we set the indicator to true - the move can be made
    ind = true;

    //and then count how many discs are going to be fliped
    for(short i1 = i - 1; _fields[i1][j] != c; i1--)
      brojac++;
  }
  //for the other 7 directions, we proceed in similar fashion
  //northeast:
  if(flank_ne(c, i, j)) {
    ind = true;

    for(short i1 = i - 1, j1 = j + 1; _fields[i1][j1] != c; i1--, j1++)
      brojac++;
  }
  //east:
  if(flank_e(c, i, j)) {
    ind = true;

    for(short j1 = j + 1; _fields[i][j1] != c; j1++)
      brojac++;
  }
  //southeast:
  if(flank_se(c, i, j)) {
    ind = true;

    for(short i1 = i + 1, j1 = j + 1; _fields[i1][j1] != c; i1++, j1++)
      brojac++;
  }
  //south:
  if(flank_s(c, i, j)) {
    ind = true;

    for(short i1 = i + 1; _fields[i1][j] != c; i1++)
      brojac++;
  }
  //southwest:
  if(flank_sw(c, i, j)) {
    ind = true;

    for(short i1 = i + 1, j1 = j - 1; _fields[i1][j1] != c; i1++, j1--)
      brojac++;
  }
  //west:
  if(flank_w(c, i, j)) {
    ind = true;

    for(short j1 = j - 1; _fields[i][j1] != c; j1--)
      brojac++;
  }
  //northwest:
  if(flank_nw(c, i, j)) {
    ind = true;

    for(short i1 = i - 1, j1 = j - 1; _fields[i1][j1] != c; i1--, j1--)
      brojac++;
  }

  //and we return the result:
  if(ind)
      return brojac;
  return 0;
}

bool table::computer_play() {
    short maxi, maxj, max=0;



    for(short i=0; i<8; i++)
        for(short j=0; j<8; j++)
        {
            short number_flanked = check_place('W', i, j);
            if(number_flanked > max)
            {
                max = number_flanked;
                maxi = i;
                maxj = j;
            }
        }
    return place('W', maxi, maxj);
}

short table::count(char c) const {
  short s = 0;
  for(short i = 0; i < 8; i++)
    for(short j = 0; j < 8; j++)
      if(_fields[i][j] == c)
	s++;
  return s;
}

short table::get_player() const {
    return _player;
}

void table::switch_player() {
    if(_player == 'W')
        _player = 'B';
    else _player ='W';
}

short table::get_state() const {
    return _state;
}

void table::set_state(const short state) {
    _state = state;
}

bool table::move_made() const {
    return _move_made;
}

void table::set_move_made(const bool move) {
    _move_made = move;
}

bool table::get_ind_comp() const {
    return _ind_comp;
}

void table::set_ind_comp(const bool ind) {
    _ind_comp=ind;
}

std::pair<short,short> table::translate(QPoint p) const {
    std::pair<short,short> a = std::make_pair(p.x()/80, p.y()/80);
    return a;
}

//note: paint brush width is 1 pixel
void table::paintEvent(QPaintEvent *) {
    QPainter p(this);
    //draw the game board
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRect(0, 0, 641, 641);
    //(re)paint the pieces
    for(short i = 0; i < 8; i++)
        for(short j = 0; j < 8; j++) {
            //draw a square around each field
            p.drawRect(i * 80, j * 80, 80, 80);
            //then paint it green
            p.fillRect(i * 80 + 1, j * 80 + 1, 79, 79, Qt::darkGreen);
            if(_fields[i][j] == 'W')
                p.setBrush(Qt::white);
            else if(_fields[i][j] == 'B')
                p.setBrush(Qt::black);
            if(_fields[i][j] != '-')
                p.drawEllipse(i * 80 + 4, j * 80 + 4, 72, 72);
            p.setBrush(Qt::black); //not sure if this is needed
        }
}

void table::mousePressEvent(QMouseEvent *event) {
    if(_state != 1) //if the game is not in progress, do nothing
        return;
    //check if we are playing versus computer or not
    if(!_ind_comp || (_ind_comp && _player=='B'))
    {
        //(otherwise, proceed)
        //if the left mouse button was pressed
        if(event->button() == Qt::LeftButton) {
            //track the position
            QPoint p = event->pos();
            //convert to the coordinates used in _fields[][]
            std::pair<short,short> a = translate(p);
            short x = a.first;
            short y = a.second;
            //attempt to place a disc there and if successful
            if(place(_player, x, y)) {
                //adjust the indicator
                //note: place function has already made changes to _fields
                _move_made = true;
                switch_player();

                if(_ind_comp)
                {
                    //sleep(3);

                    if(computer_play())
                    {
                        _move_made = true;
                        switch_player();
                    }
                }
            }
            /*else
                //if the disc could not be placed at the desired position
                //check if the player can actually make any move
                if(playable(_player))
                    //if they can, do nothing
                    return;
                else
                    //if they can't, we need to check if a move was made previously
                    if(_move_made) {
                        //if so, we pass control to the other player
                        switch_player();
                        //and adjust the indicator accordingly
                        _move_made = false;
                    }
                    else
                        //otherwise, the game ends
                        if(!_move_made || count('W') + count('B') == 64)
                            _state = 2;*/

        }
    }
    //and update the widget
    update();
}
