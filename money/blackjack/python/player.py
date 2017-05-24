from hand import *

class Player:
  def __init__(self, name):
    self.name = name
    self.money = 300
    self.hands = []

  def __str__(self):
    return self.name + ": $" + str(self.money)

  def can_afford(self, amount):
    if(self.money < amount):
      return False
    else:
      return True
