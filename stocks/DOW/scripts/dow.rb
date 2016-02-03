root = "/home/phil/funsidestuff/stocks"
day = `date | awk '{print $2"_"$3"_"$6}'`.chomp

dow_stocks = `ls #{root}/DOW/stocks`.split(/\n/)
intraminute_values = Array.new(dow_stocks.size)
i = 0
dow_stocks.each { |stock|
  intraminute_values[i] = [stock]
  i += 1
}

#we only want to get the intra-minute data when we write to ery1min or do a -r
if(ARGV[0] == "-r" or ARGV[0] == "ery1min")
  i = 0
else
  i = 8
end 

while i < 9 do
  time = `date | awk '{print $4}'`.chomp.gsub(/[:]/, '') #replace : with nothing
  `curl http://money.cnn.com/data/dow30/ > #{root}/curdata/dow.curdata 2> #{root}/tmp/garb`
  dow = `#{root}/DOW/scripts/cnn_dow_web_parse`.split(/\n/)

  if(ARGV.size == 0 or ARGV[0] == "-r") 
    puts day
    puts time
    puts dow
  else
    dow.each { |stock|
      stk = stock.split(/ /)
      nickname = stk[0]
      price = stk[1]

      if(`find #{root} | grep /DOW/stocks/#{nickname}/#{day}/#{ARGV[0]}`.empty?)
        if(`find #{root} | grep /DOW/stocks/#{nickname}/#{day}`.empty?)
          if(`find #{root} | grep /DOW/stocks/#{nickname}`.empty?)
            `mkdir #{root}/DOW/stocks/#{nickname}/`
          end
          `mkdir #{root}/DOW/stocks/#{nickname}/#{day}`
        end
        `echo > #{root}/DOW/stocks/#{nickname}/#{day}/#{ARGV[0]}.data`
      end

      change = 0.0
      intraminute_values.each { |ary|
        if(nickname == ary[0])
          if(ary.size > 1) #has more than just the name, 1+ price
            change = price.to_f - ary[ary.size - 1].to_f
          else #yet to test
            last_price = `cat #{root}/DOW/stocks/#{nickname}/#{day}/#{ARGV[0]}.data | tail -n 1 | awk '{print $2}'`
            if(last_price.to_f != 0)
              change = price.to_f - last_price.to_f
            end
          end
          ary.push(price)
        end
      }

      `echo "#{time}#{String(price).center(12)}#{String(change.round(2)).center(8)}  i = #{i}" >> #{root}/DOW/stocks/#{nickname}/#{day}/#{ARGV[0]}.data`

      if(i == 8 and ARGV[0] == "ery1min")
        `echo >> #{root}/DOW/stocks/#{nickname}/#{day}/#{ARGV[0]}.data`
      end
      puts "#{nickname} updated @ #{day}:#{time}"
    } 
  end
  if(i < 8)
    puts
    sleep 6
  end
  i += 1
end
