#ifndef UTILS_H
#define UTILS_H

#include "log.h"
#include <stdlib.h>

static inline void fatal()
{
  errln("Aborting due to fatal call");
  abort();
}

#define assert_or_fatal(expr) \
    if (!(expr)) { errln("%s:%d:Fatal assertion failed", __FILE__, __LINE__); fatal(); }

#endif
