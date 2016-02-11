root = "/home/phil/funsidestuff/stocks"
day = `date | awk '{print $2"_"$3"_"$6}'`.chomp

stocks = `ls #{root}/DOW/stocks`.split(/\n/)

#!empty? == something exists within
if(!`find #{root} | grep /DOW/stocks/#{stocks[0]}/#{day}`.empty?) #is there a "today" directory?
  if(!`find #{root} | grep /DOW/stocks/#{stocks[0]}/#{day}/ery1min.data`.empty?) #does today have data?
    stocks.each { |stock|
      files_to_correct = `ls #{root}/DOW/stocks/#{stocks[0]}/#{day}`.split(/\n/)
      files_to_correct.each { |file|
        within_time = Array.new
        intraminute_values = `cat #{root}/DOW/stocks/#{stock}/#{day}/#{file}`.split(/\n/)
        intraminute_values.each { |line|        
          time = line.split(/ /)[0].to_i        
          if(time > 63000 and time < 131000)
            if(file == "ery1min.data")
              if((time % 100) < 3)
                within_time.push("")
              end
            end
            within_time.push(line)
          end
        }
        within_time = within_time * "\n"
        output = File.open("#{root}/DOW/stocks/#{stock}/#{day}/#{file}", "w")
        output << within_time << "\n"
        output.close
      }
    }
  end
end
