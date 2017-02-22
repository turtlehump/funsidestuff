#ifndef CARD_H
#define CARD_H

#include <iostream>

using namespace std;

class Card
{
    public:
        Card(int value, int suit);
        void     print();
        void     print_blackjack();
        int      get_blackjack_value();

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

    private:
        CardValue m_value;
        CardSuit m_suit;
};
#endif
