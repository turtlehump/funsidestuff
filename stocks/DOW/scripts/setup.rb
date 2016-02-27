root = "/home/phil/funsidestuff/stocks"
day = `date | awk '{print $2"_"$3"_"$6}'`.chomp

if(`find #{root} | grep curdata`.empty?)
  `mkdir #{root}/curdata`
end

`curl http://money.cnn.com/data/dow30/ > #{root}/curdata/dow.curdata 2> #{root}/tmp`

dow = `#{root}/DOW/scripts/cnn_dow_web_parse`.split(/\n/)
dow.each { |stock|
  stk = stock.split(/ /)
  nickname = stk[0]

  if(`find #{root} | grep /DOW/stocks/#{nickname}/#{day}`.empty?)
    if(`find #{root} | grep /DOW/stocks/#{nickname}`.empty?)
      if(`find #{root} | grep /DOW/stocks/`.empty?)
        `mkdir #{root}/DOW/stocks/`
      end
      `mkdir #{root}/DOW/stocks/#{nickname}/`
    end
    `mkdir #{root}/DOW/stocks/#{nickname}/#{day}`
  end
}

if(`find #{root} | grep /DOW/days/#{day}`.empty?)
  if(`find #{root} | grep /DOW/days`.empty?)
    `mkdir #{root}/DOW/days/`
  end
  `echo "Trading day hasn't closed yet" > #{root}/DOW/days/#{day}`
end

