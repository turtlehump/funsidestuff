#include "table.h"

void Table::print(bool final_print)
{
  cout << "****TABLE STATUS****" << endl;

  //Dealer is on top for display reasons
  //Dealer is the last to get a card (last index in the player vector)
  m_players[m_num_players - 1]->dealer_print(final_print);

  for(unsigned int i = 0; i < m_players.size() - 1; i++)
    m_players[i]->print();
}

void Table::add_player(Player* new_player)
{
  m_num_players++;
  m_players.push_back(new_player);
  return;
}

Player* Table::get_next_player()
{
  m_cur_player_index++;
  return m_players[m_cur_player_index];
}

bool Table::set_deck(Deck* deck)
{
  if(!m_deck)
  {
    m_deck = deck;
    return true;
  }
  else
  {
    cout << "The table already has a deck..." << endl;
    return false;
  }
}
  
void Table::starting_deal()
{
  //deal first cards to everyone, including the dealer
  for(unsigned int i = 0; i < m_players.size(); i++)
  {
    m_players[i]->hit(m_deck->deal_top_card());
    sleep(1);
    this->print(false);
  }
  //deal second card to everyone, including the dealer
  for(unsigned int i = 0; i < m_players.size(); i++)
  {
    m_players[i]->hit(m_deck->deal_top_card());
    sleep(1);
    this->print(false);
  }
}
