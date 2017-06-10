from hand import *

class Player:
  def __init__(self, name):
    self.m_name = name
    self.m_money = 300
    self.m_hands = []
    self.is_playing = False

  def __str__(self):
    i = 1
    tmp = ""
    if(self.is_playing):
      tmp += "*"
    else:
      tmp += " "
    for m_hand in self.m_hands:
      if i == 1:
        tmp += self.m_name + ":"
      else:
        tmp += ' ' * (len(self.m_name) + 2)
      if(m_hand.is_playing):
        tmp += "*"
      else:
        tmp += " "
      tmp += str(i) + " "
      tmp += m_hand.__str__()
      if(i < len(self.m_hands)):
        tmp += '\n'
      i += 1
    return tmp

  def can_afford(self, amount):
    if(self.m_money < amount):
      return False
    else:
      return True

  def money_print(self):
    print(self.m_name + ": $" + str(self.m_money))
