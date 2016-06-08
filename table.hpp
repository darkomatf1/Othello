#ifndef TABLE_HPP
#define TABLE_HPP

#include <QWidget>
#include <QtGui>
//#include <unistd.h>

class table : public QWidget {
public:
  //constructor for the table
  table(QWidget *parent);
  //and the assisting function that resets/initializes some class members
  //note that reset() will be used when ANY new game starts (button is pressed)
  void reset();
  //flank_north
  bool flank_n(char c, short i, short j) const;
  //flank_northeast
  bool flank_ne(char c, short i, short j) const;
  //flank_east
  bool flank_e(char c, short i, short j) const;
  //flank_southeast
  bool flank_se(char c, short i, short j) const;
  //flank_south
  bool flank_s(char c, short i, short j) const;
  //flank_southwest
  bool flank_sw(char c, short i, short j) const;
  //flank_west
  bool flank_w(char c, short i, short j) const;
  //flank_northwest
  bool flank_nw(char c, short i, short j) const;
  //check if the 'c'-colored player can make any move
  bool playable(char c) const;
  //attempt to place a 'c'-colored disc into (i,j) field
  bool place(char c, short i, short j);
  //return number of flanked discs if we play on (i,j) field
  // 0 returned = can't play on that field
  short check_place(char c, short i, short j);
  //computer player make move
  bool computer_play();
  //count the amount of 'c'-colored discs
  short count(char c) const;
  //get current player
  short get_player() const;
  //switch the current player
  void switch_player();
  //get current game state
  short get_state() const;
  //set the game state
  void set_state(const short state);
  //get move_made indicator
  bool move_made() const;
  //set move_made indicator
  void set_move_made(const bool move);
  //get computer indicator
  bool get_ind_comp() const;
  //set computer indicator
  void set_ind_comp(const bool ind);
  //translate a clicked point into (x,y) coordinates used by the class
  std::pair<short,short> translate(QPoint p) const;
protected:
  //interaction and paint methods for the board
  void paintEvent(QPaintEvent *);
  void mousePressEvent(QMouseEvent * event);
private:
  //we note the current game state on the table using:
  //'W' = white disc is placed here
  //'B' = black disc is placed here
  //'-' = nothing is placed here
  char _fields[8][8];
  //we need to keep track of who is currently playing
  //either 'W' (White) or 'B' (Black)
  char _player;
  //and keep track of the current game state
  //0 = ready to start, 1 = in progress, 2 = over
  short _state;
  //and last, but not least, know if a move was previously made
  bool _move_made;
  //indicator which show if computer is playing
  // if _ind_comp=true, it is choosen to play against computer
  bool _ind_comp;
};

#endif
