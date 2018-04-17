#include <iostream>

/*
 * To change the global log level pass LOGG_LOG_LEVEL with your desired log
 * level to cmake, e.g:
 *
 * $ cmake -DLOGG_LOG_LEVEL=logg::INFO ..
 */

#include "logg/logg.h"

int main() {
  // Logging with the different predefined log levels.
  logg::fatal(std::cout) << "Hello, world!";
  logg::error(std::cout) << "Hello, world!";
  logg::warn(std::cout)  << "Hello, world!";
  logg::info(std::cout)  << "Hello, world!";
  logg::debug(std::cout) << "Hello, world!";
  logg::trace(std::cout) << "Hello, world!";

  // Logging a wide character string.
  logg::info(std::wcout) << L"Hello, world!";
}
