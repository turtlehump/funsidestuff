#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <sstream>
using namespace std;

enum CardFace {ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, WILD, XTWO, XFIVE, XTEN};

class Card
{
  public:
    Card(CardFace face);

    CardFace get_face()                {return m_face;}

    void     reveal()                  {m_revealed = true;}
    bool     is_revealed()             {return m_revealed;}

    int      money_value()             {return m_money_value;}

    string   display_value();
    void     print();

  private:
    CardFace    m_face;
    int         m_money_value;
    bool        m_special;     //Does this card have any special effects to the payout?
    bool        m_revealed;
};
#endif
