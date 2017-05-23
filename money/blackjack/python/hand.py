from card import *

class Hand:
  def __init__(self):
    self.m_cards = []

  def hit(self, new_card):
    self.m_cards.append(new_card)
    return self.value()

  def value(self):
    return 42
