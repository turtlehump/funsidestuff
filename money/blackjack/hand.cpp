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
      m_cur_playing = false;
      m_completed = true;
    }
    return hard_value;
  }
  else return soft_value;
} 

int Hand::stand()
{
  m_cur_playing = false;
  m_completed = true;
  return this->value();
}

int Hand::double_down(Card* new_card)
{
  m_bet *= 2;
  m_cards.push_back(new_card);
  return this->stand();
}

//you can only split as your first action
//you are guaranteed to have 2 cards
bool Hand::can_split()
{
  if(m_cards.size() == 2)
  {
    if(m_cards[0]->print_blackjack() == m_cards[1]->print_blackjack())
    return true;
  }
  //else
    return false;
}

//you can only split as your first action
//you are guaranteed to have 2 cards
//the 2 cards are guaranteed to have the same value
//take a card from current hand and make a new hand with it.
Hand* Hand::split()
{
  Hand* new_hand = new Hand(m_bet);

  Card* tmp = m_cards[1];
  m_cards.erase(m_cards.begin() + 1);

  new_hand->hit(tmp);

  return new_hand;
}

void Hand::print()
{
  for(unsigned int i = 0; i < m_cards.size(); i++)
  {
    cout << " " << m_cards[i]->print();
    if(i < (m_cards.size() - 1)) //if not the last card
      cout << ",";
  }
  (m_bust)? cout << "\t[BUST]": cout << "\t[" << this->value() << "]";

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
      cout << " " << m_cards[0]->print();}
    if(m_cards.size() == 2){
      cout << " " << m_cards[0]->print() << ", -";}
  }
  return;
}

int Hand::value()
{
  int soft_value = this->soft_value();

  if(soft_value > 21)
    return this->hard_value();
  else
    return soft_value;
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
  if(hand_value > 21) m_bust = true;
  return hand_value;
}

//This should be called after the hand has been played and we have a real value
int Hand::determine_payout(int dealers_hand_value)
{
  if(m_bust) return 0; //you would have busted before the dealer
  if(dealers_hand_value > 21) return 2 * m_bet;

  int value = this->value();
  if(value > dealers_hand_value)
    return 2 * m_bet;
  else if(value == dealers_hand_value)
    return m_bet;
  else //if(value < dealer_hand_value)
    return 0;
}
