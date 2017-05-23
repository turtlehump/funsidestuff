class Player:
  def __init__(self, name):
    #args -- tuple of anonymous arguments
    #kwargs -- dictionary of named arguments
    self.name = name
    self.money = 300
    self.hands = []

  def __str__(self):
    return self.name + ": $" + str(self.money)
