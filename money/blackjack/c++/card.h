#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <string>
using namespace std;

enum CardValue{
    ACE = 1,
    TWO = 2,
    THREE = 3, 
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    TEN = 10,
    JACK = 11,
    QUEEN = 12,
    KING = 13
};

enum CardSuit {
    SPADES,
    DIAMONDS,
    CLUBS,
    HEARTS
};

class Card
{
    public:
        Card(int value, int suit);
        string   print();
        string   print_blackjack();

        int      soft_blackjack_value();
        int      hard_blackjack_value();

        CardValue  get_value() {return m_value;}
        CardSuit   get_suit()  {return m_suit;}

    private:
        CardValue m_value;
        CardSuit m_suit;
};
#endif

