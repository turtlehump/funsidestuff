#include "player.h"

Player::Player(string name)
{
  m_name = name;
  m_cur_playing = false;
  m_next_hand = 0;
  m_money = 300;
  return;
}

Player::Player(string name, int money)
{
  m_name = name;
  m_cur_playing = false;
  m_next_hand = 0;
  m_money = money;
  return;
}

Player::~Player()
{
  for(unsigned int i = 0; i < m_hands.size(); i++)
    delete m_hands[i];
  return;
}

void Player::reset_hands()
{
  m_next_hand = 0;
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
    (m_hands[i]->is_playing())? cout << "*": cout << " ";
    cout << i + 1 << " (" << m_hands[i]->get_bet() << "):";
    m_hands[i]->print();
  }
  cout << endl;
  return;
}

void Player::add_hand(Hand* new_hand)
{
  this->take_money(new_hand->get_bet());
  m_hands.push_back(new_hand);
  return;
}

Hand* Player::get_next_hand()
{
  if(m_next_hand == m_hands.size())
  {
    if(m_hands[m_next_hand - 1]->is_completed())
      return NULL;
    else
      m_next_hand = 0;
  }
  Hand* tmp = m_hands[m_next_hand];
  m_next_hand++;
  return tmp;
}

int Player::determine_payout(int dealers_hand_value)
{
  int total_payout = 0;
  for(unsigned int i = 0; i < m_hands.size(); i++)
  {
    if(m_hands[i]->has_bust()) cout << m_name << ": Hand " << i + 1 << ": BUST";
    else
    {
      int hand_value = m_hands[i]->value();

      if(hand_value > dealers_hand_value) cout << m_name << ": Hand " << i + 1 << ": WON";
      if(hand_value == dealers_hand_value) cout << m_name << ": Hand " << i + 1 << ": PUSHED";
      if(hand_value < dealers_hand_value) cout << m_name << ": Hand " << i + 1 << ": LOST";
    }

    cout << endl;
    total_payout += m_hands[i]->determine_payout(dealers_hand_value);
  }
  return total_payout;
}
