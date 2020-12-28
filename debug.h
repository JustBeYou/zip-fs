#ifndef _DEBUG_H_
#define _DEBUG_H_

#define DEBUG_MSG "\033[0;31m[DEBUG]\033[0m "

#ifdef DEBUG
#define debug(x) (x)
#else
#define debug(x)
#endif

#endif