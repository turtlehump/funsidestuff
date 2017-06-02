from question_asker import *
from deck import *
from dealer import *
from player import *

qa = QuestionAsker()

def clear_screen():
  print('\n' * 59)

class Table:
############################
  def __init__(self):
    self.m_dealer = Dealer()

############################
  def setup(self):
    self.set_limits()
    self.set_deck()
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
  def set_limits(self):
    clear_screen()
    #minimum bet setting
    while(True):
      self.min_bet = qa.aquire_int("What is the minimum bet? ")
      if(self.min_bet < 1):
        print("\nMinimum bet must be greater than 0.\n")
      else:
        break

    #maximum bet setting
    while(True):
      self.max_bet = qa.aquire_int("And the maximum bet? (>= " \
                                   + str(5 * self.min_bet) + ") ")
      if(self.max_bet < 5 * self.min_bet):
        print("\nMaximum bet must be at least 5x times the minimum"\
              + " bet of " + str(self.min_bet) + " ("\
              + str(5 * self.min_bet) + ")\n")
      else:
        break

############################
  def set_deck(self):
    clear_screen()
    while(True):
      num_decks = qa.aquire_int("How many decks are we going to use? ")
      if(num_decks < 1):
        print("\nWe must play with a positive number of decks.\n")
      else:
        break
    tmp = Deck(num_decks)
    tmp.shuffle()
    tmp.cut()
    self.m_deck = tmp

############################
  def set_players(self):
    clear_screen()
    while(True):
      num_players = qa.aquire_int("How many players are at the table? ")
      if(num_players < 1):
        print("\nThere must be at least 1 player.\n")
      else:
        break

    print()
    self.m_players = []
    for i in range(0, num_players):
      name = input("Player " + str(i + 1) + "'s name: ")
      tmp = Player(name)
      self.m_players.append(tmp)

############################
#########THE BEEF###########
############################
  def play_an_entire_hand(self):
      self.set_hands_for_players()
      clear_screen()
#      print("Playing Blackjack with " \
#            + str(len(self.m_players)) \
#            + " players.")
#      for player in self.m_players:
#        print(player.m_name + " is playing " + str(len(player.hands)))
#        for hand in player.hands:
#          print(hand)
      self.starting_deal()

############################
  def ask_play_again(self):
    while(True):
      play_again = input("Play again? (y/n) ")
      if(play_again == 'y' or play_again == 'Y'):
        self.reset_players_hands()
        return 1
      elif(play_again == 'n' or play_again == 'n'):
        return 2
      else:
        print("\nInvalid Option\n")

############################
  def set_hands_for_players(self):
    clear_screen()
    self.player_money_print()
    print()
    print("Count: " + str(self.m_deck.m_count))
    print("Dealt: " + str(self.m_deck.m_dealt))
    print()
    for player in self.m_players:
      while(True):
        while(True):
          num_hands = qa.aquire_int("How many hands does "\
                              + player.m_name + " want to play? ")
          if(num_hands < 1):
            print('\n' + player.m_name \
                  + " must play a positive number of hands.\n")
          else:
            break
        while(True):
          bet = qa.aquire_int("And the bet? ("\
                              + str(self.min_bet) + "-"\
                              + str(self.max_bet) + ") ")
          if(bet < self.min_bet):
            print('\n' + player.m_name \
                  + " must bet more than " + str(self.min_bet) + '\n')
          elif(bet > self.max_bet):
            print('\n' + player.m_name \
                  + " must bet less than " + str(self.max_bet) + '\n')
          else:
            break

        total_bet = num_hands * bet

        if(not player.can_afford(total_bet)):
          print('\n' + player.m_name \
                + " can not afford this bet of " + str(total_bet) + '\n')
          continue
        else:
          for i in range(0, num_hands):
            tmp = Hand(bet)
            player.m_hands.append(tmp)
          break

############################
  def starting_deal(self):
    for i in range(0,2):
      for player in self.m_players:
        for hand in player.m_hands:
          hand.hit(self.m_deck.deal_top_card())
          self.dealing_print()

############################
  def dealing_print(self):
    #final_print = False
    clear_screen()
    print("****STARTING DEAL****\n")

    #Dealer is on top for display reasons
    #self.m_dealer.dealer_print(final_print)
    #print()
    for player in self.m_players:
      print(player)

############################
  def reset_players_hands(self):
    for player in self.m_players:
      player.m_hands.clear()

############################
  def player_money_print(self):
    for player in self.m_players:
      player.money_print()
