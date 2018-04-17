#include <iostream>

/*
 * To change the global log level pass LOGG_LOG_LEVEL with your desired log
 * level to cmake, e.g:
 *
 * $ cmake -DLOGG_LOG_LEVEL=logg::INFO ..
 */

#include "logg/logg.h"

int main() {
  logg::trace(std::cout, lgsrc) << "Hello, world!";
  logg::trace(std::cout, lgfun) << "Hello, world!";
}
