require 'rubygems'
require 'rpjsip'

include Rpjsip

account_id = pjsip_init "<proxy>", "<domain>", "<user>", "<passwd>"
call account_id , "<number>", "<domain>"
puts pjsip_destroy