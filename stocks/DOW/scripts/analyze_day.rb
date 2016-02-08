root = "/home/phil/funsidestuff/stocks"
day = `date | awk '{print $2"_"$3"_"$6}'`.chomp

stocks = `ls #{root}/DOW/stocks`.split(/\n/) #there will always be stocks because of the setup script

#!empty? == something exists within
if(!`ls #{root}/DOW/stocks/#{stocks[0]}`.empty?) #are there day directories?
  if(!`ls #{root}/DOW/stocks/#{stocks[0]}/#{day}`.empty?) #does today have data?
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
          `echo "#{stock.center(8)} Average = #{average.to_s.center(8)}   Low = #{low.to_s.center(8)}   High = #{high.to_s.center(8)}" > #{root}/DOW/days/#{day}`
        else #append file
          `echo "#{stock.center(8)} Average = #{average.to_s.center(8)}   Low = #{low.to_s.center(8)}   High = #{high.to_s.center(8)}" >> #{root}/DOW/days/#{day}`
        end
        `echo "         Open = #{open.to_s.center(8)}     Close = #{close.to_s.center(8)}  Change = #{change.to_s.center(8)}" >> #{root}/DOW/days/#{day}`
        if(i < stocks.size - 1)
          `echo >> #{root}/DOW/days/#{day}`
        end
      else
        puts "#{stock.center(8)} Average = #{average.to_s.center(8)}   Low = #{low.to_s.center(8)}   High = #{high.to_s.center(8)}"
        puts "         Open = #{open.to_s.center(8)}     Close = #{close.to_s.center(8)}  Change = #{change.to_s.center(8)}"
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
  puts "Stock has no data"
end
