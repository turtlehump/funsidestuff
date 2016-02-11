root = "/home/phil/funsidestuff/stocks"

if(ARGV[0] != "-n")
  puts "Usage()"
  puts "$ new_portfolio.rb -n <name>"
elsif(ARGV.size != 2)
  puts "Usage()"
  puts "$ new_portfolio.rb -n <name>"
else
  name = ARGV[1]
  `mkdir #{root}/portfolio/profiles/#{name}`
end
