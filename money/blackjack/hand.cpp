#include "hand.h"

Hand::~Hand()
{
  for(unsigned int i = 0; i < m_cards.size(); i++)
    delete m_cards[i];
}

int Hand::hit(Card* new_card)
{
  m_cards.push_back(new_card);

  int soft_value = this->soft_value(); //so we dont have to calcualte it again
  int hard_value = this->hard_value(); //so we dont have to calcualte it again

  if(soft_value > 21)
  {
    if(hard_value > 21)
    {
      m_bust = true;
      return 0;
    }
    else return hard_value;
  }
  else return soft_value;
} 

int Hand::stand()
{
  m_cur_playing = false;
  m_completed = true;
  int soft_value = this->soft_value();

  if(soft_value > 21)
    return this->hard_value();

  else return soft_value;
}

//you can only split as your first action
//you are guaranteed to have 2 cards
//the 2 cards are guaranteed to have the same value
//take a card from current hand and return it so you cam make a new hand with it.
Card* Hand::split()
{
  Card* tmp = m_cards[1];
  m_cards.erase(m_cards.begin() + 1);
  return tmp;
}

void Hand::print()
{
  for(unsigned int i = 0; i < m_cards.size(); i++)
  {
    cout << " "; m_cards[i]->print();
    if(i < (m_cards.size() - 1)) //if not the last card
      cout << ",";
  }

  return;
}

void Hand::dealer_print(bool is_playing, bool final_print)
{
  if(is_playing || final_print) this->print();
  else
  {
    //if(m_cards.size() == 0)
    //  do nothing;
    if(m_cards.size() == 1){
      cout << " "; m_cards[0]->print();}
    if(m_cards.size() == 2){
      cout << " "; m_cards[0]->print(); cout << ", -";}
  }
  return;
}

int Hand::soft_value()
{
  int hand_value = 0;
  for(unsigned int i = 0; i < m_cards.size(); i++)
    hand_value += m_cards[i]->soft_blackjack_value();
  return hand_value;
}

int Hand::hard_value()
{
  int hand_value = 0;
  for(unsigned int i = 0; i < m_cards.size(); i++)
    hand_value += m_cards[i]->hard_blackjack_value();
  return hand_value;
}


int Hand::determine_payout(int dealers_hand_value)
{
  int payout;
  (m_bust)? payout = 0 : payout = m_bet;
  return payout;
}
