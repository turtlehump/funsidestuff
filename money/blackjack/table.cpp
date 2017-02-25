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

void Table::dealing_print()
{
  for(int i = 0; i < 59; i++)
    cout << endl;
  cout << "****STARTING DEAL****" << endl << endl;

  //Dealer is on top for display reasons
  m_dealer->dealer_print();
  for(unsigned int i = 0; i < m_players.size(); i++)
    m_players[i]->print();
}

void Table::playing_print(Player* player)
{
  for(int i = 0; i < 59; i++)
    cout << endl;
  cout << "****" << player->get_name() << " IS PLAYING****" << endl << endl;

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

void Table::simulation()
{
  char play_again;
  do
  {
    this->play_hand();
    cout << "Play again? (y/n) ";
    cin >> play_again;
  }while(play_again == 'y' || play_again == 'Y');
  return;
}

void Table::play_hand()
{
  this->starting_deal();

  for(unsigned int i = 0; i < m_players.size(); i++)
  {
    this->player_play(m_players[i]);
  }

  int dealer_hand_value = dealer_play();

  this->final_print(); 
  cout << "The dealer got " << dealer_hand_value << endl;

  //reset all the hands
  m_dealer->reset_hand();
  for(unsigned int i = 0; i < m_players.size(); i++)
    m_players[i]->reset_hand();

  return;
}

void Table::player_play(Player* player)
{
  player->start_playing();
  this->playing_print(player);
  sleep(1);

  player->stand();
  return;
}

int Table::dealer_play()
{
  m_dealer->start_playing();
  this->playing_print(m_dealer);
  sleep(1);

  //by the time this function is called the dealer should already have 2 cards
  while(m_dealer->soft_hand_value() < 17)
  {
    m_dealer->hit(m_deck->deal_top_card());
    this->playing_print(m_dealer);
    sleep(1);
  }
  //the soft hand is at least 17

  if(m_dealer->soft_hand_value() == 17 && m_dealer->hard_hand_value() != 17)
  {
    m_dealer->hit(m_deck->deal_top_card()); //hit on soft 17
    this->playing_print(m_dealer);
    sleep(1);
  }

  if(m_dealer->soft_hand_value() >= 18 && m_dealer->soft_hand_value() <= 21)
  {
    m_dealer->stand();
    return m_dealer->soft_hand_value();
  }
  else //if(m_dealer->soft_hand_value() > 21)
  {
    while(m_dealer->hard_hand_value() < 17)
    {
      m_dealer->hit(m_deck->deal_top_card());
      this->playing_print(m_dealer);
      sleep(1);
    }
    m_dealer->stand();
    return m_dealer->hard_hand_value();
  }
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
      this->dealing_print();
    }
    sleep(1);
    m_dealer->hit(m_deck->deal_top_card()); 
    this->dealing_print();
  }
  return;
}
