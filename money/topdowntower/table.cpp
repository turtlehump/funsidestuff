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

    int i = 0; //increment on first loop for rows 1-7
    bool should_continue;
    int takehome_payment;
    do
    {
      i++;
      sleep(1);
      takehome_payment = this->flip_and_evaluate_row(i);
      this->print_tower();
      if(i < 7)
        should_continue = this->ask_continue(takehome_payment);
      else
      {
        should_continue = false;
        if(m_savior_card)
          takehome_payment += 30;
        sleep(1);
      }
      takehome_payment *= m_bet_multiplier;
    }while(i < 7 && should_continue);

    this->flip_all_cards();
    this->print_tower();

    cout << endl << "Payout of $" << takehome_payment;
    cout << " from row " << i;
    if(m_savior_card && i == 7)
      cout << " (including the +30 from the savior card)";
    cout << "." << endl << endl;

    m_player->give_money(takehome_payment);

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
  for(int i = 1; i <= 7; i++) //i will be the row (1-7)
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

int Table::flip_and_evaluate_row(int row)
{
  int takehome_payment = 0;
  for(int i = 0; i < row; i++)
  {
    int card_value;
    if(row == 1)
    {
      card_value = m_row1[i]->get_value();
      if(card_value == 0)
        card_value = this->joker_max_value(row, i);
      takehome_payment += card_value;
      m_row1[i]->flip();
    }
    else if(row == 2)
    {
      card_value = m_row2[i]->get_value();
      if(card_value == 0)
        card_value = this->joker_max_value(row, i);
      takehome_payment += card_value;
      m_row2[i]->flip();
    }
    else if(row == 3)
    {
      card_value = m_row3[i]->get_value();
      if(card_value == 0)
        card_value = this->joker_max_value(row, i);
      takehome_payment += card_value;
      m_row3[i]->flip();
    }
    else if(row == 4)
    {
      card_value = m_row4[i]->get_value();
      if(card_value == 0)
        card_value = this->joker_max_value(row, i);
      takehome_payment += card_value;
      m_row4[i]->flip();
    }
    else if(row == 5)
    {
      card_value = m_row5[i]->get_value();
      if(card_value == 0)
        card_value = this->joker_max_value(row, i);
      takehome_payment += card_value;
      m_row5[i]->flip();
    }
    else if(row == 6)
    {
      card_value = m_row6[i]->get_value();
      if(card_value == 0)
        card_value = this->joker_max_value(row, i);
      takehome_payment += card_value;
      m_row6[i]->flip();
    }
    else if(row == 7)
    {
      card_value = m_row7[i]->get_value();
      if(card_value == 0)
        card_value = this->joker_max_value(row, i);
      takehome_payment += card_value;
      m_row7[i]->flip();
    }
  }

  return takehome_payment;
}

//row can be from 1-7
//index can be from 0-6
int Table::joker_max_value(int row, int index)
{
  int max_value = 7;
  switch(row)
  {
    case 1:
      break;

    case 2: //only one card to check for row 2
      if(m_row1[0]->get_value() == 7)
        max_value = 6;
      break;

    case 3:
      if(index == 0 && m_row2[0]->get_value() == 7)
        max_value = 6;
      else if(index == (row - 1) &&
              m_row2[row - 1]->get_value() == 7)
        max_value = 6;
      else //middle index
        if(m_row2[index - 1]->get_value() == 7 ||
           m_row2[index]->get_value() == 7)
        {
          if(m_row2[index - 1]->get_value() == 6 ||
             m_row2[index]->get_value() == 6)
            max_value = 5;
          else
            max_value = 6;
        }
      break;

    case 4:
      if(index == 0 && m_row3[0]->get_value() == 7)
        max_value = 6;
      else if(index == (row - 1) &&
              m_row3[row - 1]->get_value() == 7)
        max_value = 6;
      else //middle index
        if(m_row3[index - 1]->get_value() == 7 ||
           m_row3[index]->get_value() == 7)
        {
          if(m_row3[index - 1]->get_value() == 6 ||
             m_row3[index]->get_value() == 6)
            max_value = 5;
          else
            max_value = 6;
        }
      break;

    case 5:
      if(index == 0 && m_row4[0]->get_value() == 7)
        max_value = 6;
      else if(index == (row - 1) &&
              m_row4[row - 1]->get_value() == 7)
        max_value = 6;
      else //middle index
        if(m_row4[index - 1]->get_value() == 7 ||
           m_row4[index]->get_value() == 7)
        {
          if(m_row4[index - 1]->get_value() == 6 ||
             m_row4[index]->get_value() == 6)
            max_value = 5;
          else
            max_value = 6;
        }
      break;

    case 6:
      if(index == 0 && m_row5[0]->get_value() == 7)
        max_value = 6;
      else if(index == (row - 1) &&
              m_row5[row - 1]->get_value() == 7)
        max_value = 6;
      else //middle index
        if(m_row5[index - 1]->get_value() == 7 ||
           m_row5[index]->get_value() == 7)
        {
          if(m_row5[index - 1]->get_value() == 6 ||
             m_row5[index]->get_value() == 6)
            max_value = 5;
          else
            max_value = 6;
        }
      break;

    case 7:
      if(index == 0 && m_row6[0]->get_value() == 7)
        max_value = 6;
      else if(index == (row - 1) &&
              m_row6[row - 1]->get_value() == 7)
        max_value = 6;
      else //middle index
        if(m_row6[index - 1]->get_value() == 7 ||
           m_row6[index]->get_value() == 7)
        {
          if(m_row6[index - 1]->get_value() == 6 ||
             m_row6[index]->get_value() == 6)
            max_value = 5;
          else
            max_value = 6;
        }
      break;
  }

  return max_value;
}

bool Table::ask_continue(int takehome_payment)
{
  string should_continue;
  do
  {
    cout << "Continue or payout $" << takehome_payment << "? (c/p) ";
    cin >> should_continue;
  
    if(should_continue == "c" || should_continue == "C")
      return true;
    else if(should_continue == "p" || should_continue == "P")
      return false;
    else
      cout << endl << "That is an invalid option."  << endl << endl;

  }while(!(should_continue == "c" || should_continue == "C" ||
           should_continue == "p" || should_continue == "P"));

  //we will never get out of the loop
  return 420; //:D
}

void Table::print_tower()
{
  for(int i = 0; i < 59; i++)
    cout << endl;

  for(int i = 1; i <= 7; i++) //rows 1-7
  {
    cout << i << ":";
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
  cout << "s:";
  for(int i = 0; i < 6; i++)
    cout << "  ";
  cout << "   "; 
  cout << m_savior_card->get_display_value() << endl;

  return;
}

void Table::flip_all_cards()
{
  for(int i = 1; i <= 7; i++) //rows 1-7
  {
    for(int j = 0; j < i; j++)
    {
      if(i == 1)
        m_row1[j]->flip();
      if(i == 2)
        m_row2[j]->flip();
      if(i == 3)
        m_row3[j]->flip();
      if(i == 4)
        m_row4[j]->flip();
      if(i == 5)
        m_row5[j]->flip();
      if(i == 6)
        m_row6[j]->flip();
      if(i == 7)
        m_row7[j]->flip();
    }
  }
  m_savior_card->flip();

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
