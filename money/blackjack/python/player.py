from hand import *

class Player:
  def __init__(self, name):
    self.name = name
    self.money = 300
    self.hands = []

  def __str__(self):
    return self.name + ": $" + str(self.money)
