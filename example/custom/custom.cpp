#include <iostream>

/*
 * To change the global log level pass LOGG_LOG_LEVEL with your desired log
 * level to cmake, e.g:
 *
 * $ cmake -DLOGG_LOG_LEVEL=logg::INFO ..
 */

#include "logg/logg.h"

enum my_levels {
  FOO = 42,
  BAR = 314,
  BAZ = 999
};

int main() {
  // Logging with defined custom log levels.
  logg::log<FOO>(std::cout) << "Hello, world!";
  logg::log<BAR>(std::cout) << "Hello, world!";
  logg::log<BAZ>(std::cout) << "Hello, world!";

  // Logging with hard coded custom log level.
  logg::log<17>(std::cout) << "Hello, world!";
}
