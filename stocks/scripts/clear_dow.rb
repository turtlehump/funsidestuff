root = "/home/phil/funsidestuff/stocks"

dow = `ls #{root}/DOW/stocks`.split(/\n/)

dow.each { |stock|
  `rm -rf #{root}/DOW/stocks/#{stock}/*`
}
