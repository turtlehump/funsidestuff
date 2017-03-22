#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <sstream>
using namespace std;

class Card
{
    public:
        Card(int value)    {m_value = value; m_revealed = false;}

        int    get_value()    {return m_value;}
        bool   is_revealed()  {return m_revealed;}

        void   flip()         {m_revealed = true;}

        string get_display_value();
        void   print();

    private:
        int  m_value;
        bool m_revealed;
};
#endif
