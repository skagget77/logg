#include <iostream>

/*
 * To change the global log level pass LOGG_LOG_LEVEL with your desired log
 * level to cmake, e.g:
 *
 * $ cmake -DLOGG_LOG_LEVEL=logg::INFO ..
 */

#include "logg/logg.h"

namespace {
  std::ostream& log = std::cout;
}

int add_two_integers(int a, int b) {
  const auto& trace = logg::trace(log);
  trace << "a=" << a << ", b=" << b;
  int r = a + b;
  trace << ", r=" << r;
  return r;
}

int main() {
  int r = add_two_integers(3, 5);
  std::cout << "r=" << r << std::endl;
}
