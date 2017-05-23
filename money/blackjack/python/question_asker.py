class QuestionAsker:
  def aquire_int(self, question):
    while(1):
      try:
        tmpstr = int(input(question))
      except:
        print("Bad Int")
        continue
      return tmpstr


#qa = QuestionAsker
#tmpint = qa.aquire_int("Gimme an int: ")
#tmpint2 = tmpint + 2
#print("your int plus 2: ", tmpint2)
