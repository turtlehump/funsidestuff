#include "table.h"

Table::~Table()
{
  delete m_dealer;
  delete m_deck;
  for(unsigned int i = 0; i < m_players.size(); i++)
    delete m_players[i];
}

void Table::set_deck()
{
  int num_decks;
  cout << "How many decks are we going to use? ";
  cin >> num_decks;

  Deck* deck = new Deck(num_decks * 52);

  deck->shuffle();
  deck->cut();

  m_deck = deck;
  return;
}

void Table::get_players()
{
  int num_players;
  cout << "How many players are at the table? ";
  cin >> num_players;
  cout << endl;
  for(int i = 0; i < num_players; i++)
  {
    string name;
    cout << "Player " << i << "'s name: ";
    cin.ignore();
    getline(cin, name);
    cin.unget();

    Player* new_player = new Player(name);
    m_players.push_back(new_player);
  }
  return;
}

void Table::print(bool final_print)
{
  for(int i = 0; i < 59; i++)
    cout << endl;
  cout << "****TABLE STATUS****" << endl << endl;

  //Dealer is on top for display reasons
  m_dealer->dealer_print();

  for(unsigned int i = 0; i < m_players.size(); i++)
    m_players[i]->print();
}

void Table::play_blackjack()
{
  this->starting_deal();

  for(unsigned int i = 0; i < m_players.size(); i++)
  {
    m_players[i]->play_blackjack(m_deck);
  }

  int dealer_hand_value = m_dealer->dealer_play_blackjack(m_deck);
  this->print(true);
  cout << "The dealer got " << dealer_hand_value << endl;
  return;
}

void Table::starting_deal()
{
  //deal first cards to everyone, including the dealer
  for(unsigned int i = 0; i < m_players.size(); i++)
  {
    sleep(1);
    m_players[i]->hit(m_deck->deal_top_card());
    this->print(false);
  }
  sleep(1);
  m_dealer->hit(m_deck->deal_top_card()); 
  this->print(false);

  //deal second card to everyone, including the dealer
  for(unsigned int i = 0; i < m_players.size(); i++)
  {
    sleep(1);
    m_players[i]->hit(m_deck->deal_top_card());
    this->print(false);
  }
  sleep(1);
  m_dealer->hit(m_deck->deal_top_card()); 
  this->print(false);
}
