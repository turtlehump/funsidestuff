from random import randint
from time import *
from card import *

class Deck:
  def __init__(self, num_decks):
    self.m_count = 0
    self.m_dealt = 0
    self.m_cards = []
    self.deck_count = num_decks
    for i in range(0, self.deck_count):
      for j in CardSuit:
        for k in CardValue:
          tmp = Card(k, j)
          self.m_cards.append(tmp)

  def print(self):
    for i in range(len(self.m_cards)):
      print(self.m_cards[i])

  #shuffle assumes you have a new, full deck
  def shuffle(self):
    shuffled = []
    while(len(self.m_cards) > 0):
      randcard = randint(0, len(self.m_cards) - 1)
      shuffled.append(self.m_cards.pop(randcard))
    self.m_cards = shuffled
    self.m_count = 0
    self.m_dealt = 0

  def cut(self):
    self.m_cut = randint( int(len(self.m_cards) / 10),\
                          int(len(self.m_cards) / 10) * 4)
                          #between 10-40%
    print("(The cut is just before " + str(self.m_cut) + " cards left)")
    sleep(3)

  def deal_top_card(self):
    sleep(1)
    self.m_dealt += 1
    return self.m_cards.pop()
