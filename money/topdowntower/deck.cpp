#include "deck.h"

Deck::Deck()
{
  Card* new_card;

  //Only 1 card initializer but 2 pointers
  //m_deck is for shuffling and to play the game
  //m_card_counts is to run stats on the odds
  // - need the actual card objects because thats
  //   what gets inserted into the tower and the table
  //   object is what identifies conflicts

  vector<Card*> cards_of_a_kind;

  for(int i = 0; i < 7 /*7 ONEs*/  ; i++)
  {
    new_card = new Card(CardFace::ONE);

    m_deck.push_back(new_card);
    cards_of_a_kind.push_back(new_card);
    m_size++;
  }
  m_cards_by_faces.push_back(cards_of_a_kind);
  cards_of_a_kind.clear();

  for(int i = 0; i < 7 /*7 TWOs*/  ; i++)
  {
    new_card = new Card(CardFace::TWO);

    m_deck.push_back(new_card);
    cards_of_a_kind.push_back(new_card);
    m_size++;
  }
  m_cards_by_faces.push_back(cards_of_a_kind);
  cards_of_a_kind.clear();

  for(int i = 0; i < 7 /*7 THREEs*/; i++)
  {
    new_card = new Card(CardFace::THREE);

    m_deck.push_back(new_card);
    cards_of_a_kind.push_back(new_card);
    m_size++;
  }
  m_cards_by_faces.push_back(cards_of_a_kind);
  cards_of_a_kind.clear();

  for(int i = 0; i < 7 /*7 FOURs*/ ; i++)
  {
    new_card = new Card(CardFace::FOUR);

    m_deck.push_back(new_card);
    cards_of_a_kind.push_back(new_card);
    m_size++;
  }
  m_cards_by_faces.push_back(cards_of_a_kind);
  cards_of_a_kind.clear();

  for(int i = 0; i < 7 /*7 FIVEs*/ ; i++)
  {
    new_card = new Card(CardFace::FIVE);

    m_deck.push_back(new_card);
    cards_of_a_kind.push_back(new_card);
    m_size++;
  }
  m_cards_by_faces.push_back(cards_of_a_kind);
  cards_of_a_kind.clear();

  for(int i = 0; i < 7 /*7 SIXs*/  ; i++)
  {
    new_card = new Card(CardFace::SIX);

    m_deck.push_back(new_card);
    cards_of_a_kind.push_back(new_card);
    m_size++;
  }
  m_cards_by_faces.push_back(cards_of_a_kind);
  cards_of_a_kind.clear();

  for(int i = 0; i < 7 /*7 SEVENs*/; i++)
  {
    new_card = new Card(CardFace::SEVEN);

    m_deck.push_back(new_card);
    cards_of_a_kind.push_back(new_card);
    m_size++;
  }
  m_cards_by_faces.push_back(cards_of_a_kind);
  cards_of_a_kind.clear();

  for(int i = 0; i < 4 /*4 WILDs*/ ; i++)
  {
    new_card = new Card(CardFace::WILD);

    m_deck.push_back(new_card);
    cards_of_a_kind.push_back(new_card);
    m_size++;
  }
  m_cards_by_faces.push_back(cards_of_a_kind);
  cards_of_a_kind.clear();

  for(int i = 0; i < 3 /*3 X2s*/   ; i++)
  {
    new_card = new Card(CardFace::XTWO);

    m_deck.push_back(new_card);
    cards_of_a_kind.push_back(new_card);
    m_size++;
  }
  m_cards_by_faces.push_back(cards_of_a_kind);
  cards_of_a_kind.clear();

  for(int i = 0; i < 2 /*2 X5s*/   ; i++)
  {
    new_card = new Card(CardFace::XFIVE);

    m_deck.push_back(new_card);
    cards_of_a_kind.push_back(new_card);
    m_size++;
  }
  m_cards_by_faces.push_back(cards_of_a_kind);
  cards_of_a_kind.clear();

  for(int i = 0; i < 1 /*1 X10s*/  ; i++)
  {
    new_card = new Card(CardFace::XTEN);

    m_deck.push_back(new_card);
    cards_of_a_kind.push_back(new_card);
    m_size++;
  }
  m_cards_by_faces.push_back(cards_of_a_kind);
  cards_of_a_kind.clear();
}

Deck::~Deck()
{
  for(unsigned int i = 0; i < m_deck.size(); i++)
    if(m_deck[i]) delete m_deck[i];

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

//For playing the game vvvvvvvvvvvv

Card* Deck::deal_top_card()
{
  Card* top_card = *(m_deck.begin());
  m_deck.erase(m_deck.begin());

  return top_card;
}

//For playing the game ^^^^^^^^^^^^

//For calculating the odds  vvvvvvvvvvvvv

void Deck::reveal_all_for_odds()
{
  for(long unsigned int i = 0; i < m_cards_by_faces.size(); i++)
  {
    for(long unsigned int j = 0; j < m_cards_by_faces[i].size(); j++)
    {
      m_cards_by_faces[i][j]->reveal();
    }
  }

  return;
}

Card* Deck::deal_card_by_face(CardFace cardface)
{
  m_size--;
  Card* card = m_cards_by_faces[cardface][m_cards_by_faces[cardface].size() - 1];
  m_cards_by_faces[cardface].pop_back();
  return card;
}

void Deck::put_card_back(Card* card)
{
  m_size++;
  m_cards_by_faces[card->get_face()].push_back(card);
}

//For calculating the odds  ^^^^^^^^^^^^^
