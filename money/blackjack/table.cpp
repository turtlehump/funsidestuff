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
  for(int i = 0; i < 59; i++)
    cout << endl;
  cout << "How many decks are we going to use? ";
  cin >> num_decks;

  m_deck = new Deck(num_decks * 52);

  m_deck->shuffle();
  m_deck->cut();

  return;
}

void Table::get_players()
{
  int num_players;
  for(int i = 0; i < 59; i++)
    cout << endl;
  cout << "How many players are at the table? ";
  cin >> num_players;
  cout << endl;
  for(int i = 0; i < num_players; i++)
  {
    string name;
    cout << "Player " << i + 1 << "'s name: ";
    cin.ignore();
    getline(cin, name);
    cin.unget();

    Player* new_player = new Player(name);
    m_players.push_back(new_player);
  }
  return;
}

void Table::simulation()
{
  char play_again;
  do
  {
    this->play_a_hand();
    cout << "Play again? (y/n) ";
    cin >> play_again;
  }while(play_again == 'y' || play_again == 'Y');
  return;
}

void Table::play_a_hand()
{
  this->set_hands_for_players();

  this->starting_deal();

  for(unsigned int i = 0; i < m_players.size(); i++)
  {
    this->player_play(m_players[i]);
  }

  int dealer_hand_value = dealer_play();

  this->final_print(); 
  cout << endl;
  cout << "The dealer got " << dealer_hand_value;
  if(dealer_hand_value > 21)
    cout << " (BUST!)";
  cout << endl << endl;

  //determine winners and pay them

  //reset all the hands
  m_dealer->reset_hand();
  for(unsigned int i = 0; i < m_players.size(); i++)
    m_players[i]->reset_hands();

  return;
}

void Table::set_hands_for_players()
{
  int num_hands;
  int bet;
  for(int i = 0; i < 59; i++)
    cout << endl;
  for(unsigned int i = 0; i < m_players.size(); i++)
  {
    cout << "How many hands does " << m_players[i]->get_name() << " want to play? ";
    cin >> num_hands;
    cout << "And the bet? ";
    cin >> bet;
    for(int j = 0; j < num_hands; j++)
    {
      m_players[i]->add_hand(new Hand(bet));
    }
  }
  return;
}

void Table::starting_deal()
{
  //deal two cards to everyone, including the dealer
  for(int starting_cards = 0; starting_cards < 2; starting_cards++)
  {
    for(unsigned int i = 0; i < m_players.size(); i++)
    {
      int num_hands_for_player = m_players[i]->get_num_hands();
      for(int j = 0; j < num_hands_for_player; j++)
      {
        m_players[i]->get_next_hand()->hit(m_deck->deal_top_card());
        this->dealing_print();
      }
    }
    m_dealer->hit(m_deck->deal_top_card()); 
    this->dealing_print();
  }
  return;
}

void Table::player_play(Player* player)
{
  player->start_playing();

  int hand_num = 1;
  Hand* playing_hand = player->get_next_hand();
  while(playing_hand)
  {
    playing_hand->start_playing();
    this->hand_play(playing_hand, player->get_name(), hand_num);
    playing_hand = player->get_next_hand();
    hand_num++;
  }
  player->stand();
  return;
}

int Table::hand_play(Hand* hand, string player_name, int hand_num)
{
  int option = 0;
  do
  {
    this->playing_print(player_name, hand_num);
    sleep(1);
    cout << endl;
    cout << "What do you want to do? " << endl;
    cout << "1) hit" << endl;
    cout << "2) stand" << endl;

    cin >> option;

    if(option == 1)
      hand->hit(m_deck->deal_top_card());

  }while(option != 2);

  return hand->stand();
}

int Table::dealer_play()
{
  m_dealer->start_playing();
  this->playing_print("Dealer", 1);

  //by the time this function is called the dealer should already have 2 cards
  while(m_dealer->soft_hand_value() < 17)
  {
    m_dealer->hit(m_deck->deal_top_card());
    this->playing_print("Dealer", 1);
  }
  //the soft hand is at least 17

  if(m_dealer->soft_hand_value() == 17 && m_dealer->hard_hand_value() != 17)
  {
    m_dealer->hit(m_deck->deal_top_card()); //hit on soft 17
    this->playing_print("Dealer", 1);
  }

  if(m_dealer->soft_hand_value() >= 18 && m_dealer->soft_hand_value() <= 21)
  {
    m_dealer->done_playing();
    return m_dealer->soft_hand_value();
  }
  else //if(m_dealer->soft_hand_value() > 21)
  {
    while(m_dealer->hard_hand_value() < 17)
    {
      m_dealer->hit(m_deck->deal_top_card());
      this->playing_print("Dealer", 1);
    }
    m_dealer->done_playing();
    return m_dealer->hard_hand_value();
  }
}

void Table::dealing_print()
{
  bool final_print = false;
  for(int i = 0; i < 59; i++)
    cout << endl;
  cout << "****STARTING DEAL****" << endl << endl;

  //Dealer is on top for display reasons
  m_dealer->print(final_print);
  cout << endl;
  for(unsigned int i = 0; i < m_players.size(); i++)
    m_players[i]->print();
}

void Table::playing_print(string name, int hand_num)
{
  bool final_print = false;
  for(int i = 0; i < 59; i++)
    cout << endl;
  cout << "****" << name << " IS PLAYING: HAND " << hand_num << "****" << endl << endl;

  //Dealer is on top for display reasons
  m_dealer->print(final_print);
  cout << endl;
  for(unsigned int i = 0; i < m_players.size(); i++)
    m_players[i]->print();
}

void Table::final_print()
{
  bool final_print = true;
  for(int i = 0; i < 59; i++)
    cout << endl;
  cout << "****END OF HAND****" << endl << endl;

  //Dealer is on top for display reasons
  m_dealer->print(final_print);
  cout << endl;
  for(unsigned int i = 0; i < m_players.size(); i++)
    m_players[i]->print();

  return;
}
