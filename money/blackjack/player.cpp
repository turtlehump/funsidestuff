#include "player.h"

Player::Player(string name)
{
  m_name = name;
  m_cur_playing = false;
  m_next_hand = 0;
  m_money = 300;
  m_prev_num_hands = 0;
  m_prev_bet = 0;
  return;
}

Player::Player(string name, double money)
{
  m_name = name;
  m_cur_playing = false;
  m_next_hand = 0;
  m_money = money;
  m_prev_num_hands = 0;
  m_prev_bet = 0;
  return;
}

Player::~Player()
{
  for(unsigned int i = 0; i < m_hands.size(); i++)
    delete m_hands[i];
  return;
}

void Player::set_hands_and_bet(int num_hands,
                               int bet)
{
  m_prev_num_hands = num_hands;
  m_prev_bet = bet;
  return;
}

void Player::repeat_last_hand()
{
  for(int i = 0; i < m_prev_num_hands; i++)
    this->add_hand(new Hand(m_prev_bet));

  return;
}

void Player::add_hand(Hand* new_hand)
{
  this->take_money(new_hand->get_bet());
  m_hands.push_back(new_hand);
  return;
}

Hand* Player::get_next_hand(bool still_dealing_starting_cards)
{
  if(m_hands.size() == 0)
    return NULL;

  if(m_next_hand == m_hands.size())
  //because we store the index of the next-hand-we-should-return
  //and valid indexes go to (size() - 1)
  {
    if(still_dealing_starting_cards)
      m_next_hand = 0;
    else
      return NULL;
  }
  Hand* tmp = m_hands[m_next_hand];
  m_next_hand++;

  return tmp;
}

bool Player::all_hands_are_bust_or_blackjack()
{
  for(unsigned int i = 0; i < m_hands.size(); i++)
  {
    if(!(m_hands[i]->has_bust() || m_hands[i]->is_blackjack()))
  //if(!m_hands[i]->has_bust() && !m_hands[i]->is_blackjack())
  //these are the same thing, pick whichever way your brain works
      return false;
  }

  return true;
}

void Player::reset_hands()
{
  m_next_hand = 0;
  for(unsigned int i = 0; i < m_hands.size(); i++)
    delete m_hands[i];
  m_hands.clear();
  m_insurance = 0;

  return;
}

void Player::set_insurance()
{
  double insurance_wanted = 0;
  string option;

  cout << "Does " << m_name << " want insurance? (y/n) ";
  cin >> option;

  if(option == "y" || option == "Y")
  {
    double total_bet = 0;
    for(unsigned int i = 0; i < m_hands.size(); i++)
      total_bet += m_hands[i]->get_bet();

    double possible_insurance = total_bet * 0.5;

    do
    {
      cout << "How much insurance do you want? ";
      cout << "(max: " << possible_insurance << ") ";
      cin >> insurance_wanted;

      if(insurance_wanted > possible_insurance || insurance_wanted < 0)
        cout << endl << "Not a valid amount of insurance." << endl << endl;
    }while(insurance_wanted > possible_insurance || insurance_wanted < 0);
  }

  m_insurance = insurance_wanted;
  m_money -= insurance_wanted;

  return;
}

void Player::print()
{
  (m_cur_playing)? cout << "*": cout << " ";
  cout << m_name << ":";
  for(unsigned int i = 0; i < m_hands.size(); i++)
  {
    if(i > 0)
    {
      cout << endl;
      for(unsigned int j = 0; j < m_name.size() + 2; j++)
      //starting space + name + ':'
        cout << " ";
    }
    (m_hands[i]->is_playing())? cout << "*": cout << " ";
    cout << i + 1 << " (" << m_hands[i]->get_bet() << "):";
    m_hands[i]->print();
  }
  cout << endl;

  return;
}


double Player::determine_payout(int  dealers_hand_value,
                                bool dealer_blackjack)
{
  double total_payout = 0;
  for(unsigned int i = 0; i < m_hands.size(); i++)
  {
    if(m_hands[i]->has_bust())
    {
      cout << m_name << ": Hand " << i + 1 << ": BUST";
      //total_payout += 0;
    }
    else
    {
      int hand_value = m_hands[i]->value();
      bool is_blackjack = m_hands[i]->is_blackjack();
      //so we dot have to calculate it a bunch of times

      if(is_blackjack && !dealer_blackjack)
      {
        cout << m_name << ": Hand " << i + 1 << ": WON (BLACKJACK)";
        total_payout += m_hands[i]->get_bet() * 2.5;
      }
      else if(dealers_hand_value > 21 || hand_value > dealers_hand_value)  
      {
        cout << m_name << ": Hand " << i + 1 << ": WON";
        total_payout += m_hands[i]->get_bet() * 2;
      }
      else if( (dealer_blackjack && is_blackjack) ||
               (hand_value == dealers_hand_value))
      {
        cout << m_name << ": Hand " << i + 1 << ": PUSHED";
        total_payout += m_hands[i]->get_bet();
        //took the bet out of the players money when bet was placed
      }
      else if( (dealer_blackjack && !is_blackjack) ||
               (hand_value < dealers_hand_value))
      {
        cout << m_name << ": Hand " << i + 1 << ": LOST";
        //total_payout += 0;
      }
    }
    cout << endl;
  }

  return total_payout;
}
