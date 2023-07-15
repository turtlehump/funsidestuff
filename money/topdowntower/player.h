#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
using namespace std;

class Player
{
  public:
    Player(string name);
    Player(string name, int money);

    string  get_name()                {return m_name;}

    int     money()                   {return m_money;}
    void    take_money(int money)     {m_money -= money;}
    void    give_money(int money)     {m_money += money;}

    bool    can_afford(int bet)       {return (m_money >= bet);}

    void    print()                   {cout << m_name << ": $" << m_money;}

  private:
    string  m_name;
    int     m_money;
};
#endif
