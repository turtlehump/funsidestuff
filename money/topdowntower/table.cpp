#include "table.h"

Table::Table()
{
  m_deck = NULL;
  m_savior_card = NULL;
  m_player = NULL;
  m_bet_multiplier = 0;

  return;
}

Table::~Table()
{
  delete m_deck;
  delete m_savior_card;
  delete m_player;

  return;
}

void Table::set_player()
{
  for(int i = 0; i < 59; i++)
    cout << endl;

  string name;
  cout << "Whats your name? ";
  cin >> name;
  m_player = new Player(name);

  for(int i = 0; i < 59; i++)
    cout << endl;

  return;
}

void Table::simulation()
{
  bool should_play_again;
  do
  {
    this->ask_for_bet();

    m_deck = new Deck();
    m_deck->shuffle();
    this->fill_tower();

    this->print_tower();

    cout << endl;

    should_play_again = ask_play_again();

  }while(should_play_again);

  cout << endl;
  m_player->print();
  cout << endl;

  return;
}

void Table::ask_for_bet()
{
  for(int i = 0; i < 59; i++)
    cout << endl;

  m_player->print();
  cout << endl;

  int units, bet;
  do
  {
    do
    {
      cout << "Your bet will be in units of $15. " << endl;
      cout << "How many units do you want to bet? (1-5) " << endl;
      cin >> units;

      if(units < 1)
      {
        cout << endl << endl;
        cout << "You must bet a positive number of units.";
        cout << endl;
      }
      if(units > 5)
      {
        cout << endl << endl;
        cout << "You must bet less than 5 units.";
        cout << endl;
      }
    }while(units < 1 || units > 5);

    bet = units * 15;

    if(m_player->can_afford(bet))
      m_bet_multiplier = units;
    else
    {
      cout << endl << m_player->get_name() << " cannot afford ";
      cout << "this bet of $" << bet << " (";
      m_player->print();
      cout << ")" << endl << endl;
    }
  }while(!m_player->can_afford(bet));

  m_player->take_money(bet);

  return;
}

void Table::fill_tower()
{
  for(int i = 1; i < 8; i++) //i will be the row (1-7)
  {
    for(int j = 0; j < i; j++) //j will be the index (depends on row)
    {
      //I was hoping to be able to make a string and use that
      //string as a variable name, but variable names are set at 
      //compile time and strings are evaluated at run time
      if(i == 1)
        m_row1[j] = m_deck->deal_top_card();

      else if(i == 2)
        m_row2[j] = m_deck->deal_top_card();

      else if(i == 3)
        m_row3[j] = m_deck->deal_top_card();

      else if(i == 4)
        m_row4[j] = m_deck->deal_top_card();

      else if(i == 5)
        m_row5[j] = m_deck->deal_top_card();

      else if(i == 6)
        m_row6[j] = m_deck->deal_top_card();

      else if(i == 7)
        m_row7[j] = m_deck->deal_top_card();
    }
  }
  m_savior_card = m_deck->deal_top_card();

  return;
}

void Table::print_tower()
{
  for(int i = 0; i < 59; i++)
    cout << endl;

  for(int i = 1; i < 8; i++) //rows 1-7
  {
    for(int j = 0; j < 7 - i; j++)
      cout << "  ";

    for(int j = 0; j < i; j++)
    {
      cout << "   ";
      if(i == 1)
        cout << m_row1[j]->get_display_value();
      if(i == 2)
        cout << m_row2[j]->get_display_value();
      if(i == 3)
        cout << m_row3[j]->get_display_value();
      if(i == 4)
        cout << m_row4[j]->get_display_value();
      if(i == 5)
        cout << m_row5[j]->get_display_value();
      if(i == 6)
        cout << m_row6[j]->get_display_value();
      if(i == 7)
        cout << m_row7[j]->get_display_value();
    }
    cout << endl;
  }

  cout << endl;

  //following the same format
  for(int i = 0; i < 6; i++)
    cout << "  ";
  cout << "   "; 
  cout << m_savior_card->get_display_value() << endl;

  return;
}

bool Table::ask_play_again()
{
  string play_again;

  do
  {
    cout << "Do you want to play again? (y/n) ";
    cin >> play_again;

    if(play_again == "y" || play_again == "Y")
      return true;
    else if(play_again == "n" || play_again == "N")
      return false;
    else
      cout << endl << "That is not a valid input." << endl << endl;

  }while(!(play_again == "y" || play_again == "Y" ||
           play_again == "n" || play_again == "N" ));

  //we will never get out the loop
  return 420; //:D
}
