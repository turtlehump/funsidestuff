#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <sstream>
using namespace std;

enum CardFace
{
  ONE = 0,
  TWO = 1,
  THREE = 2,
  FOUR = 3,
  FIVE = 4,
  SIX = 5,
  SEVEN = 6,
  WILD = 7,
  XTWO = 8,
  XFIVE = 9,
  XTEN = 10,
  LAST = 11
};

class Card
{
  public:
    Card(CardFace face);

    CardFace get_face()                {return m_face;}

    void     reveal()                  {m_revealed = true;}
    bool     is_revealed()             {return m_revealed;}
    void     make_conflicted()         {m_conflicted = true;}
    bool     is_conflicted()           {return m_conflicted;}
    void     fixed_conflict()          {m_conflicted = false;}
    bool     is_special()              {return m_special;}

    int      money_value()             {return m_money_value;}

    string   display_value();
    void     print();

  private:
    CardFace m_face;
    int      m_money_value;
    bool     m_special;     //Does this card have any special effects to the payout?
    bool     m_revealed;
    bool     m_conflicted;
};
#endif
