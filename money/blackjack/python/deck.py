from card import *

class Deck:
  def __init__(self, num_decks):
    self.m_cards = []
    for i in range(0, num_decks):
      for j in CardSuit:
        for k in CardValue:
          tmp = Card(k, j)
          self.m_cards.append(tmp)

  def __str__(self):
    tmp = ""
    index = 0
    for i in range(len(self.m_cards)):
      tmp = tmp + self.m_cards[i].__str__()
      if(i < len(self.m_cards) - 1):
        tmp = tmp + '\n'
    return tmp

#deck = Deck(2)
#print(deck)
