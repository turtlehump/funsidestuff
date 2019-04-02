require 'csv'
my_csv = CSV.read $3

my_csv.sort! {|a,b| a[0] <=> b[0]}

my_csv.each {|line| p line}
