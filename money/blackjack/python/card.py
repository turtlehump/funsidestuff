from enum import *

class CardValue(IntEnum):
  ACE = 1
  TWO = 2
  THREE = 3
  FOUR = 4
  FIVE = 5
  SIX = 6
  SEVEN = 7
  EIGHT = 8
  NINE = 9
  TEN = 10
  JACK = 11
  QUEEN = 12
  KING = 13

class CardSuit(Enum):
  SPADES = 1
  DIAMONDS = 2
  CLUBS = 3
  HEARTS = 4

class Card:
  def __init__(self, value, suit):
    self.m_value = CardValue(value)
    self.m_suit = CardSuit(suit)

  def __str__(self): #the print function
    tmpValue = ""
    tmpSuit = ""
    if self.m_value == CardValue.JACK:
      tmpValue = "J"
    elif self.m_value == CardValue.QUEEN:
      tmpValue = "Q"
    elif self.m_value == CardValue.KING:
      tmpValue = "K"
    elif self.m_value == CardValue.ACE:
      tmpValue = "A"
    else:
      tmpValue = CardValue(self.m_value)

    if self.m_suit == CardSuit.SPADES:
      tmpSuit = "s"
    elif self.m_suit == CardSuit.DIAMONDS:
      tmpSuit = "d"
    elif self.m_suit == CardSuit.CLUBS:
      tmpSuit = "c"
    else: #elif self.m_suit = CardSuit.HEARTS:
      tmpSuit = "h"

    return "{0}{1}".format(tmpValue, tmpSuit)


#for i in CardSuit:
#  for j in CardValue:
#    tmp = Card(j,i)
#    print(tmp)
