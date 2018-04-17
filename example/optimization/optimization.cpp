#include <iostream>
#include <vector>

/*
 * To change the global log level pass LOGG_LOG_LEVEL with your desired log
 * level to cmake, e.g:
 *
 * $ cmake -DLOGG_LOG_LEVEL=logg::INFO ..
 */

#include "logg/logg.h"

// Macro to turn off vectorization for a function. Makes the disassembly a bit
// easier to read and compare.
#ifdef __GNUG__
#define NO_VECTORIZATION __attribute__((optimize("no-tree-vectorize")))
#else
#define NO_VECTORIZATION
#endif

namespace {
  auto& log = std::cout;
}

// Calculates n! without any logging.
NO_VECTORIZATION
int fac1(int n) {
  int p = 1;

  for (auto i = 2; i < n; i++) {
    p *= i;
  }

  return p;
}

// Calculates n! with logging at the INFO level.
NO_VECTORIZATION
int fac2(int n) {
  int p = 1;

  logg::info(log) << "n=" << n;

  for (auto i = 2; i < n; i++) {
    p *= i;
  }

  logg::info(log) << "p=" << p;

  return p;
}

int main() {
  auto p1 = fac1(6);
  std::cout << "p1: " << p1 << std::endl;

  auto p2 = fac2(6);
  std::cout << "p2: " << p2 << std::endl;
}
