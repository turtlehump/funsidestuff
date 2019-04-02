areacode=
firstthree=
lastfour=
wholenumber=

junkfile=junk.txt

site1=http://www.wheresmycellphone.com
site1var1=noArea #possibly iArea
site1var2=noNumb #possibly iNumb
# the 2 variables needed are "noArea" and "noNumb" for this site

site2=http://www.callmylostphone.com
site2var1=recipient #possibly iRecipient
# the 1 variable needed is "recipient"

site3=http://www.icantfindmyphone.com
site3var1=areaCode
site3var2=firstThree
site3var3=lastFour
#there are 3 variables for this site: areaCode, firstThree, lastFour

function Usage()
{
  echo "Usage:"
  echo "caller.sh -n <area_code> <first_three> <last_four>"
}

if [ $# -ne 0 ]
then
  echo THE NUMBER OF COMMAND ARGUEMENTS IS $#
  echo THE FIRST ARGUEMENT IS $1

  if [ $# -ne 5 ]
  then
    echo how the fuck do I call a function right here 1
  fi
  if [ $2 -ne "-n" ] then
  then
    echo how the fuck do I call a function right here 2
  fi
fi


echo "We will be calling ($areacode) $firstthree - $lastfour until I say stop"

counter=0
score=0
while [ $score -lt 20  ]
do
  echo ""
  echo "ROUND $counter! (round*2 = total calls attempted)"
  echo "SCORE: $score"
  echo ""


  echo "Calling from $site1."
  curl -s -d "$site1var1=$areacode&$site1var2=$firstthree$lastfour" $site1 > $junkfile
  if [ $(cat $junkfile | wc -l) -gt 2 ]
  then
    echo "FAILED! wait longer."
    echo ""
    rm $junkfile
  else
    echo "SUCCESS!"
    let score+=1
    echo ""
    echo "Call sent, now sleeping for a bit"
    rm $junkfile
    sleep 300
  fi


  echo "Calling from $site2."
  curl -s --data "$site2var1=$areacode&$site2var1=$firstthree$lastfour" $site2 > $junkfile
#  curl -s -d "$site2var1=$wholenumber" $site2 > $junkfile
  if [ $(cat $junkfile | wc -l) -gt 2 ]
  then
    echo "FAILED! wait longer."
    echo ""
    rm $junkfile
  else
    echo "SUCCESS!"
    let score+=1
    echo ""
    echo "Call sent, now sleeping for a bit"
    rm $junkfile
    sleep 300
  fi

#  echo "Calling from $site3."
#  curl -s --data "$site3var1=$areacode&$site3var2=$firstthree&$site3var3=$lastfour" $site3 > $junkfile
#  if [ $(cat $junkfile | wc -l) -gt 2 ]
#  then
#    echo "FAILED! wait longer."
#    echo ""
#    rm $junkfile
#  else
#    echo "SUCCESS!"
#    let score+=1
#    echo ""
#    echo "Call sent, now sleeping for a bit"
#    rm $junkfile
#    sleep 30
#  fi

  let counter+=1
done
