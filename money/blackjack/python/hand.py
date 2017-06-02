from card import *

class Hand:
  def __init__(self, bet):
    self.m_cards = []
    self.m_bet = bet

  def __str__(self):
    i = 0
    tmp = "(" + str(self.m_bet) + "): "
    for card in self.m_cards:
      if(i > 0):
        tmp += ", "
      tmp += card.__str__()
      i += 1
    tmp += "\t[" + str(self.value()) + "]"
    return tmp

  def hit(self, new_card):
    self.m_cards.append(new_card)
    return self.value()

  def value(self):
    return 42

  def dealer_playing_str(self):
    tmp = ""
    size = len(self.m_cards)
    if(size == 0):
      return tmp
      #do nothing
    tmp += "-"
    for i in range(1, size):
      tmp += ", " + self.m_cards[i].__str__()
    return tmp
