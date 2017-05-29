from time import *
from card import *

class Deck:
  def __init__(self, num_decks):
    self.m_cards = []
    for i in range(0, num_decks):
      for j in CardSuit:
        for k in CardValue:
          tmp = Card(k, j)
          self.m_cards.append(tmp)

  def print(self):
    for i in range(len(self.m_cards)):
      print(self.m_cards[i])

  def deal_top_card(self):
    sleep(1)
    return self.m_cards.pop()
