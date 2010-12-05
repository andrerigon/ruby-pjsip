require 'rubygems'
require 'rpjsip'

include Rpjsip

account_id = pjsip_init :proxy => "proxy.azzu.com.br",
                          :domain => "azzu", 
                          :user => "<user>", 
                          :passwd => "<passwd>" 
                        
call_id = make_call ( {:account_id => account_id , 
                    :number => "03498767899", 
                    :domain => "azzu" })  { puts "\n\n\n\n------------> calling!\n\n\n\n" } 
sleep(3)

end_call call_id

pjsip_destroy
