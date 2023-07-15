#include "player.h"

Player::Player(string name)
{
  m_name = name;
  m_money = 300;

  return;
}

Player::Player(string name, int money)
{
  m_name = name;
  m_money = money;

  return;
}
