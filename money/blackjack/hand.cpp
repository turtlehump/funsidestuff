#include "hand.h"

Hand::Hand(double bet)
{
  m_bet = bet;
  m_bust = false;
  m_cur_playing = false;
  m_completed = false;

  return;
}

Hand::~Hand()
{
  for(unsigned int i = 0; i < m_cards.size(); i++)
    delete m_cards[i];

  return;
}

int Hand::hit(Card* new_card)
{
  m_cards.push_back(new_card);

  return this->value();
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

  return false;
}

//you can only split as your first action
//you are guaranteed to have 2 cards
//the 2 cards are guaranteed to have the same value
//take a card from current hand and make a new hand with it.
Hand* Hand::split(Deck* deck)
{
  Hand* new_hand = new Hand(m_bet);

  Card* tmp = m_cards[1];
  m_cards.erase(m_cards.begin() + 1);

  new_hand->hit(tmp);

  this->hit(deck->deal_top_card());
  new_hand->hit(deck->deal_top_card());

  return new_hand;
}

int Hand::value()
{
  int soft_value = this->soft_value();
  int hard_value = this->hard_value();

  if(soft_value < 21)
    return soft_value;

  else if(soft_value == 21)
  {
    m_cur_playing = false;
    m_completed = true;

    return soft_value;
  }

  else //if(soft_value > 21)
  {
    if(hard_value == 21)
    {
      m_cur_playing = false;
      m_completed = true;
    }
    if(hard_value > 21)
    {
      m_bust = true;
      m_cur_playing = false;
      m_completed = true;
    }

    return hard_value;
  }
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

bool Hand::is_first_card_ace()
{
  if(m_cards.size() == 0 || m_cards[0]->get_value() != ACE)
    return false;

  return true;
}

bool Hand::is_blackjack()
{
  if(m_cards.size() == 2 && this->soft_value() == 21)
    return true;

  return false;
}

void Hand::print()
{
  for(unsigned int i = 0; i < m_cards.size(); i++)
  {
    cout << " " << m_cards[i]->print();
    if(i < (m_cards.size() - 1)) //if not the last card
      cout << ",";
  }
  if(this->is_blackjack()) cout << "\t[BLACKJACK]";
  else if(m_bust)          cout << "\t[BUST]";
  else                     cout << "\t[" << this->value() << "]";

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
