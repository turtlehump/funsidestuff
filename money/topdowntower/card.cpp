#include "card.h"

string Card::get_display_value()
{
  if(!m_revealed)
    return "-";

  if(m_value)
  {
    stringstream tmpstream;
    tmpstream << m_value;
    return tmpstream.str();
  }

  //(m_value == 0) represents a "joker"
  return "J";
}

void Card::print()
{
  if(!m_revealed)
    cout << "-";
  else
  {
    if(m_value)
      cout << m_value;
    else //(m_value == 0) represents a "joker"
      cout << "J";
  }

  return;
}
