#include "player.h"

Player::~Player()
{
  for(unsigned int i = 0; i < m_cards.size(); i++)
  {
    delete m_cards[i];
  }
}
int Player::hit(Card* new_card)
{
  m_cards.push_back(new_card);
  int hand_value = 0;
/*
  for(unsigned int i = 0; i < m_cards.size(); i++)
    hand_value += m_cards[i]->get_blackjack_value();
*/

  return hand_value;
}

void Player::reset_hand()
{
  m_cards.clear();
  cout << m_name << " is waiting for a new hand" << endl;
  m_cur_playing = false;
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
void Player::dealer_print()
{
  cout << "Dealer:";

  if(m_cards.size() == 0)
  {
    cout << endl;
    return;
  }
  if(m_cards.size() == 1)
  {
    cout << " "; m_cards[0]->print(); cout << endl;
    return;
  }

  if(m_cards.size() == 2)
  {
    if(m_cur_playing){ //if its the dealers turn to play
      cout << " "; m_cards[0]->print(); cout << ", "; m_cards[1]->print();}
    else{
      cout << " "; m_cards[0]->print(); cout << ", -";}
  }
  else //its obviously the dealers turn because they have more than 2 cards, just print the list
  {
    for(unsigned int i = 0; i < m_cards.size(); i++)
    {
      cout << " "; m_cards[i]->print();
      if(i < (m_cards.size() - 1))
        cout << ",";
    }
  }
  cout << endl;
  return;
}

int Player::play_blackjack(Deck* deck)
{
  //by the time this function is called the player should already have 2 cards
  cout << m_name << "'s turn to play." << endl;
  m_cur_playing = true;
  return this->soft_hand_value();
}

int Player::dealer_play_blackjack(Deck* deck)
{
  //by the time this function is called the dealer should already have 2 cards
  cout << m_name << "'s turn to play." << endl;

  while(this->soft_hand_value() < 17)
    this->hit(deck->deal_top_card());
  //the soft hand is at least 17

  if(this->soft_hand_value() == 17 && this->hard_hand_value() != 17)
    this->hit(deck->deal_top_card()); //hit on soft 17

  if(this->soft_hand_value() >= 18 && this->soft_hand_value() <= 21)
    return this->soft_hand_value();
  else //if(this->soft_hand_value() > 21)
  {
    while(this->hard_hand_value() < 17)
      this->hit(deck->deal_top_card());
    return hard_hand_value();
  }
}

int Player::soft_hand_value()
{
  int hand_value = 0;
  for(unsigned int i = 0; i < m_cards.size(); i++)
    hand_value += m_cards[i]->soft_blackjack_value();
  return hand_value;
}

int Player::hard_hand_value()
{
  int hand_value = 0;
  for(unsigned int i = 0; i < m_cards.size(); i++)
    hand_value += m_cards[i]->hard_blackjack_value();
  return hand_value;
}
