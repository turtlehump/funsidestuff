#include "deck.h"

Deck::Deck()
{
  for(int i = 0; i < 7; i++)  m_deck.push_back(new Card(CardFace::ONE));    //7 ONEs
  for(int i = 0; i < 7; i++)  m_deck.push_back(new Card(CardFace::TWO));    //7 TWOs
  for(int i = 0; i < 7; i++)  m_deck.push_back(new Card(CardFace::THREE));  //7 THREEs
  for(int i = 0; i < 7; i++)  m_deck.push_back(new Card(CardFace::FOUR));   //7 FOURs
  for(int i = 0; i < 7; i++)  m_deck.push_back(new Card(CardFace::FIVE));   //7 FIVEs
  for(int i = 0; i < 7; i++)  m_deck.push_back(new Card(CardFace::SIX));    //7 SIXs
  for(int i = 0; i < 7; i++)  m_deck.push_back(new Card(CardFace::SEVEN));  //7 SEVENs
  for(int i = 0; i < 4; i++)  m_deck.push_back(new Card(CardFace::WILD));   //4 WILDs
  for(int i = 0; i < 3; i++)  m_deck.push_back(new Card(CardFace::XTWO));   //3 X2s
  for(int i = 0; i < 2; i++)  m_deck.push_back(new Card(CardFace::XFIVE));  //2 X5s
  for(int i = 0; i < 1; i++)  m_deck.push_back(new Card(CardFace::XTEN));   //1 X10s
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
    cout << i << ": " << m_deck[i]->display_value() << endl;

  return;
}

//technically this is a wash, not a shuffle
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
  Card* top_card = *(m_deck.begin());
  m_deck.erase(m_deck.begin());

  return top_card;
}
