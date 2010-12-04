# Loads mkmf which is used to make makefiles for Ruby extensions
require 'mkmf'

# Give it a name
extension_name = 'rpjsip'

# The destination
dir_config(extension_name)

# ld config
$LDFLAGS = "-lpjsua-i386-apple-darwin10.5.0 -lpjsip-ua-i386-apple-darwin10.5.0 -lpjsip-simple-i386-apple-darwin10.5.0 -lpjsip-i386-apple-darwin10.5.0 -lpjmedia-codec-i386-apple-darwin10.5.0 -lpjmedia-i386-apple-darwin10.5.0 -lpjmedia-audiodev-i386-apple-darwin10.5.0 -lpjnath-i386-apple-darwin10.5.0 -lpjlib-util-i386-apple-darwin10.5.0 -lresample-i386-apple-darwin10.5.0 -lmilenage-i386-apple-darwin10.5.0 -lsrtp-i386-apple-darwin10.5.0 -lgsmcodec-i386-apple-darwin10.5.0 -lspeex-i386-apple-darwin10.5.0 -lilbccodec-i386-apple-darwin10.5.0 -lg7221codec-i386-apple-darwin10.5.0 -lportaudio-i386-apple-darwin10.5.0  -lpj-i386-apple-darwin10.5.0 -lm -lpthread  -framework CoreAudio -framework CoreServices -framework AudioUnit -framework AudioToolbox -lcrypto -lssl  "  


# Do the work
if have_library "pjsua-i386-apple-darwin10.5.0" then 
  create_makefile(extension_name)
else
  puts "No pjsip library found"
end