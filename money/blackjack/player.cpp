#include "player.h"

int Player::hit(Card* new_card)
{
  m_cards.push_back(new_card);

  int hand_value = 0;
  for(unsigned int i = 0; i < m_cards.size(); i++)
    hand_value += m_cards[i]->get_blackjack_value();

  return hand_value;
}

void Player::reset_hand()
{
  m_cards.clear();
  cout << m_name << " is waiting for a new hand" << endl;
  return;
}

void Player::print()
{
  cout << m_name << ":";
  for(unsigned int i = 0; i < m_cards.size(); i++)
  {
    cout << " "; m_cards[i]->print();
    if(i < (m_cards.size() - 1))
      cout << ",";
  }
  cout << endl;
  return;
}

//This should only be called on the dealer player
void Player::dealer_print(bool final_print)
{
  cout << "Dealer:";

  if(m_cards.size() == 0)
  {
    cout << endl;
    return;
  }

  if(final_print)
    for(unsigned int i = 0; i < m_cards.size(); i++)
    {
      cout << " "; m_cards[i]->print();
      if(i < (m_cards.size() - 1))
        cout << ",";
    }
  else
  {
    cout << " -";
    for(unsigned int i = 1; i < m_cards.size(); i++)
    {
      cout << ", "; m_cards[i]->print();
    }
  }
  cout << endl;
  return;
}
