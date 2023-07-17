#include "card.h"

Card::Card(CardFace face)
{
  m_face = face;
  switch(m_face)
  {
    case ONE:   m_money_value = 1; m_special = false; break;
    case TWO:   m_money_value = 2; m_special = false; break;
    case THREE: m_money_value = 3; m_special = false; break;
    case FOUR:  m_money_value = 4; m_special = false; break;
    case FIVE:  m_money_value = 5; m_special = false; break;
    case SIX:   m_money_value = 6; m_special = false; break;
    case SEVEN: m_money_value = 7; m_special = false; break;
    case WILD:  m_money_value = 7; m_special = false; break;
    case XTWO:  m_money_value = 0; m_special = true;  break;
    case XFIVE: m_money_value = 0; m_special = true;  break;
    case XTEN:  m_money_value = 0; m_special = true;  break;
  }
  m_revealed = false;
  m_conflicted = false;
}

string Card::display_value()
{
  if(!m_revealed)
    return " - ";

  string display_value = "";

  if(this->m_conflicted) display_value += "*";
  else                  display_value += " ";

  switch(m_face)
  {
    case ONE:   display_value += "1"; break;
    case TWO:   display_value += "2"; break;
    case THREE: display_value += "3"; break;
    case FOUR:  display_value += "4"; break;
    case FIVE:  display_value += "5"; break;
    case SIX:   display_value += "6"; break;
    case SEVEN: display_value += "7"; break;
    case WILD:  return " W ";
    case XTWO:  return " X2";
    case XFIVE: return " X5";
    case XTEN:  return "X10";
  }

  if(this->m_conflicted) display_value += "*";
  else                   display_value += " ";

  return display_value;
}

void Card::print()
{
  if(!m_revealed)
    cout << " - ";

  switch(m_face)
  {
    case ONE:   cout << " 1 ";
    case TWO:   cout << " 2 ";
    case THREE: cout << " 3 ";
    case FOUR:  cout << " 4 ";
    case FIVE:  cout << " 5 ";
    case SIX:   cout << " 6 ";
    case SEVEN: cout << " 7 ";
    case WILD:  cout << " W ";
    case XTWO:  cout << " X2";
    case XFIVE: cout << " X5";
    case XTEN:  cout << "X10";
  }

  return;
}
