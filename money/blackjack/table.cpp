#include "table.h"

Table::~Table()
{
  delete m_dealer;
  delete m_deck;
  for(unsigned int i = 0; i < m_players.size(); i++)
    delete m_players[i];
}

void Table::setup_game()
{
  this->set_table_limits();
  this->set_deck();
  this->get_players();

  return;
}

void Table::simulation()
{
  int play_again;
  bool repeat_last_hand = false;

  do
  {
    this->play_an_entire_hand(repeat_last_hand);
    play_again = this->ask_play_again();

    this->purge_broke_players();

    if(play_again == 3) repeat_last_hand = true;
    else                repeat_last_hand = false;

  }while(play_again == 1 || play_again == 3);

  this->player_money_print();

  return;
}

void Table::set_table_limits()
{
  int min = 0, max = 0;

  this->clear_screen_print();

  do
  {
    cout << "What is the table minimum bet? ";
    cin >> min;
    while(cin.fail())
    {
      cout << endl << "Expecting an int." << endl << endl;
      cin.clear();
      cin.ignore();
      cout << "What is the table minimum bet? ";
      cin >> min;
    }
    if(min < 1)
      cout << endl << "The table must have a positive minimum bet." << endl << endl;
  }while(min < 1);
  //minimum must be positive to continue

  do
  {
    cout << "And the maximum bet? ";
    cin >> max;
    while(cin.fail())
    {
      cout << endl << "Expecting an int." << endl << endl;
      cin.clear();
      cin.ignore();
      cout << "And the maximum bet? ";
      cin >> max;
    }
    if(max < (5 * min))
    {
      cout << endl << "The maximum must be at least 5x the minimum bet of ";
      cout << min << " (" << (5 * min) << ")" << endl << endl;
    }
  }while(max < (5 * min));

  m_min_bet = min;
  m_max_bet = max;

  return;
}

void Table::set_deck()
{
  int num_decks = 0;

  this->clear_screen_print();

  do
  {
    cout << "How many decks are we going to use? ";
    cin >> num_decks;
    while(cin.fail())
    {
      cout << endl << "Expecting an int." << endl << endl;
      cin.clear();
      cin.ignore();
      cout << "How many decks are we going to use? ";
      cin >> num_decks;
    }
    if(num_decks < 1)
      cout << endl << "Number of decks must be positive." << endl << endl;
  }while(num_decks < 1);

  m_deck = new Deck(num_decks * 52);

  m_deck->shuffle();
  m_deck->cut();

  return;
}

void Table::get_players()
{
  int num_players = 0;
  
  this->clear_screen_print();

  do
  {
    cout << "How many players are at the table? ";
    cin >> num_players;
    while(cin.fail())
    {
      cout << endl << "Expecting an int." << endl << endl;
      cin.clear();
      cin.ignore();
      cout << "How many players are at the table? ";
      cin >> num_players;
    }
    if(num_players < 1)
      cout << endl << "Must have a positive number of players." << endl << endl;
  }while(num_players < 1);

  cout << endl;
  for(int i = 0; i < num_players; i++)
  {
    string name;
    do
    {
      cout << "Player " << i + 1 << "'s name: ";
      cin.ignore();
      getline(cin, name);
      cin.unget();
      if(name == "Dealer")
        cout << endl << "Player cannot be named \"Dealer\"." << endl << endl;
    }while(name == "Dealer");

    Player* new_player = new Player(name);
    m_players.push_back(new_player);
  }

  return;
}

/*vvvvvvvvv THE BEEF vvvvvvvvv*/
void Table::play_an_entire_hand(bool repeat_last_hand)
{
  this->set_hands_for_players(repeat_last_hand);

  this->starting_deal();

  if(m_dealer->should_offer_insurance())
  {
    for(unsigned int i = 0; i < m_players.size(); i++)
      m_players[i]->set_insurance();

    if(m_dealer->has_blackjack())
    {
      this->table_final_print(21);

      //payout insurance (2:1 plus the bet back)
      for(unsigned int i = 0; i < m_players.size(); i++)
        m_players[i]->give_money(3 * m_players[i]->get_insurance());

      //new hand right away
      m_dealer->reset_hand();
      for(unsigned int i = 0; i < m_players.size(); i++)
        m_players[i]->reset_hands();

      return;
    }
  }

  for(unsigned int i = 0; i < m_players.size(); i++)
    this->player_play(m_players[i]);

  int dealer_hand_value = 0;
  if(this->dealer_should_play())
    dealer_hand_value = this->dealer_play();

  this->table_final_print(dealer_hand_value); 

  //determine winners and pay them
  this->pay_winners(dealer_hand_value);

  m_dealer->reset_hand();
  for(unsigned int i = 0; i < m_players.size(); i++)
    m_players[i]->reset_hands();

  return;
}
/*^^^^^^^^^ THE BEEF ^^^^^^^^^*/

//return 0 if all players are broke
//return 1 if yes play again
//return 2 if no play again
//return 3 if repeat last hand
int Table::ask_play_again()
{
  if(this->all_players_are_broke())
  {
    cout << endl << "Holy smokes... That is an epic fail." << endl;
    return 0;
  }
  else
  {
    for(unsigned int i = 0; i < m_players.size(); i++)
    {
      if(m_players[i]->get_money_count() == 0)
        cout << endl << m_players[i]->get_name() << " IS BROKE" << endl << endl;
    }
  }

  string play_again;
  bool good_input = true;

  bool can_repeat = this->all_players_can_repeat();

  do
  {
    if(can_repeat) cout << "Play again? (y/n/r) ";
    else           cout << "Play again? (y/n) (no repeat) ";
    cin >> play_again;

    if(play_again == "y" || play_again == "Y")
      return 1;
    else if(play_again == "n" || play_again == "N")
      return 2;
    else if(play_again == "r" || play_again == "R")
    {
      if(can_repeat)
        return 3;
      else
      {
        good_input = false;
        cout << endl << "That is not a valid option." << endl << endl;
      }
    }
    else
    {
      good_input = false;
      cout << endl << "That is not a valid option." << endl << endl;
    }
  }while(!good_input);
  //control will never get out of the loop

  return 420; // :D
}

void Table::set_hands_for_players(bool repeat_last_hand)
{
  int num_hands = 0;;
  double bet = 0;

  if(repeat_last_hand)
  {
    this->clear_screen_print();
    this->player_money_print();
    cout << "Count: " << m_deck->get_count() << endl;
    cout << "Dealt: " << m_deck->get_cards_dealt() << endl;
    sleep(1);

    for(unsigned int i = 0; i < m_players.size(); i++)
      m_players[i]->repeat_last_hand();
  }
  else //if(!repeat_last_hand)
  {
    this->clear_screen_print();
    this->player_money_print();
    cout << "Count: " << m_deck->get_count() << endl;
    cout << "Dealt: " << m_deck->get_cards_dealt() << endl;
    cout << endl;

    for(unsigned int i = 0; i < m_players.size(); i++)
    {
      do
      {
        do
        {
          cout << "How many hands does " << m_players[i]->get_name() << " want to play? ";
          cin >> num_hands;
          while(cin.fail())
          {
            cout << endl << "Expecting an int." << endl << endl;
            cin.clear();
            cin.ignore();
            cout << "How many hands does " << m_players[i]->get_name() << " want to play? ";
            cin >> num_hands;
          }
          if(num_hands < 1)
            cout << endl << "You must play a positive number of hands." << endl << endl;
        }while(num_hands < 1);

        do
        {
          cout << "And the bet (" << m_min_bet << "-" << m_max_bet << ")? ";
          cin >> bet;
          while(cin.fail())
          {
            cout << endl << "Expecting an number." << endl << endl;
            cin.clear();
            cin.ignore();
            cout << "And the bet (" << m_min_bet << "-" << m_max_bet << ")? ";
            cin >> bet;
          }
          if(bet < m_min_bet)
            cout << endl << "Table minimum is " << m_min_bet << endl << endl;
          if(bet > m_max_bet)
            cout << endl << "Table maximum is " << m_max_bet << endl << endl;
        }while(bet < m_min_bet || bet > m_max_bet);

        if(!m_players[i]->can_afford(num_hands * bet))
        {
          cout << endl << m_players[i]->get_name();
          cout << " cannot afford this play of $" << num_hands * bet;
          cout << " (" << m_players[i]->get_name();
          cout << ": $" << m_players[i]->get_money_count() << ")";
          cout << endl << endl;
        }

      }while(!m_players[i]->can_afford(num_hands * bet));

      m_players[i]->set_hands_and_bet_for_replay(num_hands, bet);

      for(int j = 0; j < num_hands; j++)
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
        m_players[i]->get_next_hand(true)->hit(m_deck->deal_top_card());
        this->table_dealing_print();
      }
    }
    m_dealer->hit(m_deck->deal_top_card()); 
    this->table_dealing_print();
  }

  return;
}

void Table::player_play(Player* player)
{
  player->start_playing();

  int hand_num = 1; //used for anouncing

  Hand* playing_hand = player->get_next_hand(true);
  //need to revert back to the starting hand,
  //just like we were still dealing the starting cards

  while(playing_hand)
  {
    if(!playing_hand->is_blackjack()) 
    {
      playing_hand->start_playing();
      this->hand_play(playing_hand, player, hand_num);
    }
    playing_hand = player->get_next_hand(false);
    hand_num++;
  }
  player->stand();

  return;
}

int Table::hand_play(Hand* hand, Player* player, int hand_num)
{
  int option = 0;
  do
  {
    this->table_playing_print(player->get_name(), hand_num);
    cout << "What do you want to do? " << endl << endl;
    cout << "1) hit";
    if(hand->can_double_down() && player->can_afford(hand->get_bet()))
      cout << "            3) double down";
    cout << endl;
    cout << "2) stand";
    if(hand->can_split() && player->can_afford(hand->get_bet()))
      cout << "          4) split";
    cout << endl;

    cin >> option;
    while(cin.fail())
    {
      cout << endl << "Expecting an int." << endl << endl;
      cin.clear();
      cin.ignore();
      cout << "What do you want to do? ";
      cin >> option;
    }

    switch(option)
    {
      case 1:
        hand->hit(m_deck->deal_top_card());
        break;

      case 2:
        return hand->stand();

      case 3:
        if(hand->can_double_down())
        {
          int bet = hand->get_bet();
          if(player->can_afford(bet))
          { 
            player->take_money(bet);
            int hand_value = hand->double_down(m_deck->deal_top_card());
            this->table_playing_print(player->get_name(), hand_num);

            //keep it aligned for easy tracking of cards
            cout << endl << endl << endl << endl << endl;

            sleep(1);
            return hand_value;
          }
          else
          {
            cout << endl << "You do not have enough to double down on this bet (";
            cout << bet << ")." << endl << endl;
            sleep(1);
          }
        }
        else
        {
          cout << endl << "That is not a valid option." << endl << endl;
          sleep(1);
        }
        break;

      case 4:
        if(hand->can_split())
        {
          int bet = hand->get_bet();
          if(player->can_afford(bet))
            player->add_hand(hand->split(m_deck));
          else
          {
            cout << endl << "You do not have enough to split on this bet (";
            cout << bet << ")." << endl << endl;
            sleep(1);
          }
        }
        else
        {
          cout << endl << "That is not a valid option." << endl << endl;
          sleep(1);
        }
        break;

      default:
        cout << endl << "That is not a valid option." << endl << endl;
        sleep(1);
     }

  }while(!hand->has_bust() && hand->value() != 21);

  this->table_playing_print(player->get_name(), hand_num);

  return 0;
}

bool Table::dealer_should_play()
{
  for(unsigned int i = 0; i < m_players.size(); i++)
  {
    if(m_players[i]->all_hands_are_bust_or_blackjack());
      //do nothing

    else //at least 1 hand was not a bust or a blackjack
      return true;
  }

  //every hand was either a bust or a blackjack
  return false;
}

int Table::dealer_play()
{
  m_dealer->start_playing();
  this->table_playing_print("Dealer", 0);

  //by the time this function is called the dealer should already have 2 cards
  while(m_dealer->soft_hand_value() < 17)
  {
    m_dealer->hit(m_deck->deal_top_card());
    this->table_playing_print("Dealer", 0);
  }
  //the soft hand is at least 17

  if(m_dealer->soft_hand_value() == 17 && m_dealer->hard_hand_value() != 17)
  {
    m_dealer->hit(m_deck->deal_top_card()); //hit on soft 17
    this->table_playing_print("Dealer", 0);
  }

  if(m_dealer->soft_hand_value() >= 18 && m_dealer->soft_hand_value() <= 21)
  {
    sleep(1);
    m_dealer->done_playing();
    return m_dealer->soft_hand_value();
  }
  else //if(m_dealer->soft_hand_value() > 21)
  {
    while(m_dealer->hard_hand_value() < 17)
    {
      m_dealer->hit(m_deck->deal_top_card());
      this->table_playing_print("Dealer", 0);
    }
    m_dealer->done_playing();
    return m_dealer->hard_hand_value();
  }
}

void Table::pay_winners(int dealer_hand_value)
{
  for(unsigned int i = 0; i < m_players.size(); i++)
    m_players[i]->give_money(m_players[i]->determine_payout(dealer_hand_value, m_dealer->has_blackjack()));

  return;
}

void Table::table_dealing_print()
{
  bool final_print = false;
  this->clear_screen_print();
  cout << "****STARTING DEAL****" << endl << endl;

  //Dealer is on top for display reasons
  m_dealer->print(final_print);
  cout << endl;
  for(unsigned int i = 0; i < m_players.size(); i++)
    m_players[i]->print();

  //this is to kkep it aligned throughout all prints so its easier to track
  cout << endl << endl << endl << endl;

  return;
}

void Table::table_playing_print(string name, int hand_num)
{
  bool final_print = false;
  this->clear_screen_print();
  if(name == "Dealer")
    cout << "****DEALER IS PLAYING****" << endl << endl;
  else
    cout << "****" << name << " IS PLAYING: HAND " << hand_num << "****" << endl << endl;

  //Dealer is on top for display reasons
  m_dealer->print(final_print);
  cout << endl;
  for(unsigned int i = 0; i < m_players.size(); i++)
    m_players[i]->print();

  //this is to keep it aligned throughout all prints so its easier to track
  if(name == "Dealer")
    cout << endl << endl << endl << endl;

  return;
}

void Table::table_final_print(int dealer_hand_value)
{
  bool final_print = true;
  this->clear_screen_print();
  cout << "****END OF HAND****" << endl << endl;

  //Dealer is on top for display reasons
  m_dealer->print(final_print);
  cout << endl;
  for(unsigned int i = 0; i < m_players.size(); i++)
    m_players[i]->print();

  cout << endl;

  cout << "The dealer got ";
  if(m_dealer->has_blackjack()) cout << "a Blackjack.";
  else                          cout << dealer_hand_value << ".";

  //this is to keep it aligned throughout all prints so its easier to track
  cout << endl << endl;

  return;
}

void Table::clear_screen_print()
{
  for(int i = 0; i < 59; i++)
    cout << endl;

  return;
}

void Table::player_money_print()
{
  cout << endl;
  for(unsigned int i = 0; i < m_players.size(); i++)
  {
    cout << m_players[i]->get_name() << ": $" << m_players[i]->get_money_count();
    cout << endl;
  }
  cout << endl;

  return;
}

bool Table::all_players_are_broke()
{
  for(unsigned int i = 0; i < m_players.size(); i++)
  {
    if(m_players[i]->get_money_count() > 0)
      return false;
  }

  return true;
}

bool Table::all_players_can_repeat()
{
  for(unsigned int i = 0; i < m_players.size(); i++)
  {
    if(!m_players[i]->can_repeat_last_play())
      return false;
  }

  return true;
}

void Table::purge_broke_players()
{
  for(unsigned int i = 0; i < m_players.size(); i++)
  {
    if(m_players[i]->get_money_count() == 0)
    {
      delete m_players[i];
      m_players.erase(m_players.begin() + i);
    }
  }

  return;
}
