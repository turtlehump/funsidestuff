from card import *

class Hand:
  def __init__(self, bet):
    self.m_cards = []
    self.m_bet = bet

  def __str__(self):
    return "Hand with bet of " + str(self.m_bet)

  def hit(self, new_card):
    self.m_cards.append(new_card)
    return self.value()

  def value(self):
    return 42
