root = "/home/phil/funsidestuff/stocks"
day = `date | awk '{print $2"_"$3"_"$6}'`.chomp

dow = `ls #{root}/DOW/stocks`.split(/\n/)

dow.each { |stock|
  if(!`find #{root} | grep /DOW/stocks/#{stock}/#{day}/ery1min.data`.empty?)
    `rm #{root}/DOW/stocks/#{stock}/#{day}/ery1min.data`
  end
}
