root = "/home/phil/funsidestuff/stocks"
day = `date | awk '{print $2"_"$3"_"$6}'`.chomp
time = `date | awk '{print $4}'`.chomp
`curl http://money.cnn.com/data/dow30/ > #{root}/curdata/dow.curdata 2> #{root}/tmp/garb`
`rm #{root}/tmp/garb`
dow = `#{root}/scripts/cnn_dow_web_parse`.split(/\n/)
`rm #{root}/curdata/dow.curdata`

#std out
puts day
puts time
puts dow

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
    }
  } 
  puts "Updated"
else
  puts "Done"
end
