from question_asker import *
from deck import *
from player import *

qa = QuestionAsker()

def clear_screen():
  print('\n' * 59)

class Table:
############################
  def setup(self):
    self.set_deck()
    self.set_limits()
    self.set_players()

############################
  def simultaion(self):
    while(True):
      self.play_an_entire_hand()
      play_again = self.ask_play_again()

      if(play_again == 1):
        continue
      elif(play_again == 2):
        break

    self.player_money_print()

############################
  def set_deck(self):
    clear_screen()
    num_decks = qa.aquire_int("How many decks are we going to use? ")
    self.m_deck = Deck(num_decks)

############################
  def set_limits(self):
    clear_screen()
    #minimum bet setting
    while(True):
      self.min_bet = qa.aquire_int("What is the minimum bet? ")
      if(self.min_bet < 1):
        print("Minimum bet must be greater than 0.")
      else:
        break

    #maximum bet setting
    while(True):
      self.max_bet = qa.aquire_int("And the maximum bet? ")
      if(self.max_bet < 5 * self.min_bet):
        print("Maximum bet must be at least 5 times the minimum bet.")
      else:
        break

############################
  def set_players(self):
    clear_screen()
    num_players = 0
    while(num_players < 1):
      num_players = qa.aquire_int("How many players are playing? ")
    
    self.players = []
    for i in range(0, num_players):
      name = input("Player " + str(i + 1) + "'s name: ")
      tmp = Player(name)
      self.players.append(tmp)

############################
  def play_an_entire_hand(self):
      clear_screen()
      print("Playing Blackjack with " \
            + str(len(self.players)) \
            + " players.")

############################
  def ask_play_again(self):
    while(True):
      play_again = input("Play again? (y/n) ")
      if(play_again == 'y' or play_again == 'Y'):
        return 1
      if(play_again == 'n' or play_again == 'n'):
        return 2

############################
  def player_money_print(self):
    for i in self.players:
      print(i)
