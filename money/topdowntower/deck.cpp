#include "deck.h"

//deck should consist of 7 cards of 1-7 and 3 "jokers"
Deck::Deck()
{
  for(int i = 0; i < 7; i++) //7 each
  {
    for(int j = 1; j < 8; j++) //of 1-7
    {
      Card* new_card = new Card(j);
      m_deck.push_back(new_card);
    }
  }
  for(int i = 0; i < 3; i++) //and 3 "jokers"
  {
    Card* joker = new Card(0);
    m_deck.push_back(joker);
  }

  return;
}

Deck::~Deck()
{
  for(unsigned int i = 0; i < m_deck.size(); i++)
    delete m_deck[i];
  m_deck.clear();

  return;
}

void Deck::print()
{
  for(unsigned int i = 0; i < m_deck.size(); i++)
    cout << i << ": " << m_deck[i]->get_display_value() << endl;

  return;
}

//technically this is a wash...
vector<Card*> Deck::shuffle()
{
  srand((int) time(0));
  vector<Card*> new_deck;
  while(!m_deck.empty())
  {
    int rand_card = rand() % m_deck.size();
    new_deck.push_back(m_deck[rand_card]);
    m_deck.erase(m_deck.begin() + rand_card);
  }
  m_deck = new_deck;

  return m_deck;
}

Card* Deck::deal_top_card()
{
  Card* top_card = m_deck[m_deck.size() - 1];
  m_deck.erase(m_deck.end() - 1);

  return top_card;
}
