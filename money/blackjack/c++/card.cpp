#include "card.h"

Card::Card(int value, int suit)
{
  switch(value)
  {
    case 0: m_value = ACE;    break;
    case 1: m_value = TWO;    break;
    case 2: m_value = THREE;  break;
    case 3: m_value = FOUR;   break;
    case 4: m_value = FIVE;   break;
    case 5: m_value = SIX;    break;
    case 6: m_value = SEVEN;  break;
    case 7: m_value = EIGHT;  break;
    case 8: m_value = NINE;   break;
    case 9: m_value = TEN;    break;
    case 10: m_value = JACK;  break;
    case 11: m_value = QUEEN; break;
    case 12: m_value = KING;  break;
    default: cout << "Bad Card Value." << endl;
  }
  switch(suit)
  {
    case 0: m_suit = SPADES;   break;
    case 1: m_suit = DIAMONDS; break;
    case 2: m_suit = CLUBS;    break;
    case 3: m_suit = HEARTS;   break;
    default: cout << "Bad Card Suit." << endl;
  }
}

string Card::print()
{
  string tmp;
  switch(m_value)
  {
    case ACE:   tmp = "A";  break;
    case TWO:   tmp = "2";  break;
    case THREE: tmp = "3";  break;
    case FOUR:  tmp = "4";  break;
    case FIVE:  tmp = "5";  break;
    case SIX:   tmp = "6";  break;
    case SEVEN: tmp = "7";  break;
    case EIGHT: tmp = "8";  break;
    case NINE:  tmp = "9";  break;
    case TEN:   tmp = "10"; break;
    case JACK:  tmp = "J";  break;
    case QUEEN: tmp = "Q";  break;
    case KING:  tmp = "K";  break;
  }
  switch(m_suit)
  {
    case SPADES:   tmp += "s"; break;
    case DIAMONDS: tmp += "d"; break;
    case CLUBS:    tmp += "c"; break;
    case HEARTS:   tmp += "h"; break;
  }
  return tmp;
}

string Card::print_blackjack()
{
  string tmp;
  switch(m_value)
  {
    case ACE:   tmp = "A";  break;
    case TWO:   tmp = "2";  break;
    case THREE: tmp = "3";  break;
    case FOUR:  tmp = "4";  break;
    case FIVE:  tmp = "5";  break;
    case SIX:   tmp = "6";  break;
    case SEVEN: tmp = "7";  break;
    case EIGHT: tmp = "8";  break;
    case NINE:  tmp = "9";  break;
    case TEN:   tmp = "10"; break;
    case JACK:  tmp = "J";  break;
    case QUEEN: tmp = "Q";  break;
    case KING:  tmp = "K";  break;
  }
  return tmp;
}

int Card::soft_blackjack_value()
{
  int tmp;
  switch(m_value)
  {
    case ACE:   tmp =  11;   break;
    case TWO:   tmp =  2;   break;
    case THREE: tmp =  3;   break;
    case FOUR:  tmp =  4;   break;
    case FIVE:  tmp =  5;   break;
    case SIX:   tmp =  6;   break;
    case SEVEN: tmp =  7;   break;
    case EIGHT: tmp =  8;   break;
    case NINE:  tmp =  9;   break;
    case TEN:   tmp =  10;  break;
    case JACK:  tmp =  10;  break;
    case QUEEN: tmp =  10;  break;
    case KING:  tmp =  10;  break;
  }
  return tmp;
}

int Card::hard_blackjack_value()
{
  int tmp;
  switch(m_value)
  {
    case ACE:   tmp =  1;   break;
    case TWO:   tmp =  2;   break;
    case THREE: tmp =  3;   break;
    case FOUR:  tmp =  4;   break;
    case FIVE:  tmp =  5;   break;
    case SIX:   tmp =  6;   break;
    case SEVEN: tmp =  7;   break;
    case EIGHT: tmp =  8;   break;
    case NINE:  tmp =  9;   break;
    case TEN:   tmp =  10;  break;
    case JACK:  tmp =  10;  break;
    case QUEEN: tmp =  10;  break;
    case KING:  tmp =  10;  break;
  }
  return tmp;
}
