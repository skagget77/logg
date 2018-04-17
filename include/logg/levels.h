#pragma once

#include <limits>

namespace logg {
  /**
   * Standard log levels.
   *
   */
  enum levels : unsigned {
    OFF   = 0,
    FATAL = 100,
    ERROR = 200,
    WARN  = 300,
    INFO  = 400,
    DEBUG = 500,
    TRACE = 600,
    ALL   = std::numeric_limits<unsigned>::max()
  };
}
