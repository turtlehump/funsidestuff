root = "~/funsidestuff/stocks"
`curl http://money.cnn.com/data/dow30/ > #{root}/DOW/dow.curdata 2> #{root}/tmp/garbage`
`rm #{root}/tmp/garbage`
puts `#{root}/scripts/cnn_dow_web_parse`
