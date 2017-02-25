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

void Table::print()
{
  for(int i = 0; i < 59; i++)
    cout << endl;
  cout << "****TABLE STATUS****" << endl << endl;

  if(m_dealer->is_playing())
    m_dealer->announce_playing();
  else
  {
    for(unsigned int i = 0; i < m_players.size(); i++)
    {
      if(m_players[i]->is_playing())
      {
        m_players[i]->announce_playing();
        break;
      }
    }
  }

  //Dealer is on top for display reasons
  m_dealer->dealer_print();
  for(unsigned int i = 0; i < m_players.size(); i++)
    m_players[i]->print();
}

void Table::final_print()
{
  for(int i = 0; i < 59; i++)
    cout << endl;
  cout << "****END OF HAND****" << endl << endl;

  //Dealer is on top for display reasons
  m_dealer->print();
  for(unsigned int i = 0; i < m_players.size(); i++)
    m_players[i]->print();

  return;
}

void Table::play_blackjack()
{
  this->starting_deal();

  for(unsigned int i = 0; i < m_players.size(); i++)
  {
    m_players[i]->play_blackjack(m_deck);
  }

  int dealer_hand_value = m_dealer->dealer_play_blackjack(m_deck);

  this->final_print(); 
  cout << "The dealer got " << dealer_hand_value << endl;

  //reset all the hands
  m_dealer->reset_hand();
  for(unsigned int i = 0; i < m_players.size(); i++)
    m_players[i]->reset_hand();

  return;
}

void Table::starting_deal()
{
  //deal two cards to everyone, including the dealer
  for(int starting_cards = 0; starting_cards < 2; starting_cards++)
  {
    for(unsigned int i = 0; i < m_players.size(); i++)
    {
      sleep(1);
      m_players[i]->hit(m_deck->deal_top_card());
      this->print();
    }
    sleep(1);
    m_dealer->hit(m_deck->deal_top_card()); 
    this->print();
  }
  return;
}
