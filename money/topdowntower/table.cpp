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
  //Print Tower
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
      else              cout << " X ";
    }
    cout << endl;
  }

  //Print Savior Card
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

void Table::play_game(Player* player, int base_bet)
{
  bool again;
  do
  {
    this->clear_screen();

    this->play_round(player, base_bet);

    bool valid = false;
    if(player->can_afford(base_bet))
    {
      do
      {
        string play_again;
        cout << "Do you want to play again? (y/n)" << endl;
        cin >> play_again;

        if     (play_again == "y" || play_again == "Y") {valid = true; again = true;}
        else if(play_again == "n" || play_again == "N") {valid = true; again = false;}
        else cout << endl << "Valid options are \"Y\" or \"N\"." << endl << endl;
      }while(!valid);
    }
    else
    {
      cout << endl << "You do not have enough money to play again";
      break;
    }
  }while(again);

  return;
}

void Table::play_round(Player* player, int base_bet)
{
  m_deck = new Deck(); //new deck must be deleted
  m_deck->reseed();

  int round_multiplier = this->get_bet(player, base_bet);

  m_deck->shuffle();
  this->deal();        //cards move from deck to tower -> must delete them

  this->clear_screen();
  this->print_tower();
  cout << endl;

  int win_payment = 0;
  long unsigned int current_row = 0;
  do
  {
    sleep(1);

    this->clear_screen();

    this->announce_row(current_row);
    cout << endl << endl;

    int row_payment = this->play_row_for_game(current_row);

    this->print_tower();

    win_payment = row_payment * round_multiplier;

    if(row_payment == 0) //LOSS
    {
      cout << endl;
      sleep(1);
      break;
    }
    else
    {
      if(current_row < m_tower.size() - 1)
      {
        if(this->continue_or_payout(win_payment))
        {
          current_row++;
        }
        else
        {
          sleep(1);
          break;
        }
      }
      else //JACKPOT!
      {
        sleep(1);
        break;
      }
    }
  }while(1);

  this->clear_screen();

  display_winnings(win_payment, current_row, (base_bet * round_multiplier));

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

  do
  {
    string round_multiplier_str;
    int round_multiplier;
    bool valid = false;
    do
    {
      cout << "Your bet will be in multiples of $" << base_bet << endl;
      cout << "How many multiples? (1-5) " << endl;
      cin >> round_multiplier_str;

      if     (round_multiplier_str == "1") {valid = true; round_multiplier = 1;}
      else if(round_multiplier_str == "2") {valid = true; round_multiplier = 2;}
      else if(round_multiplier_str == "3") {valid = true; round_multiplier = 3;}
      else if(round_multiplier_str == "4") {valid = true; round_multiplier = 4;}
      else if(round_multiplier_str == "5") {valid = true; round_multiplier = 5;}
      else cout << endl << "Valid entries are 1 - 5" << endl << endl;
    }while(!valid);

    int bet = round_multiplier * base_bet;

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
int Table::play_row_for_game(long unsigned int current_row)
{
  bool conflicts = false;
  int winnings = 0, multiplier = 1;

  if(current_row == 0)
  {
    m_tower[0][0]->reveal();

    //special things for wilds and multipliers on the first card
    switch(m_tower[0][0]->get_face())
    {
      case WILD:  return 15;
      case XTWO:  return 15;
      case XFIVE: return 20;
      case XTEN:  return 25;
      default:    return m_tower[0][0]->money_value();
    }
  }

  //vvvvvvvvvvvv  THE MEAT vvvvvvvvvvvvvvv 
  //else
  //{
    for(unsigned long int i = 0; i < m_tower[current_row].size(); i++)
    {
      Card* current_card = m_tower[current_row][i];

      current_card->reveal();

      winnings += current_card->money_value();

      switch(current_card->get_face())
      {
        case XTWO:  multiplier *= 2;  break;
        case XFIVE: multiplier *= 5;  break;
        case XTEN:  multiplier *= 10; break;
        default:    break;
      }

      if(i == 0)                                     //far left card
      {
        if(this->conflict(current_card, m_tower[current_row - 1][i]))
        {
          current_card->make_conflicted();
          m_tower[current_row - 1][i]->make_conflicted();
          conflicts = true;
        }
      }
      else if(i == m_tower[current_row].size() - 1)  //far right card
      {
        if(this->conflict(current_card, m_tower[current_row - 1][i - 1]))
        {
          current_card->make_conflicted();
          m_tower[current_row - 1][i - 1]->make_conflicted();
          conflicts = true;
        }
      }
      else                                           //cards in the middle - check both "parents"
      {
        if(this->conflict(current_card, m_tower[current_row - 1][i]))
        {
          current_card->make_conflicted();
          m_tower[current_row - 1][i]->make_conflicted();
          conflicts = true;
        }
        if(this->conflict(current_card, m_tower[current_row - 1][i - 1]))
        {
          current_card->make_conflicted();
          m_tower[current_row - 1][i - 1]->make_conflicted();
          conflicts = true;
        }
      }
    }

    if(!conflicts)
      return winnings_calc(winnings, multiplier, current_row);
    else
    {
      if(!m_savior_card)
        return 0;

      bool fixed = handle_conflicts(current_row);
      this->clear_screen();

      if(fixed)
      {
        this->announce_row(current_row);
        cout << endl << endl;
        return evaluate_winnings(current_row);
      }
      else
        return 0;
    }
  //}
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

bool Table::handle_conflicts(long unsigned int current_row)
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

  if(!m_savior_card) return false;
  else
  {
    m_savior_card->reveal();

    this->clear_screen();
    this->announce_handling_conflict();
    cout << endl << endl;
    this->print_tower();
    cout << endl;
    sleep(1);

    for(unsigned long int i = 0; i < m_tower[current_row].size(); i++)
    {
      Card* current_card = m_tower[current_row][i];
      if(current_card->is_conflicted())
      {
        if(!m_savior_card)
          return false;

        delete current_card;
        m_tower[current_row][i] = m_savior_card;
        current_card = m_savior_card;
        m_savior_card = NULL;

        bool still_conflicted = false;
        //check for new conflicts
        if(i == 0)                                     //far left card
        {
          if(this->conflict(current_card, m_tower[current_row - 1][i]))
          {
            current_card->make_conflicted();
            m_tower[current_row - 1][i]->make_conflicted();
            still_conflicted = true;
          }
          else
          {
            if(!this->conflict(m_tower[current_row][i + 1], m_tower[current_row - 1][i]))
              m_tower[current_row - 1][i]->fixed_conflict();
          }
        }
        else if(i == m_tower[current_row].size() - 1)  //far right card
        {
          if(this->conflict(current_card, m_tower[current_row - 1][i - 1]))
          {
            current_card->make_conflicted();
            m_tower[current_row - 1][i - 1]->make_conflicted();
            still_conflicted = true;
          }
          else
          {
            if(!this->conflict(m_tower[current_row][i - 1], m_tower[current_row - 1][i - 1]))
              m_tower[current_row - 1][i - 1]->fixed_conflict();
          }
        }
        else                                           //cards in the middle - check both "parents"
        {
          //Right Parent
          if(this->conflict(current_card, m_tower[current_row - 1][i]))
          {
            current_card->make_conflicted();
            m_tower[current_row - 1][i]->make_conflicted();
            still_conflicted = true;
          }
          else
          {
            if(!this->conflict(m_tower[current_row][i + 1], m_tower[current_row - 1][i]))
              m_tower[current_row - 1][i]->fixed_conflict();
          }

          //Left Parent
          if(this->conflict(m_tower[current_row][i], m_tower[current_row - 1][i - 1]))
          {
            m_tower[current_row][i]->make_conflicted();
            m_tower[current_row - 1][i - 1]->make_conflicted();
            still_conflicted = true;
          }
          else
          {
            if(!this->conflict(m_tower[current_row][i - 1], m_tower[current_row - 1][i - 1]))
              m_tower[current_row - 1][i - 1]->fixed_conflict();
          }
        }

        this->clear_screen();
        this->announce_handling_conflict();
        cout << endl << endl;
        this->print_tower();
        cout << endl;
        sleep(1);

        if(still_conflicted) return false;
      }
    }

    return true;
  }
}

int Table::evaluate_winnings(long unsigned int current_row)
{
  int winnings = 0, multiplier = 1;
  for(unsigned long int i = 0; i < m_tower[current_row].size(); i++)
  {
    Card* current_card = m_tower[current_row][i];
    int card_winnings = current_card->money_value();

    if(card_winnings)
      winnings += card_winnings;
    else //Its a multiplier of some kind
    {
      CardFace cardface = current_card->get_face();
      if(cardface == XTWO)  {multiplier *=  2; continue;}
      if(cardface == XFIVE) {multiplier *=  5; continue;}
      if(cardface == XTEN)  {multiplier *= 10; continue;}
    }
  }

  return winnings_calc(winnings, multiplier, current_row);
}

int Table::winnings_calc(int winnings, int multiplier, long unsigned int current_row)
{
  if(winnings == 0) //full row of multiplers
    return (current_row + 1) * multiplier;

  if(current_row == m_tower.size() - 4)          //4th-to-Last Row
    winnings = winnings + (winnings / 10);       // +10%

  if(current_row == m_tower.size() - 3)          //3rd-to-Last Row
    winnings = winnings + (winnings / 4);        // +25%

  if(current_row == m_tower.size() - 2)          //2nd-to-Last Row
    winnings = winnings + ((winnings * 3) / 4);  // +75%

  if(current_row == m_tower.size() - 1)          //Last Row
  {
     if(!m_savior_card)                          ////JACKPOT
       winnings = winnings * 3;                  //// 300%
     else                                        ////MEGA JACKPOT!!
       winnings = winnings * 7;                  //// 700%
  }

  return winnings * multiplier;
}

//Return true for continue
//Return false for payout
bool Table::continue_or_payout(int takehome_payment)
{
  string should_continue;
  do
  {
    cout << "Continue or payout $" << takehome_payment << "? (c/p)" << endl;
    cin >> should_continue;
  
    if     (should_continue == "c" || should_continue == "C") return true;
    else if(should_continue == "p" || should_continue == "P") return false;
    else cout << endl << "Valid entries are \"C\" and \"P\""  << endl << endl;

  }while(1);

  return 420; //:D
}

void Table::display_winnings(int win_payment, long unsigned int current_row, int bet)
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
    if(current_row < m_tower.size() - 1)
    {
      if(win_payment >= (bet * 2))
        this->announce_big_win();
      else
        this->announce_win();
      cout << endl << endl;
      this->print_tower();
      cout << endl;
      sleep(1);

      this->reveal_all_cards();

      this->clear_screen();
      if(win_payment >= (bet * 2))
        this->announce_big_win();
      else
        this->announce_win();
      cout << endl << endl;
      this->print_tower();
      cout << "Payout of $" << win_payment << " from row " << current_row + 1 << endl;
    }
    else //if(current_row == m_tower.size() - 1)
    {
      if(!m_savior_card) this->announce_jackpot();
      else               this->announce_mega_jackpot();
      cout << endl << endl;
      this->print_tower();
      cout << "JACKPOT of $" << win_payment << "!!!" << endl;
    }
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


void Table::get_odds()
{
  string r_or_bf;
  bool random_odds;
  do
  {
    cout << "Random (r) or Brute Force (bf)? ";
    cin >> r_or_bf;
    if(r_or_bf == "r")  random_odds = true;
    if(r_or_bf == "bf") random_odds = false;
  }while(!(r_or_bf == "r" || r_or_bf == "bf"));

  m_total_possibilities       = new BigInt(0);
  for(long unsigned int i = 0; i < m_tower.size(); i++)
  {
    m_winning_sum_by_row.push_back(new BigInt(0));
    m_max_row_before_loss.push_back(new BigInt(0));
    m_num_losses_by_row.push_back(new BigInt(0));
    m_num_wins_by_row.push_back(new BigInt(0));
    m_highest_payout_by_row.push_back(0);
  }
  m_highest_payout_per_deal_sum = new BigInt(0);
  m_possible_jackpots           = new BigInt(0);
  m_jackpots_payouts_sum        = new BigInt(0);
  m_highest_jackpot_payout      = 0;
  m_possible_mega_jackpots      = new BigInt(0);
  m_mega_jackpots_payouts_sum   = new BigInt(0);
  m_highest_mega_jackpot_payout = 0;

  if(random_odds)
  {
    this->random_odds();
  }
  else
  {
    m_deck = new Deck(); //new deck must be deleted
    m_deck->reveal_all_for_odds();

    //check for progress file and update progress

    for(CardFace cardface = ONE; cardface != LAST; cardface = CardFace(cardface + 1))
    {
      this->bf_odds(0, 0, cardface, 0, 0, 0);

      cout << "Completed m_tower[0][0] of " << cardface << endl;
      cout << "Total possibilities of " << m_total_possibilities->str_value() << " (cumulative)" << endl << endl;
    }
    cout << "DONE" << endl;

    //print odds by row
    delete m_deck;
  }

  cout << endl << "DONE" << endl;
  return;
}

void Table::random_odds()
{
  m_deck = new Deck(); //new deck must be deleted
  m_deck->reseed();

  BigInt* sample_size = new BigInt(string("1000000"));
  int     base_bet = 15;

  cout << "Going to run ";
  sample_size->print();
  cout << " itterations and get the stats" << endl << endl;

  BigInt* i = new BigInt(0);
  for(; i->less_than(sample_size); i->add(1))
  {
    m_deck->shuffle();
    this->deal();

    int highest_payout_of_deal = 0;
    for(long unsigned int j = 0; j < m_tower.size(); j++)
    {
      int row_payment = this->play_row_for_random_odds(j);
      this->update_random_odds_stats_by_row(row_payment, j);

      if(!row_payment)
        break;
      else
      {
        if(row_payment > highest_payout_of_deal)
          highest_payout_of_deal = row_payment;
      }
    }
    m_highest_payout_per_deal_sum->add(highest_payout_of_deal);

    this->put_tower_cards_back_in_deck();
  }

  this->print_random_odds_stats(sample_size, base_bet);

  delete sample_size;
  delete i;
  delete m_deck;
}

int Table::play_row_for_random_odds(long unsigned int current_row)
{
  if(current_row == 0)
  {
    //special things for wilds and multipliers on the first card
    switch(m_tower[0][0]->get_face())
    {
      case WILD:  return 15;
      case XTWO:  return 15;
      case XFIVE: return 20;
      case XTEN:  return 25;
      default:    return m_tower[0][0]->money_value();
    }
  }

  int winnings = 0;
  int multiplier = 1;
  for(unsigned long int i = 0; i < m_tower[current_row].size(); i++)
  {
    Card* current_card = m_tower[current_row][i];
    bool conflict = false;

    if(current_card->is_special())
    {
      if(current_card->get_face() == XTWO)  multiplier *= 2;
      else if(current_card->get_face() == XFIVE) multiplier *= 5;
      else if(current_card->get_face() == XTEN)  multiplier *= 10;
    }
    else
    {
      if(i == 0)                                      //far left card
      {
        if(this->conflict(current_card, m_tower[current_row - 1][i])) conflict = true;
      }
      else if(i == m_tower[current_row].size() - 1)  //far right card
      {
        if(this->conflict(current_card, m_tower[current_row - 1][i - 1])) conflict = true;
      }
      else                                           //cards in the middle - check both "parents"
      {
        if(this->conflict(current_card, m_tower[current_row - 1][i])) conflict = true;
        if(this->conflict(current_card, m_tower[current_row - 1][i - 1])) conflict = true;
      }
      if(conflict)
      {
        if(!m_savior_card)
        {
          return 0;
        }
        else
        {
          //Swap out card with savior card
          m_deck->put_card_back(m_tower[current_row][i], true);
          m_tower[current_row][i] = m_savior_card;
          current_card = m_savior_card;
          m_savior_card = NULL;

          //Check the new current card
          if(current_card->is_special())
          {
            if(current_card->get_face() == XTWO) {multiplier *= 2;   continue;}
            if(current_card->get_face() == XFIVE){multiplier *= 5;   continue;}
            if(current_card->get_face() == XTEN) {multiplier *= 10;  continue;}
          }

          if(i == 0)                                     //far left card
          {
            if(this->conflict(current_card, m_tower[current_row - 1][i])) return 0;
          }
          else if(i == m_tower[current_row].size() - 1)  //far right card
          {
            if(this->conflict(current_card, m_tower[current_row - 1][i - 1])) return 0;
          }
          else                                           //cards in the middle - check both "parents"
          {
            if(this->conflict(current_card, m_tower[current_row - 1][i]))     return 0;
            if(this->conflict(current_card, m_tower[current_row - 1][i - 1])) return 0;
          }
        }
      }
      winnings += m_tower[current_row][i]->money_value();
    }
  }

  return winnings_calc(winnings, multiplier, current_row);
}

void Table::update_random_odds_stats_by_row(int win_payment, long unsigned int row)
{
  if(win_payment)
  {
    m_num_wins_by_row[row]->add(1);

    if(row < m_tower.size())
    {
      m_winning_sum_by_row[row]->add(win_payment);
      if(win_payment > m_highest_payout_by_row[row])
        m_highest_payout_by_row[row] = win_payment;
    }

    //Breakdown the last row
    if(row == m_tower.size() - 1)
    {
      if(!m_savior_card) //JACKPOT
      {
        m_possible_jackpots->add(1);
        m_jackpots_payouts_sum->add(win_payment);
        if(win_payment > m_highest_jackpot_payout)
          m_highest_jackpot_payout = win_payment;
      }
      else               //MEGA JACKPOT!!
      {
        m_possible_mega_jackpots->add(1);
        m_mega_jackpots_payouts_sum->add(win_payment);
        if(win_payment > m_highest_mega_jackpot_payout)
          m_highest_mega_jackpot_payout = win_payment;
      }
    }
  }
  else
  {
    m_max_row_before_loss[row - 1]->add(1);
    while(row < m_tower.size())
    {
      m_num_losses_by_row[row]->add(1);
      row++;
    }
  }
}

void Table::print_random_odds_stats(BigInt* sample_size, int base_bet)
{
  cout << "Ran ";
  sample_size->print();
  cout << " random deals with the following deck breakdown:" << endl;

  for(CardFace i = CardFace::ONE; i != CardFace::LAST; i = CardFace(i + 1))
  {
    cout << i << ": " << m_deck->get_card_count(i) << endl;
  }
  cout << "****************" << endl;

  BigInt* total_money_input = new BigInt(sample_size);
  total_money_input->multiply(base_bet);

  cout << "It costs ";
  total_money_input->print();
  cout << " to get dealt ";
  sample_size->print();
  cout << " times (base bet: " << base_bet << ")" << endl << endl;

  for(long unsigned int j = 0; j < m_tower.size(); j++)
  {
    cout << "Row " << j + 1 << endl;

    cout << "-  Furthest row: ";
    m_max_row_before_loss[j]->print();
    cout << endl;

    cout << "-  Losses: ";
    m_num_losses_by_row[j]->print();
    cout << endl;

    cout << "-  Wins: ";
    m_num_wins_by_row[j]->print();
    cout << endl;

    cout << "-  Winnings: ";
    m_winning_sum_by_row[j]->print();
    if(m_winning_sum_by_row[j]->greater_than(total_money_input))
      cout << "\t\t<- Problem";
    cout << endl;

    cout << "-  Highest Payout: " << m_highest_payout_by_row[j] << endl;

    cout << endl;
  }
  //The surviving to the last row is either a Jackpot or a MEGA Jackpot
  cout << "- JACKPOTS" << endl;
  cout << "- - Wins: ";
  m_possible_jackpots->print();
  cout << endl;
  cout << "- - Winnings: ";
  m_jackpots_payouts_sum->print();
  cout << endl;
  cout << "- - Highest Payout: " << m_highest_jackpot_payout << endl;
  cout << endl;

  cout << "- MEGA JACKPOTS" << endl;
  cout << "- - Wins: ";
  m_possible_mega_jackpots->print();
  cout << endl;
  cout << "- - Winnings: ";
  m_mega_jackpots_payouts_sum->print();
  cout << endl;
  cout << "- - Highest Payout: " << m_highest_mega_jackpot_payout << endl;
  cout << endl;

  cout << "Payout if you took the best payout every time: ";
  m_highest_payout_per_deal_sum->print();
  cout << endl;
}

void Table::put_tower_cards_back_in_deck()
{
  for(long unsigned int i = 0; i < m_tower.size(); i++)
  {
    for(long unsigned int j = 0; j < m_tower[i].size(); j++)
    {
      m_deck->put_card_back(m_tower[i][j], true);
    }
  }
  if(m_savior_card) m_deck->put_card_back(m_savior_card, true);
}

void Table::bf_odds(long unsigned int current_row,
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

      bf_odds(next_row,
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
    m_deck->put_card_back(current_card, false);
    m_tower[current_row][current_spot_in_row] = NULL;

    return;
  }

  //It is the savior card
  //- Solve the conflict
  //- Then do stats updates
  else
  {
    m_savior_card = m_deck->deal_card_by_face(current_cardface);

    if(second_conflict_row == 0)  //JACKPOT OPPORTUNITY
    {
      if(first_conflict_row == 0) //MEGA JACKPOT OPPORTUNITY!!!
      {
        m_possible_mega_jackpots->add(1);
        int payout = evaluate_winnings(m_tower.size() - 1);
        m_mega_jackpots_payouts_sum->add(payout);

      }
      else                        //JACKPOT OPPORTUNITY
      {
        m_possible_jackpots->add(1);
        int payout = evaluate_winnings(m_tower.size() - 1);
        m_jackpots_payouts_sum->add(payout);
      }

      //Update the stats for the rest of the rows
      for(long unsigned int i = 0; i < m_tower.size(); i++)
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
          continue; //Saves us 2 "if"s
        }

        if(i == first_conflict_row)
        {
          //Handle conflict
	  // - NOTE: We can only flow here if(first_conflict_row < second_conflict_row)
          //       - There is only one conflict on the first_conflict_row)

          // - NOTE: evaluate_winnings(i) requires the savior card to be moved into the row
          //       - Must keep track of the original_conflicted_card so we can replace it back after row payout evaluation
          Card* original_conflicted_card = m_tower[first_conflict_row][first_conflict_index];

          //Changing out the first conflict card does not resolves the conflict
          //   - Loss on current row -> do not add current row winnings to cumulative row payouts
          if(!handle_conflict_for_bf(first_conflict_row, first_conflict_index))
          {
            //Put the original_conflict_card back in the tower before recursing back up
            m_tower[first_conflict_row][first_conflict_index] = original_conflicted_card;

            break;
          }

          //Changing out the first conflict card resolves the conflict
          else
          {
            m_winning_sum_by_row[i]->add(this->evaluate_winnings(i));

            //Need to check if the new card created a conflict in the next row
            //  -NOTE: We know its not a Jackpot because we already checked for that
            //       - There is a row below us to check against
            //         - There is always a left and right child
            Card* left_child  = m_tower[first_conflict_row + 1][first_conflict_index];
            Card* right_child = m_tower[first_conflict_row + 1][first_conflict_index + 1];

            if(this->conflict(left_child, m_tower[first_conflict_row][first_conflict_index]))
              second_conflict_row = first_conflict_row + 1;

            if(this->conflict(right_child, m_tower[first_conflict_row][first_conflict_index]))
              second_conflict_row = first_conflict_row + 1;

            //Row payout evaluated -> Put the original_conficted_card back into the tower
            m_tower[first_conflict_row][first_conflict_index] = original_conflicted_card;

            continue; //Saves us an "if"
          }
        }

        //First conflict row resolved and didnt create a conflict below it
        //  - We are between first and second conflict row
        //if(i > first_conflict_row)    - process of elimination -> dont need to ask
          m_winning_sum_by_row[i]->add(this->evaluate_winnings(i));
      }

      m_max_row_before_loss[second_conflict_row - 1]->add(1);
    }

    m_total_possibilities->add(1);

    //Put the card back into the deck
    m_deck->put_card_back(m_savior_card, false);
    m_savior_card = NULL;

    return;
  }
}

//This will only get called once the Savior card has been dealt
//This redirects the conflict card's pointer to the savior card
//  - Needs to happen to evaluate the payout on a succeful resolve
//  - Need to keep track of the card we redrected from before calling this
//return true means the redirect resolves the conflict
//return false means the redirect does not resolve the conflict
bool Table::handle_conflict_for_bf(long unsigned int conflict_row,
                                   long unsigned int conflict_index)
{
  m_tower[conflict_row][conflict_index] = m_savior_card;

  if(conflict_index == 0)
  {
    return !(this->conflict(m_tower[conflict_row][0], m_tower[conflict_row - 1][0]));
  }
  else if(conflict_index == (m_tower[conflict_row].size() - 1))
  {
    return !(this->conflict(m_tower[conflict_row][conflict_index], m_tower[conflict_row - 1][conflict_index - 1]));
  }
  else
  {
    if(this->conflict(m_tower[conflict_row][conflict_index], m_tower[conflict_row - 1][conflict_index]))     return false;
    if(this->conflict(m_tower[conflict_row][conflict_index], m_tower[conflict_row - 1][conflict_index - 1])) return false;

    return true;
  }
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

void Table::announce_row(long unsigned int current_row)
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

void Table::announce_big_win()
{
  cout << "  ########     ###      #####          ###                     ###  ###  #####      ###" << endl;
  cout << "  ###    ##    ###    ###   ###        ###                     ###  ###  ### ##     ###" << endl;
  cout << "  ###     ##   ###   ###     ###        ###        ###        ###   ###  ###  ##    ###" << endl;
  cout << "  ###    ##    ###  ###                  ###      ######     ###    ###  ###  ##    ###" << endl;
  cout << "  ########     ###  ###                  ###     ### ###     ###    ###  ###   ##   ###" << endl;
  cout << "  ###    ##    ###  ###                   ###   ###   ###   ###     ###  ###   ##   ###" << endl;
  cout << "  ###     ##   ###  ###    ######         ###   ###   ###   ###     ###  ###    ##  ###" << endl;
  cout << "  ###      ##  ###   ###      ###          ### ###     ### ###      ###  ###    ##  ###" << endl;
  cout << "  ###     ##   ###    ###   ### #          ### ###     ### ###      ###  ###     ## ###" << endl;
  cout << "  #########    ###      #####   #           #####       #####       ###  ###     ######" << endl;

  return;
}

void Table::announce_jackpot()
{
  cout << "         ###    ########        ######     ###     ###  #########         ########     #############" << endl;
  cout << "         ###   ###    ###     ###    ###   ###     ###  ###########     ###      ###   #############" << endl;
  cout << "         ###  ###      ###   ###      ###  ###    ###   ###      ###   ###        ###       ###"      << endl;
  cout << "         ###  ###      ###  ###            ###   ###    ###       ##  ###          ###      ###"      << endl;
  cout << "         ###  ###      ###  ###            ### ###      ###      ###  ###          ###      ###"      << endl;
  cout << "         ###  ############  ###            ######       ###########   ###          ###      ###"      << endl;
  cout << "         ###  ############  ###            ### ###      #########     ###          ###      ###"      << endl;
  cout << "  ###    ###  ###      ###   ###      ###  ###   ###    ###            ###        ###       ###"      << endl;
  cout << "   ###  ###   ###      ###    ###    ###   ###    ###   ###             ###      ###        ###"      << endl;
  cout << "     #####    ###      ###      ######     ###     ###  ###               ########          ###"      << endl;

  return;
}

void Table::announce_mega_jackpot()
{
  cout << "  ######       #####  ###########     #####        ########         #####      #####      #####"   << endl;
  cout << "  #######     ######  ###########   ###   ###     ###     ###      ### ###    ### ###    ### ###"  << endl;
  cout << "  ###  ###   ### ###  ###          ###     ###   ###       ###    ###   ###  ###   ###  ###   ###" << endl;
  cout << "  ###   ### ###  ###  ###         ###            ###       ###    ###   ###  ###   ###  ###   ###" << endl;
  cout << "  ###    #####   ###  #######     ###            ###       ###    ###   ###  ###   ###  ###   ###" << endl;
  cout << "  ###            ###  #######     ###            #############     ### ###    ### ###    ### ###"  << endl;
  cout << "  ###            ###  ###         ###    ######  #############      #####      #####      #####"   << endl;
  cout << "  ###            ###  ###          ###      ###  ###       ###"                                    << endl;
  cout << "  ###            ###  ###########   ###   ### #  ###       ###       ###        ###        ###"    << endl;
  cout << "  ###            ###  ###########     #####   #  ###       ###       ###        ###        ###"    << endl;

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
