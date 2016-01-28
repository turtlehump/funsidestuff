root = "/home/phil/funsidestuff/stocks"
day = `date | awk '{print $2"_"$3"_"$6}'`.chomp

#we only want to get the intra-minute data when we write to ery1min 
if(ARGV.size == 0 or ARGV[0] == "ery1min")
  i = 0
else
  i = 8
end 

while i < 9 do
  time = `date | awk '{print $4}'`.chomp
  `curl http://money.cnn.com/data/dow30/ > #{root}/curdata/dow.curdata 2> #{root}/tmp/garb`
  `rm #{root}/tmp/garb`
  dow = `#{root}/scripts/cnn_dow_web_parse`.split(/\n/)
  `rm #{root}/curdata/dow.curdata`
  
  #update
  if(ARGV.size > 0)
    dow.each { |stock|
      stk = stock.split(/ /)
      nickname = stk[0]
      price = stk[1]
      if(`find #{root} | grep /DOW/stocks/#{nickname}/#{day}`.empty?)
        `mkdir #{root}/DOW/stocks/#{nickname}/#{day}`
      end
      ARGV.each { |arg|
        `echo #{time} #{price} >> #{root}/DOW/stocks/#{nickname}/#{day}/#{arg}.data`
        if(i == 8 and ARGV[0] == "ery1min")
          `echo >> #{root}/DOW/stocks/#{nickname}/#{day}/#{arg}.data`
        end
      }
    } 
    puts "Updated"
  else
    total = 0
    dow.each { |stock|
      stk = stock.split(/ /)
      price = stk[1]
      total += price.to_f
    }
    #std out
    puts day
    puts time
    puts dow
    puts "The DOW is at #{total}"
  end
  puts
  `sleep 6`
  i += 1
end
