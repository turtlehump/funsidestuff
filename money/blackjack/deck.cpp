#include "deck.h"
#include <cstdlib>
#include <ctime>

Deck::Deck(int num_cards)
{
  m_original_card_count = num_cards;
  for(int i = 0; i < num_cards; i++)
  {
    Card* new_card = new Card(i % 13, (i / 13) % 4);
    m_deck.push_back(new_card);
  }
}

Deck::~Deck()
{
  while(!m_deck.empty())
  {
    Card* tmp = m_deck.front();
    delete tmp;
    m_deck.erase(m_deck.begin());
  }
}

void Deck::print()
{
  for(unsigned int i = 0; i < m_deck.size(); i++)
  {
    if(i == m_cut)
      cout << "CUT------------CUT" << endl;

    cout << i << ": "; m_deck[i]->print(); cout << endl;
  }
  return;
}

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

void Deck::cut()
{
  //the cut should be somewhere between the last 10-40% of the deck (60-90% from the front)
  m_cut = (m_original_card_count / 10) * (6 + (rand() % 4));
  cout << "(the cut is just before " << m_cut << ")" << endl;
  return;
}

Card* Deck::deal_top_card()
{
  Card* top_card = m_deck[0];
  m_deck.erase(m_deck.begin());
  return top_card;
}
