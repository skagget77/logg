#pragma once

#include "levels.h"

// Log level specified by the client has priority, when not specified we
// default it based on the current build type.
#ifdef LOGG_LOG_LEVEL
#define LOGG_DETAIL_LOG_LEVEL LOGG_LOG_LEVEL
#else
#ifdef NDEBUG
#define LOGG_DETAIL_LOG_LEVEL logg::ERROR
#else
#define LOGG_DETAIL_LOG_LEVEL logg::ALL
#endif
#endif

namespace logg::detail {
  // Global log level, any log messages with a log level lower or equal to this
  // get written to the log output stream.
  constexpr const unsigned log_level = LOGG_DETAIL_LOG_LEVEL;
}
