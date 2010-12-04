require 'rubygems'
require 'rpjsip'

include Rpjsip

account_id = pjsip_init "proxy.azzu.com.br", "azzu", "12013283", "12345"
call account_id , "03496739006", "azzu"
puts pjsip_destroy