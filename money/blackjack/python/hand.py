from card import *

class Hand:
  def __init__(self, bet):
    self.m_cards = []
    self.m_bet = bet
    self.is_playing = False

###############################
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

###############################
  def hit(self, new_card):
    self.m_cards.append(new_card)
    return self.value()

###############################
  def value(self):
    soft_value = self.soft_value()
    hard_value = self.hard_value()

    if(soft_value <= 21):
      return soft_value

    if((soft_value - hard_value) > 10):
      while(hard_value <= 11):
        hard_value += 10
    return hard_value

###############################
  def soft_value(self):
    soft_value = 0
    for card in self.m_cards:
      soft_value += card.soft_value()
    return soft_value

###############################
  def hard_value(self):
    hard_value = 0
    for card in self.m_cards:
      hard_value += card.hard_value()
    return hard_value

###############################
  #dealer will only 0, 1, or 2 cards
  def dealer_waiting_str(self):
    tmp = ""
    size = len(self.m_cards)
    if(size == 0):
      return tmp
      #do nothing
    tmp += self.m_cards[0].__str__()
    if(size == 2):
      tmp += ", -"
    return tmp

###############################
  def dealer_playing_str(self):
    tmp = ""
    for i in range(0, len(self.m_cards)):
      tmp += ", " + self.m_cards[i].__str__()
    return tmp
