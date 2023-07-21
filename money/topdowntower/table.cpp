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
      cout << " " << m_tower[i][j]->display_value();
    }
    cout << endl;
  }

  cout << "s: ";
  for(unsigned long int i = 0; i < m_tower.size(); i++)
    cout << "  ";
  if(!m_savior_card) cout << " X";
  else               cout << m_savior_card->display_value();
  cout << endl;

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

void Table::play_round(Player* player)
{
  m_deck = new Deck(); //new deck must be deleted

  int round_multiplier = this->get_bet(player, 15);

  m_deck->shuffle();
  this->deal();        //cards move from deck to tower -> must delete them

  this->clear_screen();
  this->print_tower();
  cout << endl;

  int current_row = 0, next_row = 1, win_payment = 0;
  bool press_luck;
  do
  {
    sleep(1);

    this->clear_screen();

    this->announce_row(current_row);
    cout << endl << endl;

    win_payment = this->play_row(current_row) * round_multiplier;

    if(win_payment == 0) //LOSS
    {
      sleep(1);
      break;
    }

    this->print_tower();

    if(current_row < 7)
      press_luck = this->continue_or_payout(win_payment);
    else //JACKPOT! - Survived row 7
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

  this->clear_screen();

  display_winnings(win_payment, current_row);

  player->give_money(win_payment);
  this->delete_cards_in_tower();
  delete m_deck;
  sleep(1);
  cout << endl; player->print(); cout << endl;

  return;
}

int Table::get_bet(Player* player, int base_bet)
{
  this->clear_screen();

  player->print();
  cout << endl << endl;

  if(player->money() < base_bet)
  {
     cout << endl << endl << "Too poor to play" << endl;
     return 0;
  }

  int round_multiplier, bet;
  do
  {
    do
    {
      cout << "Your bet will be in multiples of $" << base_bet << endl;
      cout << "How many multiples? (1-5) " << endl;
      cin >> round_multiplier;

      if(round_multiplier < 1)
      {
        cout << endl << endl;
        cout << "You must bet a positive number of units.";
        cout << endl;
      }
      if(round_multiplier > 5)
      {
        cout << endl << endl;
        cout << "You must bet less than 5 units.";
        cout << endl;
      }
    }while(round_multiplier < 1 || round_multiplier > 5);

    bet = round_multiplier * base_bet;

    if(player->can_afford(bet))
    {
      this->clear_screen();
      this->announce_bet(bet);
      player->take_money(bet);
      sleep(1);
      return round_multiplier;
    }
    else
    {
      cout << endl << "You cannot afford a bet of $" << bet << endl << endl;
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
        case XTWO:  multiplier *= 2;  break;
        case XFIVE: multiplier *= 5;  break;
        case XTEN:  multiplier *= 10; break;
        default:    break;
      }

      if(i == 0)  //far left card
      {
        if(this->conflict(m_tower[current_row][i], m_tower[current_row - 1][i]))
        {
          m_tower[current_row][i]->make_conflicted();
          m_tower[current_row - 1][i]->make_conflicted();
          conflicts = true;
        }
      }
      else if(i == m_tower[current_row].size() - 1)  //far right card
      {
        if(this->conflict(m_tower[current_row][i], m_tower[current_row - 1][i - 1]))
        {
          m_tower[current_row][i]->make_conflicted();
          m_tower[current_row - 1][i - 1]->make_conflicted();
          conflicts = true;
        }
      }
      else if(i < m_tower[current_row].size())  //cards in the middle - check both "parents"
      {
        if(this->conflict(m_tower[current_row][i], m_tower[current_row - 1][i]))
        {
          m_tower[current_row][i]->make_conflicted();
          m_tower[current_row - 1][i]->make_conflicted();
          conflicts = true;
        }
        if(this->conflict(m_tower[current_row][i], m_tower[current_row - 1][i - 1]))
        {
          m_tower[current_row][i]->make_conflicted();
          m_tower[current_row - 1][i - 1]->make_conflicted();
          conflicts = true;
        }
      }
    }

    if(winnings == 0) //full row of multiplers
      return 50 * current_row;
  }

  if(conflicts)
  {
    if(!m_savior_card) return 0;

    bool fixed = handle_conflicts(current_row);
    if(fixed)
    {
      this->clear_screen();
      this->announce_row(current_row);
      cout << endl << endl;
      return evaluate_winnings(current_row);
    }
    else
      return 0;
  }
  //else
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

bool Table::handle_conflicts(int current_row)
{
  cout << endl;
  this->print_tower();
  sleep(1);

  this->clear_screen();
  this->announce_handling_conflict();
  cout << endl << endl;
  this->print_tower();
  cout << endl;
  sleep(1);

  //HANDLE THE CONFLICT HERE

  //Savior Card had already been used in a previous row
  if(!this->m_savior_card)
    return false;

  //The Savior Card had not been used yet
  m_savior_card->reveal();

  this->clear_screen();
  this->announce_handling_conflict();
  cout << endl << endl;
  this->print_tower();
  cout << endl;
  sleep(1);

  for(unsigned long int i = 0; i < m_tower[current_row].size(); i++)
  {
    if(m_tower[current_row][i]->is_conflicted())
    {
      if(!m_savior_card)
        return false;

      delete m_tower[current_row][i];
      m_tower[current_row][i] = m_savior_card;
      m_savior_card = NULL;

      //check for new conflicts
      if(i == 0)  //far left card
      {
        if(this->conflict(m_tower[current_row][i], m_tower[current_row - 1][i]))
        {
          m_tower[current_row][i]->make_conflicted();
          return false;
        }
        else m_tower[current_row - 1][i]->fixed_conflict();
      }
      else if(i == m_tower[current_row].size() - 1)  //far right card
      {
        if(this->conflict(m_tower[current_row][i], m_tower[current_row - 1][i - 1]))
        {
          m_tower[current_row][i]->make_conflicted();
          return false;
        }
        else m_tower[current_row - 1][i - 1]->fixed_conflict();
      }
      else if(i < m_tower[current_row].size())  //cards in the middle - check both "parents"
      {
        if(this->conflict(m_tower[current_row][i], m_tower[current_row - 1][i]))
        {
          m_tower[current_row][i]->make_conflicted();
          m_tower[current_row - 1][i]->make_conflicted();
          return false;
        }
        else m_tower[current_row - 1][i]->fixed_conflict();

        if(this->conflict(m_tower[current_row][i], m_tower[current_row - 1][i - 1]))
        {
          m_tower[current_row][i]->make_conflicted();
          m_tower[current_row - 1][i - 1]->make_conflicted();
          return false;
        }
        else m_tower[current_row - 1][i - 1]->fixed_conflict();
      }

      this->clear_screen();
      this->announce_handling_conflict();
      cout << endl << endl;
      this->print_tower();
      cout << endl;
      sleep(1);
    }
  }

  return true;
}

//Only gets called if the savior card saved the row
int Table::evaluate_winnings(int current_row)
{
  int winnings = 0, multiplier = 1;
  for(unsigned long int i = 0; i < m_tower[current_row].size(); i++)
  {
    winnings += m_tower[current_row][i]->money_value();

    switch(m_tower[current_row][i]->get_face())
    {
      case XTWO:  multiplier *= 2;  break;
      case XFIVE: multiplier *= 5;  break;
      case XTEN:  multiplier *= 10; break;
      default:    break;
    }
  }

  if(winnings == 0) //full row of multiplers
    return 50 * current_row;

  return winnings * multiplier;
}

bool Table::continue_or_payout(int takehome_payment)
{
  string should_continue;
  do
  {
    cout << "Continue or payout $" << takehome_payment << "? (c/p)" << endl;
    cin >> should_continue;
  
    if(should_continue == "c" || should_continue == "C")
      return true;
    else if(should_continue == "p" || should_continue == "P")
      return false;
    else
      cout << endl << "That is an invalid option."  << endl << endl;

  }while(!(should_continue == "c" || should_continue == "C" ||
           should_continue == "p" || should_continue == "P"));

  return 420; //:D
}

void Table::display_winnings(int win_payment, int current_row)
{
  if(win_payment == 0)
  {
    this->announce_loss();
    cout << endl << endl;
    this->print_tower();
    cout << "LOST ON ROW " << current_row + 1 << endl;
  }
  else
  {
    this->announce_win();
    cout << endl << endl;
    this->print_tower();
    cout << endl;
    sleep(1);

    this->reveal_all_cards();

    this->clear_screen();
    this->announce_win();
    cout << endl << endl;
    this->print_tower();
    cout << "Payout of $" << win_payment << " from row " << current_row + 1 << endl;
  }

  return;
}

void Table::reveal_all_cards()
{
  for(unsigned long int i = 0; i < m_tower.size(); i++)
    for(unsigned long int j = 0; j < m_tower[i].size(); j++)
      m_tower[i][j]->reveal();

  if(m_savior_card) m_savior_card->reveal();

  return;
}

void Table::announce_bet(int bet)
{
  switch(bet)
  {
    case 15:
      cout << "  #######      #########  ############          ####     ##########"   << endl;
      cout << "  ###   ###    #########  ############         #####     ##########"   << endl;
      cout << "  ###    ###   ###            ###             ######     ##"           << endl;
      cout << "  ###   ###    ###            ###            ### ###     ## ######"    << endl;
      cout << "  #######      ######         ###           ###  ###     ###     ##"   << endl;
      cout << "  ###   ###    ######         ###                ###              ##"  << endl;
      cout << "  ###    ###   ###            ###                ###               ##" << endl;
      cout << "  ###     ###  ###            ###                ###     ##       ##"  << endl;
      cout << "  ###    ###   ########       ###           ############  ##     ##"   << endl;
      cout << "  ########     ########       ###           ############   #######"    << endl;
      break;

    case 20:
      cout << "  #######      #########  ############        ######        ######"     << endl;
      cout << "  ###   ###    #########  ############      ####  ####    ###    ###"   << endl;
      cout << "  ###    ###   ###            ###          ###      ###  ###      ###"  << endl;
      cout << "  ###   ###    ###            ###          ##       ### ###        ###" << endl;
      cout << "  #######      ######         ###                 ###   ###        ###" << endl;
      cout << "  ###   ###    ######         ###                ###    ###        ###" << endl;
      cout << "  ###    ###   ###            ###              ###      ###        ###" << endl;
      cout << "  ###     ###  ###            ###            ###         ###      ###"  << endl;
      cout << "  ###    ###   ########       ###           ###########   ###    ###"   << endl;
      cout << "  ########     ########       ###           ###########     ######"     << endl;
      break;

    case 25:
      cout << "  #######      #########  ############        ######     #########"   << endl;
      cout << "  ###   ###    #########  ############      ####  ####   #########"   << endl;
      cout << "  ###    ###   ###            ###          ###      ###  ##"          << endl;
      cout << "  ###   ###    ###            ###          ##       ###  ## #####"    << endl;
      cout << "  #######      ######         ###                 ###    ###    ##"   << endl;
      cout << "  ###   ###    ######         ###                ###             ##"  << endl;
      cout << "  ###    ###   ###            ###              ###                ##" << endl;
      cout << "  ###     ###  ###            ###            ###         ##      ##"  << endl;
      cout << "  ###    ###   ########       ###           ###########   ##    ##"   << endl;
      cout << "  ########     ########       ###           ###########    ######"    << endl;
      break;

    case 30:
      cout << "  #######      #########  ############         #####        ######"     << endl;
      cout << "  ###   ###    #########  ############       ###  ###     ###    ###"   << endl;
      cout << "  ###    ###   ###            ###           ###    ###   ###      ###"  << endl;
      cout << "  ###   ###    ###            ###                 ###   ###        ###" << endl;
      cout << "  #######      ######         ###               ###     ###        ###" << endl;
      cout << "  ###   ###    ######         ###               ####    ###        ###" << endl;
      cout << "  ###    ###   ###            ###                 ###   ###        ###" << endl;
      cout << "  ###     ###  ###            ###           ###    ###   ###      ###"  << endl;
      cout << "  ###    ###   ########       ###            ###  ###     ###    ###"   << endl;
      cout << "  ########     ########       ###              #####        ######"     << endl;
      break;

    case 40:
      cout << "  #######      #########  ############           ####      ######"     << endl;
      cout << "  ###   ###    #########  ############          #####    ###    ###"   << endl;
      cout << "  ###    ###   ###            ###              ######   ###      ###"  << endl;
      cout << "  ###   ###    ###            ###             ### ###  ###        ###" << endl;
      cout << "  #######      ######         ###            ###  ###  ###        ###" << endl;
      cout << "  ###   ###    ######         ###           ###   ###  ###        ###" << endl;
      cout << "  ###    ###   ###            ###          ##########  ###        ###" << endl;
      cout << "  ###     ###  ###            ###          ##########   ###      ###"  << endl;
      cout << "  ###    ###   ########       ###                 ###    ###    ###"   << endl;
      cout << "  ########     ########       ###                 ###      ######"     << endl;
      break;

    case 45:
      cout << "  #######      #########  ############           ####  ##########"   << endl;
      cout << "  ###   ###    #########  ############          #####  ##########"   << endl;
      cout << "  ###    ###   ###            ###              ######  ##"           << endl;
      cout << "  ###   ###    ###            ###             ### ###  ## ######"    << endl;
      cout << "  #######      ######         ###            ###  ###  ###     ##"   << endl;
      cout << "  ###   ###    ######         ###           ###   ###           ##"  << endl;
      cout << "  ###    ###   ###            ###          ##########            ##" << endl;
      cout << "  ###     ###  ###            ###          ##########  ##       ##"  << endl;
      cout << "  ###    ###   ########       ###                 ###   ##     ##"   << endl;
      cout << "  ########     ########       ###                 ###    #######"    << endl;
      break;

    case 50:
      cout << "  #######      #########  ############     ##########       ######"     << endl;
      cout << "  ###   ###    #########  ############     ##########     ###    ###"   << endl;
      cout << "  ###    ###   ###            ###          ##            ###      ###"  << endl;
      cout << "  ###   ###    ###            ###          ## ######    ###        ###" << endl;
      cout << "  #######      ######         ###          ###     ##   ###        ###" << endl;
      cout << "  ###   ###    ######         ###                   ##  ###        ###" << endl;
      cout << "  ###    ###   ###            ###                    ## ###        ###" << endl;
      cout << "  ###     ###  ###            ###          ##       ##   ###      ###"  << endl;
      cout << "  ###    ###   ########       ###           ##     ##     ###    ###"   << endl;
      cout << "  ########     ########       ###            #######        ######"     << endl;
      break;

    case 60:
      cout << "  #######      #########  ############           ###        ######"     << endl;
      cout << "  ###   ###    #########  ############          ###       ###    ###"   << endl;
      cout << "  ###    ###   ###            ###              ###       ###      ###"  << endl;
      cout << "  ###   ###    ###            ###             ###       ###        ###" << endl;
      cout << "  #######      ######         ###            ###        ###        ###" << endl;
      cout << "  ###   ###    ######         ###           #########   ###        ###" << endl;
      cout << "  ###    ###   ###            ###          ###     ###  ###        ###" << endl;
      cout << "  ###     ###  ###            ###          ###      ###  ###      ###"  << endl;
      cout << "  ###    ###   ########       ###           ###    ###    ###    ###"   << endl;
      cout << "  ########     ########       ###             ######        ######"     << endl;
      break;

    case 75:
      cout << "  #######      #########  ############      ###########  ##########"   << endl;
      cout << "  ###   ###    #########  ############      ###########  ##########"   << endl;
      cout << "  ###    ###   ###            ###                  ###   ##"           << endl;
      cout << "  ###   ###    ###            ###                 ###    ## ######"    << endl;
      cout << "  #######      ######         ###                 ###    ###     ##"   << endl;
      cout << "  ###   ###    ######         ###                ###              ##"  << endl;
      cout << "  ###    ###   ###            ###                ###               ##" << endl;
      cout << "  ###     ###  ###            ###               ###      ##       ##"  << endl;
      cout << "  ###    ###   ########       ###              ###        ##     ##"   << endl;
      cout << "  ########     ########       ###              ###         #######"    << endl;
      break;

    case 80:
      cout << "  #######      #########  ############          ####          ######"     << endl;
      cout << "  ###   ###    #########  ############        ###  ###      ###    ###"   << endl;
      cout << "  ###    ###   ###            ###            ###    ###    ###      ###"  << endl;
      cout << "  ###   ###    ###            ###             ###  ###    ###        ###" << endl;
      cout << "  #######      ######         ###               ####      ###        ###" << endl;
      cout << "  ###   ###    ######         ###             ###  ###    ###        ###" << endl;
      cout << "  ###    ###   ###            ###            ###    ###   ###        ###" << endl;
      cout << "  ###     ###  ###            ###           ###      ###   ###      ###"  << endl;
      cout << "  ###    ###   ########       ###            ###    ###     ###    ###"   << endl;
      cout << "  ########     ########       ###              ######         ######"     << endl;
      break;

    case 100:
      cout << "  #######      #########  ############          ####        ######          ######"     << endl;
      cout << "  ###   ###    #########  ############         #####      ###    ###      ###    ###"   << endl;
      cout << "  ###    ###   ###            ###             ######     ###      ###    ###      ###"  << endl;
      cout << "  ###   ###    ###            ###            ### ###    ###        ###  ###        ###" << endl;
      cout << "  #######      ######         ###           ###  ###    ###        ###  ###        ###" << endl;
      cout << "  ###   ###    ######         ###                ###    ###        ###  ###        ###" << endl;
      cout << "  ###    ###   ###            ###                ###    ###        ###  ###        ###" << endl;
      cout << "  ###     ###  ###            ###                ###     ###      ###    ###      ###"  << endl;
      cout << "  ###    ###   ########       ###           ############  ###    ###      ###    ###"   << endl;
      cout << "  ########     ########       ###           ############    ######          ######"     << endl;
      break;

    case 125:
      cout << "  #######      #########  ############           ####        ######    #########"   << endl;
      cout << "  ###   ###    #########  ############          #####     ####  ####   #########"   << endl;
      cout << "  ###    ###   ###            ###              ######    ###      ###  ##"          << endl;
      cout << "  ###   ###    ###            ###             ### ###    ##       ###  ## #####"    << endl;
      cout << "  #######      ######         ###            ###  ###           ###    ###    ##"   << endl;
      cout << "  ###   ###    ######         ###                 ###          ###             ##"  << endl;
      cout << "  ###    ###   ###            ###                 ###        ###                ##" << endl;
      cout << "  ###     ###  ###            ###                 ###      ###         ##      ##"  << endl;
      cout << "  ###    ###   ########       ###            ###########  ###########   ##    ##"   << endl;
      cout << "  ########     ########       ###            ###########  ###########    ######"    << endl;
      break;
  }

  return;
}

void Table::announce_row(int current_row)
{
  switch(current_row)
  {
    case 0: //row 1
      cout << "  ######         ######     ###                     ###         ####"     << endl;
      cout << "  ###  ###     ####  ####   ###                     ###        #####"     << endl;
      cout << "  ##    ###   ###      ###   ###        ###        ###        ######"     << endl;
      cout << "  ##    ###  ###        ###   ###      #####      ###        ### ###"     << endl;
      cout << "  ##   ###   ###        ###   ###     ### ###     ###       ###  ###"     << endl;
      cout << "  ######     ###        ###    ###   ###   ###   ###             ###"     << endl;
      cout << "  ######     ###        ###    ###   ###   ###   ###             ###"     << endl;
      cout << "  ##  ###     ###      ###      ### ###     ### ###              ###"     << endl;
      cout << "  ##   ###     ####  ####       ### ###     ### ###         ############" << endl;
      cout << "  ##    ###      ######          #####       #####          ############" << endl;
      break;

    case 1: //row 2
      cout << "  ######         ######     ###                     ###        ######"    << endl;
      cout << "  ###  ###     ####  ####   ###                     ###      ####  ####"  << endl;
      cout << "  ##    ###   ###      ###   ###        ###        ###      ###      ###" << endl;
      cout << "  ##    ###  ###        ###   ###      #####      ###       ##       ###" << endl;
      cout << "  ##   ###   ###        ###   ###     ### ###     ###              ###"   << endl;
      cout << "  ######     ###        ###    ###   ###   ###   ###             ###"     << endl;
      cout << "  ######     ###        ###    ###   ###   ###   ###           ###"       << endl;
      cout << "  ##  ###     ###      ###      ### ###     ### ###          ###"         << endl;
      cout << "  ##   ###     ####  ####       ### ###     ### ###         ############" << endl;
      cout << "  ##    ###      ######          #####       #####          ############" << endl;
      break;

    case 2: //row 3
      cout << "  ######         ######     ###                     ###        #####"   << endl;
      cout << "  ###  ###     ####  ####   ###                     ###      ###  ###"  << endl;
      cout << "  ##    ###   ###      ###   ###        ###        ###      ###    ###" << endl;
      cout << "  ##    ###  ###        ###   ###      #####      ###             ###"  << endl;
      cout << "  ##   ###   ###        ###   ###     ### ###     ###           ###"    << endl;
      cout << "  ######     ###        ###    ###   ###   ###   ###            ####"   << endl;
      cout << "  ######     ###        ###    ###   ###   ###   ###              ###"  << endl;
      cout << "  ##  ###     ###      ###      ### ###     ### ###         ###    ###" << endl;
      cout << "  ##   ###     ####  ####       ### ###     ### ###          ###  ###"  << endl;
      cout << "  ##    ###      ######          #####       #####             #####"   << endl;
      break;

    case 3: //row 4
      cout << "  ######         ######     ###                     ###          ###" << endl;
      cout << "  ###  ###     ####  ####   ###                     ###        #####" << endl;
      cout << "  ##    ###   ###      ###   ###        ###        ###        ######" << endl;
      cout << "  ##    ###  ###        ###   ###      #####      ###        ### ###" << endl;
      cout << "  ##   ###   ###        ###   ###     ### ###     ###       ###  ###" << endl;
      cout << "  ######     ###        ###    ###   ###   ###   ###       ###   ###" << endl;
      cout << "  ######     ###        ###    ###   ###   ###   ###      ##########" << endl;
      cout << "  ##  ###     ###      ###      ### ###     ### ###       ##########" << endl;
      cout << "  ##   ###     ####  ####       ### ###     ### ###              ###" << endl;
      cout << "  ##    ###      ######          #####       #####               ###" << endl;
      break;

    case 4: //row 5
      cout << "  ######         ######     ###                     ###    ##########"   << endl;
      cout << "  ###  ###     ####  ####   ###                     ###    ##########"   << endl;
      cout << "  ##    ###   ###      ###   ###        ###        ###     ##"           << endl;
      cout << "  ##    ###  ###        ###   ###      #####      ###      ## ######"    << endl;
      cout << "  ##   ###   ###        ###   ###     ### ###     ###      ###     ##"   << endl;
      cout << "  ######     ###        ###    ###   ###   ###   ###                ##"  << endl;
      cout << "  ######     ###        ###    ###   ###   ###   ###                 ##" << endl;
      cout << "  ##  ###     ###      ###      ### ###     ### ###        ##       ##"  << endl;
      cout << "  ##   ###     ####  ####       ### ###     ### ###         ##     ##"   << endl;
      cout << "  ##    ###      ######          #####       #####           #######"    << endl;
      break;

    case 5: //row 6
      cout << "  ######         ######     ###                     ###         ###"    << endl;
      cout << "  ###  ###     ####  ####   ###                     ###        ###"     << endl;
      cout << "  ##    ###   ###      ###   ###        ###        ###        ###"      << endl;
      cout << "  ##    ###  ###        ###   ###      #####      ###        ###"       << endl;
      cout << "  ##   ###   ###        ###   ###     ### ###     ###       ###"        << endl;
      cout << "  ######     ###        ###    ###   ###   ###   ###       #########"   << endl;
      cout << "  ######     ###        ###    ###   ###   ###   ###      ###     ###"  << endl;
      cout << "  ##  ###     ###      ###      ### ###     ### ###       ###      ###" << endl;
      cout << "  ##   ###     ####  ####       ### ###     ### ###        ###    ###"  << endl;
      cout << "  ##    ###      ######          #####       #####           ######"    << endl;
      break;

    case 6: //row 7
      cout << "  ######         ######     ###                     ###    ##########" << endl;
      cout << "  ###  ###     ####  ####   ###                     ###    ##########" << endl;
      cout << "  ##    ###   ###      ###   ###        ###        ###           ###"  << endl;
      cout << "  ##    ###  ###        ###   ###      #####      ###           ###"   << endl;
      cout << "  ##   ###   ###        ###   ###     ### ###     ###           ###"   << endl;
      cout << "  ######     ###        ###    ###   ###   ###   ###           ###"    << endl;
      cout << "  ######     ###        ###    ###   ###   ###   ###           ###"    << endl;
      cout << "  ##  ###     ###      ###      ### ###     ### ###           ###"     << endl;
      cout << "  ##   ###     ####  ####       ### ###     ### ###          ###"      << endl;
      cout << "  ##    ###      ######          #####       #####           ###"      << endl;
      break;
  }

  return;
}

void Table::announce_win()
{
  cout << "  ###                     ###  ###  #####      ###" << endl;
  cout << "  ###                     ###  ###  ### ##     ###" << endl;
  cout << "   ###        ###        ###   ###  ###  ##    ###" << endl;
  cout << "    ###      ######     ###    ###  ###  ##    ###" << endl;
  cout << "    ###     ### ###     ###    ###  ###   ##   ###" << endl;
  cout << "     ###   ###   ###   ###     ###  ###   ##   ###" << endl;
  cout << "     ###   ###   ###   ###     ###  ###    ##  ###" << endl;
  cout << "      ### ###     ### ###      ###  ###    ##  ###" << endl;
  cout << "      ### ###     ### ###      ###  ###     ## ###" << endl;
  cout << "       #####       #####       ###  ###     ######" << endl;

  return;
}

void Table::announce_loss()
{
  cout << "  ###           ########        #########      ########"    << endl;
  cout << "  ###         ###      ###     ###     ###    ###    ###"   << endl;
  cout << "  ###        ###        ###   ###        ##  ###       ##"  << endl;
  cout << "  ###       ###          ###   ###            ###"          << endl;
  cout << "  ###       ###          ###     ###            ###"        << endl;
  cout << "  ###       ###          ###        ###            ###"     << endl;
  cout << "  ###       ###          ###           ###            ###"  << endl;
  cout << "  ###        ###        ###   ##        ###  ##        ###" << endl;
  cout << "  #########   ###      ###     ###      ###   ###      ###" << endl;
  cout << "  #########     ########         ########       ########"   << endl;

  return;
}

void Table::announce_handling_conflict()
{
  cout << "      ######          ########      #####      ###  ##########  ###       ###       ######    #############" << endl;
  cout << "    ###    ###      ###      ###    ### ##     ###  ##########  ###       ###     ###    ###  #############" << endl;
  cout << "   ###      ###    ###        ###   ###  ##    ###  ###         ###       ###    ###      ###      ###"      << endl;
  cout << "  ###             ###          ###  ###  ##    ###  ###         ###       ###   ###                ###"      << endl;
  cout << "  ###             ###          ###  ###   ##   ###  ######      ###       ###   ###                ###"      << endl;
  cout << "  ###             ###          ###  ###   ##   ###  ######      ###       ###   ###                ###"      << endl;
  cout << "  ###             ###          ###  ###    ##  ###  ###         ###       ###   ###                ###"      << endl;
  cout << "   ###      ###    ###        ###   ###    ##  ###  ###         ###       ###    ###      ###      ###"      << endl;
  cout << "    ###    ###      ###      ###    ###     ## ###  ###         ########  ###     ###    ###       ###"      << endl;
  cout << "      ######          ########      ###     ######  ###         ########  ###       ######         ###"      << endl;

  return;
}
