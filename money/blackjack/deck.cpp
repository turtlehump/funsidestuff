#include "deck.h"

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
  for(unsigned int i = 0; i < m_deck.size(); i++)
    delete m_deck[i];
  m_deck.clear();
}

void Deck::print()
{
  for(unsigned int i = 0; i < m_deck.size(); i++)
  {
    if(i == m_cut)
      cout << "CUT------------CUT" << endl;

    cout << i << ": " << m_deck[i]->print() << endl;
  }
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

void Deck::cut()
{
  //the cut should be somewhere between the last 10-40% of the deck
  //we deal from the back is put it somewhere between the first 10-40%
  m_cut = (m_original_card_count / 10) + (rand() % ((m_original_card_count / 10) * 3));
  //      10%                          + 0-30%

  cout << "(the cut is just before " << m_cut << " cards left)" << endl;
  sleep(3); //to show the cut
  return;
}

Card* Deck::deal_top_card()
{
  sleep(1);
  if(m_deck.size() == m_cut)
  {
    cout << endl << "Hit the cut, making a new deck" << endl;
    sleep(1);

    //delete the remaining deck
    for(unsigned int i = 0; i < m_deck.size(); i++)
      delete m_deck[i];
    m_deck.clear();

    //make a new deck
    for(int i = 0; i < m_original_card_count; i++)
    {
      Card* new_card = new Card(i % 13, (i / 13) % 4);
      m_deck.push_back(new_card);
    }
    cout << "Made new deck, now shuffling and cutting" << endl;
    this->shuffle();
    this->cut();
  }

  Card* top_card = m_deck[m_deck.size() - 1];
  m_deck.erase(m_deck.end() - 1);
  return top_card;
}
