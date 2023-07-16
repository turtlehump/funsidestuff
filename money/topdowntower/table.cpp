#include "table.h"

Table::Table()
{
  m_deck = NULL;
  m_savior_card = NULL;

  return;
}

Table::~Table()
{
  //The actual deck object gets deleted at the end of every round
  
  if(m_savior_card) delete m_savior_card;
  return;
}

void Table::play_round(Player* player)
{
  m_deck = new Deck(); //new deck must be deleted

  int multiplier = this->get_bet(player);

  m_deck->shuffle();
  this->deal();        //cards move from deck to tower -> must delete them

  this->print_tower();

  int current_row = 0, next_row = 1, win_payment = 0;
  bool press_luck;
  do
  {
    sleep(1);

    cout << "Playing row: " << current_row + 1 << endl;

    win_payment = this->play_row(current_row) * multiplier;

    this->print_tower();

    if(win_payment == 0) //LOSS
    {
      sleep(1);
      cout << endl << endl << "You Lose" << endl << endl;
      sleep(2);
      break;
    }

    if(current_row < 7)
      press_luck = this->continue_or_payout(win_payment);
    else //JACKPOT!
    {
      press_luck = false;
      if(m_savior_card) // MEGA JACKPOT!!
      {
         win_payment *= 25;
      }
      sleep(1);
    }

    current_row = next_row;
    next_row++;

  }while(current_row < 7 && press_luck);

  display_winnings(win_payment, current_row);
  player->give_money(win_payment);
  cout << endl; player->print(); cout << endl;

  this->delete_cards_in_tower();
  delete m_deck;
  return;
}

void Table::print_tower()
{
  for(unsigned long int i = 0; i < m_tower.size(); i++)
  {
    cout << i + 1 << ":";
    for(unsigned long int j = 0; j < m_tower.size() - i; j++)
    {
      cout << "  ";
    }
    for(unsigned long int j = 0; j < m_tower[i].size(); j++)
    {
      cout << " " << m_tower[i][j]->display_value() << " ";
    }
    cout << endl;
  }

  cout << "s:";
  for(unsigned long int i = 0; i < m_tower.size(); i++)
    cout << "  ";
  if(!m_savior_card) cout << "X";
  else               cout << m_savior_card->display_value();
  cout << endl;

  return;
}


int Table::get_bet(Player* player)
{
  this->clear_screen();

  player->print();
  cout << endl;

  if(player->money() < 15)
  {
     cout << endl << endl << "Too poor to play" << endl;
     return 0;
  }

  int multiplier, bet;
  do
  {
    do
    {
      cout << "Your bet will be in multiples of $15. " << endl;
      cout << "How many multiples? (1-5) " << endl;
      cin >> multiplier;


      if(multiplier < 1)
      {
        cout << endl << endl;
        cout << "You must bet a positive number of units.";
        cout << endl;
      }
      if(multiplier > 5)
      {
        cout << endl << endl;
        cout << "You must bet less than 5 units.";
        cout << endl;
      }
    }while(multiplier < 1 || multiplier > 5);

    bet = multiplier * 15;

    if(player->can_afford(bet))
    {
      cout << "Bet of " << bet << endl;
      player->take_money(bet);
      return multiplier;
    }
    else
    {
      cout << endl << "You cannot afford a bet of $" << bet << endl;
      sleep(1);
    }
  }while(1);

  return 420; //:D
}

void Table::deal()
{

  for(unsigned long int i = 0; i < m_tower.size(); i++)
     for(unsigned long int j = 0; j < m_tower[i].size(); j++)
       m_tower[i][j] = m_deck->deal_top_card();

  m_savior_card = m_deck->deal_top_card();

  return;
}

//returns total winning value for the row
//To-Do:
//corrects a conflict if needed/possible
int Table::play_row(int current_row)
{
  bool conflicts = false;
  int winnings = 0, multiplier = 1;

  if(current_row == 0)
  {
    m_tower[0][0]->reveal();

    //special things for wilds and multipliers on the first card
    switch(m_tower[0][0]->get_face())
    {
      case WILD:  return 10;
      case XTWO:  return 10;
      case XFIVE: return 15;
      case XTEN:  return 25;
      default:    return m_tower[0][0]->money_value();
    }
  }

  //vvvvvvvvvvvv  THE MEAT vvvvvvvvvvvvvvv 
  else
  {
    for(unsigned long int i = 0; i < m_tower[current_row].size(); i++)
    {
      m_tower[current_row][i]->reveal();

      winnings += m_tower[current_row][i]->money_value();

      switch(m_tower[current_row][i]->get_face())
      {
        case XTWO:  multiplier *= 2; break;
        case XFIVE: multiplier *= 5; break;
        case XTEN:  multiplier *= 10; break;
        default:    break;
      }

      if(i == 0)  //far left card
      {
        if(this->conflict(m_tower[current_row][i], m_tower[current_row - 1][i]))
          conflicts = true;
      }
      else if(i == m_tower[current_row].size() - 1)  //far right card
      {
        if(this->conflict(m_tower[current_row][i], m_tower[current_row - 1][i - 1]))
          conflicts = true;
      }
      else if(i < m_tower[current_row].size())  //cards in the middle check both "parents"
      {
        if(this->conflict(m_tower[current_row][i], m_tower[current_row - 1][i]))
          conflicts = true;

        if(this->conflict(m_tower[current_row][i], m_tower[current_row - 1][i - 1]))
          conflicts = true;
      }

      if(conflicts) multiplier = 0;
    }

    if(winnings == 0) //full row of multiplers
    {
       winnings = 50 * current_row;
    }
  }

  if(conflicts)
    multiplier = handle_conflicts(current_row);

  return winnings * multiplier;
}
// ^^^^^^^^^^ THE MEAT ^^^^^^^^^^^^^^^^^

bool Table::conflict(Card* lower, Card* upper)
{
  if(lower->get_face() == upper->get_face())
  {
    switch(lower->get_face())
    {
      case ONE:
      case TWO:
      case THREE:
      case FOUR:
      case FIVE:
      case SIX:
      case SEVEN:
        return true;
      default:
        return false;
    }
  }
  return false;
}

int Table::handle_conflicts(int current_row)
{
  return 0;
}

bool Table::continue_or_payout(int takehome_payment)
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


void Table::display_winnings(int win_payment, int current_row)
{
  this->reveal_all_cards();
  this->print_tower();

  cout << endl << "Payout of $" << win_payment << " from row " << current_row + 1 << endl;

  return;
}

void Table::reveal_all_cards()
{
  for(unsigned long int i = 0; i < m_tower.size(); i++)
  {
    for(unsigned long int j = 0; j < m_tower[i].size(); j++)
    {
        m_tower[i][j]->reveal();
    }
  }

  if(m_savior_card) m_savior_card->reveal();

  return;
}

void Table::delete_cards_in_tower()
{
  for(unsigned long int i = 0; i < m_tower.size(); i++)
  {
    for(unsigned long int j = 0; j < m_tower[i].size(); j++)
    {
       delete m_tower[i][j];
    }
  }
  return;
}
