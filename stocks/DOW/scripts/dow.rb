root = "/home/phil/funsidestuff/stocks"
day = `date | awk '{print $2"_"$3"_"$6}'`.chomp

dow_stocks = `ls #{root}/DOW/stocks`.split(/\n/)
intraminute_values = Hash.new{ |hash, key| hash[key] = Array.new }
dow_stocks.each { |stock|
  intraminute_values[stock]
}

files_to_write = Array.new
mins = `date | awk '{print $4}'`.chomp.split(/:/)[1].to_i #date $4-> hour:min:sec
files_to_write.push("ery1min") #always write to ery1min
if((mins % 2) == 0)
  files_to_write.push("ery2mins")
end
if((mins % 5) == 0)
  files_to_write.push("ery5mins")
end
if((mins % 10) == 0)
  files_to_write.push("ery10mins")
end
if((mins % 15) == 0)
  files_to_write.push("ery15mins")
end
if((mins % 20) == 0)
  files_to_write.push("ery20mins")
end
if((mins % 30) == 0)
  files_to_write.push("ery30mins")
end
if(mins == 0)
  files_to_write.push("ery60mins")
end

if(ARGV[0] == "-r" or ARGV[0] == "-save")
  puts files_to_write
  i = 9
else
  i = 1
end 

while i > 0 do
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
      price = stk[1].to_f

      if(i == 9) #loop through all the files, else only do the ery1minute
        files_to_write.each { |file|
          change = 0.0
          if(!`find  #{root} | grep /DOW/stocks/#{nickname}/#{day}/#{file}.data`.empty?)
            last_price = 0
            if(file == "ery1min") #file ends with a newline
              last_price = `cat #{root}/DOW/stocks/#{nickname}/#{day}/#{file}.data | tail -n 2 | head -n 2 | awk '{print $2}'`.to_f
            else
              last_price = `cat #{root}/DOW/stocks/#{nickname}/#{day}/#{file}.data | tail -n 1 | awk '{print $2}'`.to_f
            end
            if(last_price != 0)
              change = price - last_price
            end
          end
          `echo "#{String(time)}#{String(price).center(12)}#{String(change.round(2)).center(8)}" >> #{root}/DOW/stocks/#{nickname}/#{day}/#{file}.data`
        }
        intraminute_values[nickname].push(price)
      else
        last_price = intraminute_values[nickname][intraminute_values[nickname].length - 1]
        change = price - last_price
        `echo "#{String(time)}#{String(price).center(12)}#{String(change.round(2)).center(8)}" >> #{root}/DOW/stocks/#{nickname}/#{day}/ery1min.data`
        if (i == 1)
          `echo >> #{root}/DOW/stocks/#{nickname}/#{day}/ery1min.data`
        end
        intraminute_values[nickname].push(price)
      end
      puts "#{nickname} updated @ #{day}:#{time}"
    } 
  end
  if(i > 1)
    puts
    if(i == 9)
      sleep 5 #shorter sleep because we write to a bunch of files
    else
      sleep 6
    end
  end
  i -= 1
end
