#include "dealer.h"

void Dealer::reset_hand()
{
  delete m_hand;
  m_hand = new Hand(0);
}

void Dealer::print(bool final_print)
{
  (m_cur_playing)? cout << "*": cout << " ";
  cout << "Dealer:";
  m_hand->dealer_print(m_cur_playing, final_print);
  cout << endl;

  return;
}

int Dealer::hit(Card* new_card)
{
  return m_hand->hit(new_card);
}
