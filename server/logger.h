#pragma once

#include <string.h>


// to make shorter names in debug log, (for Windows - \\, for linux/mac - /)
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

// dont forget to include <iostream>
// you can use it like this "debuglog("Hello " << user->name)"
#define logmessage(message) std::cout << __FILENAME__ << "::" << __LINE__ \
<< "::" << __FUNCTION__ << "::" << message << std::endl

#define debuglog(message) logmessage("DEBUG:: " <<  message)
#define errorlog(message) logmessage("ERROR:: " <<  message)
#define successlog(message) logmessage("SUCCESS:: " <<  message)

