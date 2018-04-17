#pragma once

#include <cstring>
#include <ostream>

// Platform specifics.
#ifdef _WIN32
#define LOGG_DETAIL_FUNCTION __FUNCSIG__
namespace logg::detail {
  const char separator = '\\';
}
#else
#define LOGG_DETAIL_FUNCTION __PRETTY_FUNCTION__
namespace logg::detail {
  const char separator = '/';
}
#endif

// Function macro, always defined.
#define LOGG_FUNCTION logg::detail::function{LOGG_DETAIL_FUNCTION}

// Source macro, always defined.
#define LOGG_SOURCE logg::detail::source{__FILE__, __LINE__}

// Aliases, defined unless explicitly disabled.
#ifndef LOGG_DISABLE_ALIASES
#define LGFUN LOGG_FUNCTION
#define lgfun LOGG_FUNCTION
#define LGSRC LOGG_SOURCE
#define lgsrc LOGG_SOURCE
#endif

namespace logg::detail {
  // Source function name.
  struct function {
    const char* name;
  };

  // Source file name and line number.
  struct source {
    const char* file;
    const unsigned line;
  };
}
