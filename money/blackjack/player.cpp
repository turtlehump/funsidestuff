#include "player.h"

Player::~Player()
{
  for(unsigned int i = 0; i < m_hands.size(); i++)
    delete m_hands[i];
}

void Player::reset_hands()
{
  for(unsigned int i = 0; i < m_hands.size(); i++)
    delete m_hands[i];
  m_hands.clear();
  return;
}

void Player::print()
{
  (m_cur_playing)? cout << "*": cout << " ";
  cout << m_name << ":";
  for(unsigned int i = 0; i < m_hands.size(); i++)
  {
    if(i > 0)
    {
      cout << endl;
      for(unsigned int j = 0; j < m_name.size() + 2; j++)
      //starting space + name + ':'
        cout << " ";
    }
//    (m_hands[i]->is_playing())? cout << "*": cout << " ";
    cout << i + 1 << " (" << m_hands[i]->get_bet() << "):";
    m_hands[i]->print();
  }
  cout << endl;
  return;
}

Hand* Player::get_next_hand()
{
  Hand* tmp = m_hands[m_next_hand];
  m_next_hand++;
  if(m_next_hand == m_hands.size())
    m_next_hand = 0;
  return tmp;
}
