class QuestionAsker:
  def aquire_int(self, question):
    while(1):
      try:
        tmpstr = int(input(question))
      except:
        print("\nBad Int\n")
        continue
      return tmpstr
