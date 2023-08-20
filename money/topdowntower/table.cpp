#include "table.h"

Table::Table()
{
  m_TOWER_SIZE = m_tower.size();
  m_savior_card = NULL;
  m_deck = NULL;

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
      cout << " ";
      if(m_tower[i][j]) cout << m_tower[i][j]->display_value();
      else cout << " X ";
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

void Table::get_full_odds_breakdown()
{
  m_deck = new Deck(); //new deck must be deleted
  m_deck->reveal_all_for_odds();

  //check for progress file and update progress

  m_total_possibilities       = new BigInt(0);
  for(long unsigned int i = 0; i < m_tower.size(); i++)
  {
    m_winning_sum_by_row.push_back(new BigInt(0));
    m_max_row_before_loss.push_back(new BigInt(0));
  }
  m_possible_jackpots         = new BigInt(0);
  m_jackpots_payouts_sum      = new BigInt(0);
  m_possible_mega_jackpots    = new BigInt(0);
  m_mega_jackpots_payouts_sum = new BigInt(0);

  for(CardFace cardface = ONE; cardface != LAST; cardface = CardFace(cardface + 1))
  {
    this->get_odds(0, 0, cardface, 0, 0, 0);

    cout << "Completed m_tower[0][0] of " << cardface << endl;
    cout << "Total possibilities of " << m_total_possibilities->str_value() << " (cumulative)" << endl << endl;

  }
  cout << "DONE" << endl;

  //print odds by row

  delete m_deck;
  return;
}

void Table::get_odds(long unsigned int current_row,
                     long unsigned int current_spot_in_row,
                     CardFace          current_cardface,
                     long unsigned int first_conflict_row,
                     long unsigned int first_conflict_index,
                     long unsigned int second_conflict_row)
{
  //Called on a card within the tower
  // - we still need to deal more cards
  if(current_row < m_TOWER_SIZE)
  {
    Card* current_card = m_deck->deal_card_by_face(current_cardface);

    m_tower[current_row][current_spot_in_row] = current_card;

    //Did this new card cause a conflict? Take note of each conflict row
    //We can skip the checks if we have already found 2 conflicts in the tower
    if(second_conflict_row == 0)
    {
      if(current_row > 0)
      {
        if(current_spot_in_row == 0)
        {
          if(conflict(current_card, m_tower[current_row - 1][current_spot_in_row]))
          {
            if(!first_conflict_row)
            {
              first_conflict_row  = current_row;
              first_conflict_index = current_spot_in_row;
            }
            else
            {
              second_conflict_row = current_row;
            }
          }
        }
        else if(current_spot_in_row == m_tower[current_row].size() - 1)
        {
          if(conflict(current_card, m_tower[current_row - 1][current_spot_in_row - 1]))
          {
            if(!first_conflict_row)
            {
              first_conflict_row  = current_row;
              first_conflict_index = current_spot_in_row;
            }
            else
            {
              second_conflict_row = current_row;
            }
          }
        }
        else
        {
          if(conflict(current_card, m_tower[current_row - 1][current_spot_in_row - 1]))
          {
            if(!first_conflict_row)
            {
              first_conflict_row  = current_row;
              first_conflict_index = current_spot_in_row;
            }
            else
            {
              second_conflict_row = current_row;
            }
          }
          else if(conflict(current_card, m_tower[current_row - 1][current_spot_in_row]))
          {
            if(!first_conflict_row)
            {
              first_conflict_row  = current_row;
              first_conflict_index = current_spot_in_row;
            }
            else
            {
              second_conflict_row = current_row;
            }
          }
        }
      }
    }

    //Make next iteration variables
    long unsigned int next_spot_in_row = current_spot_in_row + 1;
    long unsigned int next_row = current_row;
    if(next_spot_in_row == m_tower[current_row].size()) //row is full and we need to go to the next row
    {
      next_row++;
      next_spot_in_row = 0;
    }

    //Go through every possible next card
    CardFace next_cardface = CardFace::ONE;
    do
    {
      //If there are no more of this card in the deck
      //Pick the next available card
      if(!m_deck->get_card_count(next_cardface))
      {
        do
        {
          next_cardface = CardFace(next_cardface + 1);
          if(next_cardface == CardFace::LAST) break;
        }while(!m_deck->get_card_count(next_cardface));

        if(next_cardface == CardFace::LAST) break;
      }

      //vvvv THE MEAT vvvv

      get_odds(next_row,
               next_spot_in_row,
               next_cardface,
               first_conflict_row,
               first_conflict_index,
               second_conflict_row);

      //^^^^ THE MEAT ^^^^

      next_cardface = CardFace(next_cardface + 1);

    }while(next_cardface != CardFace::LAST);

    //Progress updates shows the stats after the current last-shown-card has been exhausted
    if(current_row < (m_TOWER_SIZE - 1))
    {
      this->print_tower();

      cout << "Total possibilities: ";
      m_total_possibilities->print();
      cout << endl;

      cout << "First conflict row: " << first_conflict_row << endl;
      cout << "First conflict index: " << first_conflict_index << endl;
      cout << "Second conflict row: " << second_conflict_row << endl;

      for(long unsigned int i = 0; i < m_TOWER_SIZE; i++)
      {
        cout << "Row " << i << " highest_row_before_loss: ";
        m_max_row_before_loss[i]->print();
        cout << endl;
        cout << "Row " << i << " cumulative payouts:      ";
        m_winning_sum_by_row[i]->print();

        cout << endl;
      }

      cout << "Total Jackpot possibilities:      ";
      m_possible_jackpots->print();
      cout << endl;

      cout << "Total Jackpot payouts:            ";
      m_jackpots_payouts_sum->print();
      cout << endl;

      cout << "Total Mega-Jackpot possibilities: ";
      m_possible_mega_jackpots->print();
      cout << endl;

      cout << "Total Mega-Jackpot payouts:       ";
      m_mega_jackpots_payouts_sum->print();
      cout << endl;

      cout << endl;
    }

    //Put the current card back into the deck
    m_deck->put_card_back(current_card);
    m_tower[current_row][current_spot_in_row] = NULL;

    return;
  }

  //It is the savior card
  //- Solve the conflict
  //- Then do stats updates
  else
  {
    m_savior_card = m_deck->deal_card_by_face(current_cardface);

    if(second_conflict_row == 0) //JACKPOT OPPORTUNITY
    {
      if(first_conflict_row == 0) //MEGA JACKPOT OPPORTUNITY!!!
      {
        m_possible_mega_jackpots->add(1);
        int payout = MEGA_JACKPOT_MULTIPLIER * evaluate_winnings(m_TOWER_SIZE);
        m_mega_jackpots_payouts_sum->add(payout);

      }
      else
      {
        m_possible_jackpots->add(1);
        int payout = JACKPOT_MULTIPLIER * evaluate_winnings(m_TOWER_SIZE);
        m_jackpots_payouts_sum->add(payout);
      }

      //Update the stats for the rest of the rows
      for(long unsigned int i = 0; i < m_TOWER_SIZE; i++)
      {
        m_winning_sum_by_row[i]->add(this->evaluate_winnings(i));
      }
    }
    else
    {
      //Update winning stats for all rows before the second conflict row
      for(long unsigned int i = 0; i < second_conflict_row; i++)
      {
        if(i < first_conflict_row)
        {
          m_winning_sum_by_row[i]->add(this->evaluate_winnings(i));
          continue;
        }

        if(i == first_conflict_row)
        {
          //Handle conflict
	  // - NOTE: We can only get here if(first_conflict_row < second_conflict_row)
          //         (there is only one conflict on the first_conflict_row)
          // - changing out the first conflict card resolves the conflict
          if(handle_conflict_for_odds(first_conflict_row, first_conflict_index))
          {
            m_winning_sum_by_row[i]->add(this->evaluate_winnings(i));

            //Need to check if the new card created a conflict in the next row

            continue;
          }

          // - changing out the savior card does not resolve the conflict - loss on current row
          else
          {
            m_max_row_before_loss[i - 1]->add(1);
            break;
          }
        }

        //first conflict row resolved and we are between first and second conflict row
        //
        //if(i > first_conflict_row)
        m_winning_sum_by_row[i]->add(this->evaluate_winnings(i));
      }

      m_max_row_before_loss[second_conflict_row - 1]->add(1);
    }

    m_total_possibilities->add(1);

    //put the card back into the deck
    m_deck->put_card_back(m_savior_card);
    m_savior_card = NULL;

    return;
  }
}

bool Table::handle_conflict_for_odds(long unsigned int first_conflict_row,
                                     long unsigned int first_conflict_index)
{
  //This will only get called if the first and second conflicts are on different rows
  return true;
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
