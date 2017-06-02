from hand import *

class Dealer:
  def __init__(self):
    self.m_hand = None

  def new_hand(self):
    del self.m_hand
    self.m_hand = Hand(0)

  def playing_str(self):
    return " Dealer: " + self.m_hand.dealer_playing_str()
