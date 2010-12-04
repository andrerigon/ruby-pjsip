# Load in the extension (on OS X this loads ./MyTest/mytest.bundle - unsure about Linux, possibly mytest.so)
require 'rubygems'
puts require 'rpjsip'

# MyTest is now a module, so we need to include it
include rpjsip

# Call and print the result from the test1 method
puts test1

puts add(1,2)

puts pjsip  

# => 10
