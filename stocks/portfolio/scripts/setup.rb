root = "/home/phil/funsidestuff/stocks"
date_ary = `date`.chomp.split(/ /)
month = date_ary[1]
day = date_ary[2].to_i
year = date_ary[5].to_i
date = month + "_" + day.to_s + "_" + year.to_s

if(`find #{root} | grep /portfolio/profiles`.empty?)
  `mkdir #{root}/portfolio/profiles`
else
  profiles = `ls #{root}/portfolio/profiles`
  profiles.each { |profile|
    if(`find #{root} | grep /portfolio/profiles/#{profile}/orders/`.empty?)
      `mkdir #{root}/portfolio/profiles#{profile}/orders`
    end
    if(`find #{root} | grep /portfolio/profiles/#{profile}/orders/#{date}`.empty?)
      `mkdir #{root}/portfolio/profiles/#{profile}/orders/#{date}`
    end
    day += 1
    date = month + "_" + day.to_s + "_" + year.to_s
    if(`find #{root} | grep /portfolio/profiles/#{profile}/orders/#{date}`.empty?)
      `mkdir #{root}/portfolio/profiles/#{profile}/orders/#{date}`
    end
  }
end
