root = "/home/phil/funsidestuff/stocks"
day = `date | awk '{print $2"_"$3"_"$6}'`.chomp

stocks = `ls #{root}/DOW/stocks`.split(/\n/) #there will always be stocks because of the setup script

#!empty? == something exists within
if(!`find #{root} | grep /DOW/stocks/#{stocks[0]}`.empty?) #is there this stock directory?
  if(!`find #{root} | grep /DOW/stocks/#{stocks[0]}/#{day}`.empty?) #is there a "today" directory?
    if(!`find #{root} | grep /DOW/stocks/#{stocks[0]}/#{day}/ery1min.data`.empty?) #does today have data?
      i = 0
      stocks.each {|stock|
        intraminute_values = `cat #{root}/DOW/stocks/#{stock}/#{day}/ery1min.data | awk '{print $2}'`.split(/\n/).compact.reject{ |e| e.empty? }
  
        total = 0.0
        low = intraminute_values[0].to_f
        high = intraminute_values[0].to_f
        open = intraminute_values[0].to_f
        close = intraminute_values[intraminute_values.size - 1].to_f
        change = "%" + (((close - open) / open) * 100).round(2).to_s
        intraminute_values.each { |value|
          if(value.to_f < low)
            low = value.to_f
          end
          if(value.to_f > high)
            high = value.to_f
          end
          total += value.to_f
        } #intraminute_values.each
        average = (total/intraminute_values.size).round(2)
  
        if(ARGV[0] == "-save")
          if(i == 0) #new file
            `echo "#{"Stock".center(6)}#{"Open".center(8)}#{"Close".center(8)}#{"Low".center(8)}#{"High".center(8)}#{"Change".center(8)}#{"Average".center(8)}" > #{root}/DOW/days/#{day}`
          end
          `echo "#{stock.center(6)}#{open.to_s.center(8)}#{close.to_s.center(8)}#{low.to_s.center(8)}#{high.to_s.center(8)}#{change.to_s.center(8)}#{average.to_s.center(8)}" >> #{root}/DOW/days/#{day}`
          if(i < stocks.size - 1)
            `echo >> #{root}/DOW/days/#{day}`
          end
        else
          if(i == 0)
            puts "#{"Stock".center(6)}#{"Average".center(8)}#{"Low".center(8)}#{"High".center(8)}#{"Open".center(8)}#{"Close".center(8)}#{"Change".center(8)}"
          end
          puts "#{stock.center(6)}#{average.to_s.center(8)}#{low.to_s.center(8)}#{high.to_s.center(8)}#{open.to_s.center(8)}#{close.to_s.center(8)}#{change.to_s.center(8)}"
          if(i < stocks.size - 1)
            puts
          end
        end
        i += 1
      } #stocks.each
    else
      puts "Nothing for today"
    end
  else
    puts "Nothing for today"
  end
else
  puts "No stocks"
end
