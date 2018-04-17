#pragma once

#include <cstdio>
#include <ostream>

#include "config.h"
#include "header.h"
#include "source.h"

namespace logg::detail {
  // Level template, contains functions for writing the level and location
  // of the logg message header.
  template<unsigned Level>
  struct level {
    static void write_header(char* buf, unsigned size) noexcept {
      std::snprintf(buf, size, " CUSTOM(%d) - ", Level);
    }

    static void write_header(char* buf, unsigned size, const char* fun)
        noexcept {
      std::snprintf(buf, size, " CUSTOM(%d) {%s} - ", Level, fun);
    }

    static void write_header(char* buf, unsigned size, const char* file,
        unsigned line) noexcept {
      std::snprintf(buf, size, " CUSTOM(%d) {%s:%d} - ", Level, file, line);
    }
  };

  // Level template specializations for the standard log levels.
  template<>
  struct level<FATAL> {
    static void write_header(char* buf, unsigned size) noexcept {
      std::strncpy(buf, " FATAL - ", size);
    }

    static void write_header(char* buf, unsigned size, const char* fun)
        noexcept {
      std::snprintf(buf, size, " FATAL {%s} - ", fun);
    }

    static void write_header(char* buf, unsigned size, const char* file,
        unsigned line) noexcept {
      std::snprintf(buf, size, " FATAL {%s:%d} - ", file, line);
    }
  };

  template<>
  struct level<ERROR> {
    static void write_header(char* buf, unsigned size) noexcept {
      std::strncpy(buf, " ERROR - ", size);
    }

    static void write_header(char* buf, unsigned size, const char* fun)
        noexcept {
      std::snprintf(buf, size, " ERROR {%s} - ", fun);
    }

    static void write_header(char* buf, unsigned size, const char* file,
        unsigned line) noexcept {
      std::snprintf(buf, size, " ERROR {%s:%d} - ", file, line);
    }
  };

  template<>
  struct level<WARN> {
    static void write_header(char* buf, unsigned size) noexcept {
      std::strncpy(buf, " WARN - ", size);
    }

    static void write_header(char* buf, unsigned size, const char* fun)
        noexcept {
      std::snprintf(buf, size, " WARN {%s} - ", fun);
    }

    static void write_header(char* buf, unsigned size, const char* file,
        unsigned line) noexcept {
      std::snprintf(buf, size, " WARN {%s:%d} - ", file, line);
    }
  };

  template<>
  struct level<INFO> {
    static void write_header(char* buf, unsigned size) noexcept {
      std::strncpy(buf, " INFO - ", size);
    }

    static void write_header(char* buf, unsigned size, const char* fun)
        noexcept {
      std::snprintf(buf, size, " INFO {%s} - ", fun);
    }

    static void write_header(char* buf, unsigned size, const char* file,
        unsigned line) noexcept {
      std::snprintf(buf, size, " INFO {%s:%d} - ", file, line);
    }
  };

  template<>
  struct level<DEBUG> {
    static void write_header(char* buf, unsigned size) noexcept {
      std::strncpy(buf, " DEBUG - ", size);
    }

    static void write_header(char* buf, unsigned size, const char* fun)
        noexcept {
      std::snprintf(buf, size, " DEBUG {%s} - ", fun);
    }

    static void write_header(char* buf, unsigned size, const char* file,
        unsigned line) noexcept {
      std::snprintf(buf, size, " DEBUG {%s:%d} - ", file, line);
    }
  };

  template<>
  struct level<TRACE> {
    static void write_header(char* buf, unsigned size) noexcept {
      std::strncpy(buf, " TRACE - ", size);
    }

    static void write_header(char* buf, unsigned size, const char* fun)
        noexcept {
      std::snprintf(buf, size, " TRACE {%s} - ", fun);
    }

    static void write_header(char* buf, unsigned size, const char* file,
        unsigned line) noexcept {
      std::snprintf(buf, size, " TRACE {%s:%d} - ", file, line);
    }
  };

  // Proxy template. Has no state, discards all constructor parameters.
  template<unsigned Level, class Char, class Traits, bool Enable>
  struct proxy {
    proxy(std::basic_ostream<Char, Traits>&) noexcept {}
    proxy(std::basic_ostream<Char, Traits>&, const function&) noexcept {}
    proxy(std::basic_ostream<Char, Traits>&, const source&) noexcept {}
  };

  // Proxy template specialization. Used when logging is enabled.
  template<unsigned Level, class Char, class Traits>
  struct proxy<Level, Char, Traits, true> {
    proxy(std::basic_ostream<Char, Traits>& os)
        : os(os) {
      char buf[128];
      auto off = build_header(buf, sizeof (buf));
      level<Level>::write_header(buf + off, sizeof (buf) - off);
      os << buf;
    }

    proxy(std::basic_ostream<Char, Traits>& os, const function& fun)
        : os(os) {
      char buf[128];
      auto off = build_header(buf, sizeof (buf));
      level<Level>::write_header(buf + off, sizeof (buf) - off, fun.name);
      os << buf;
    }

    proxy(std::basic_ostream<Char, Traits>& os, const source& src)
        : os(os) {
      char buf[128];
      auto off = build_header(buf, sizeof (buf));
      level<Level>::write_header(buf + off, sizeof (buf) - off,
        std::strrchr(src.file, logg::detail::separator), src.line);
      os << buf;
    }

    ~proxy() {
      os << std::endl;
    }

    // Underlaying log stream.
    std::basic_ostream<Char, Traits>& os;
  };
}

namespace logg {
  // Alias template.
  template<unsigned Level, class Char, class Traits>
  using proxy = detail::proxy<Level, Char, Traits, Level <= detail::log_level>;

  /**
   * Returns a logger with log level set to @p Level.
   *
   * @tparam Level Log level.
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @return Logger.
   */
  template<unsigned Level, class Char, class Traits>
  proxy<Level, Char, Traits> log(std::basic_ostream<Char, Traits>& os) {
    return proxy<Level, Char, Traits>(os);
  }

  /**
   * Returns a logger with log level set to @p Level.
   *
   * @tparam Level Log level.
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @param fun Source function.
   * @return Logger.
   */
  template<unsigned Level, class Char, class Traits>
  proxy<Level, Char, Traits> log(
      std::basic_ostream<Char, Traits>& os, const detail::function& fun) {
    return proxy<Level, Char, Traits>(os, fun);
  }

  /**
   * Returns a logger with log level set to @p Level.
   *
   * @tparam Level Log level.
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @param src Source location.
   * @return Logger.
   */
  template<unsigned Level, class Char, class Traits>
  proxy<Level, Char, Traits> log(
      std::basic_ostream<Char, Traits>& os, const detail::source& src) {
    return proxy<Level, Char, Traits>(os, src);
  }

  /**
   * Returns a logger with log level set to FATAL.
   *
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @return Logger.
   */
  template<class Char, class Traits>
  proxy<FATAL, Char, Traits> fatal(std::basic_ostream<Char, Traits>& os) {
    return proxy<FATAL, Char, Traits>(os);
  }

  /**
   * Returns a logger with log level set to FATAL.
   *
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @param fun Source function.
   * @return Logger.
   */
  template<class Char, class Traits>
  proxy<FATAL, Char, Traits> fatal(
      std::basic_ostream<Char, Traits>& os, const detail::function& fun) {
    return proxy<FATAL, Char, Traits>(os, fun);
  }

  /**
   * Returns a logger with log level set to FATAL.
   *
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @param src Source location.
   * @return Logger.
   */
  template<class Char, class Traits>
  proxy<FATAL, Char, Traits> fatal(
      std::basic_ostream<Char, Traits>& os, const detail::source& src) {
    return proxy<FATAL, Char, Traits>(os, src);
  }
  
  /**
   * Returns a logger with log level set to ERROR.
   *
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @return Logger.
   */
  template<class Char, class Traits>
  proxy<ERROR, Char, Traits> error(std::basic_ostream<Char, Traits>& os) {
    return proxy<ERROR, Char, Traits>(os);
  }

  /**
   * Returns a logger with log level set to ERROR.
   *
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @param fun Source function.
   * @return Logger.
   */
  template<class Char, class Traits>
  proxy<ERROR, Char, Traits> error(
      std::basic_ostream<Char, Traits>& os, const detail::function& fun) {
    return proxy<ERROR, Char, Traits>(os, fun);
  }

  /**
   * Returns a logger with log level set to ERROR.
   *
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @param src Source location.
   * @return Logger.
   */
  template<class Char, class Traits>
  proxy<ERROR, Char, Traits> error(
      std::basic_ostream<Char, Traits>& os, const detail::source& src) {
    return proxy<ERROR, Char, Traits>(os, src);
  }

  /**
   * Returns a logger with log level set to WARN.
   *
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @return Logger.
   */
  template<class Char, class Traits>
  proxy<WARN, Char, Traits> warn(std::basic_ostream<Char, Traits>& os) {
    return proxy<WARN, Char, Traits>(os);
  }

  /**
   * Returns a logger with log level set to WARN.
   *
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @param fun Source function.
   * @return Logger.
   */
  template<class Char, class Traits>
  proxy<WARN, Char, Traits> warn(
      std::basic_ostream<Char, Traits>& os, const detail::function& fun) {
    return proxy<WARN, Char, Traits>(os, fun);
  }

  /**
   * Returns a logger with log level set to WARN.
   *
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @param src Source location.
   * @return Logger.
   */
  template<class Char, class Traits>
  proxy<WARN, Char, Traits> warn(
      std::basic_ostream<Char, Traits>& os, const detail::source& src) {
    return proxy<WARN, Char, Traits>(os, src);
  }

  /**
   * Returns a logger with log level set to INFO.
   *
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @return Logger.
   */
  template<class Char, class Traits>
  proxy<INFO, Char, Traits> info(std::basic_ostream<Char, Traits>& os) {
    return proxy<INFO, Char, Traits>(os);
  }

  /**
   * Returns a logger with log level set to INFO.
   *
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @param fun Source function.
   * @return Logger.
   */
  template<class Char, class Traits>
  proxy<INFO, Char, Traits> info(
      std::basic_ostream<Char, Traits>& os, const detail::function& fun) {
    return proxy<INFO, Char, Traits>(os, fun);
  }

  /**
   * Returns a logger with log level set to INFO.
   *
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @param src Source location.
   * @return Logger.
   */
  template<class Char, class Traits>
  proxy<INFO, Char, Traits> info(
      std::basic_ostream<Char, Traits>& os, const detail::source& src) {
    return proxy<INFO, Char, Traits>(os, src);
  }

  /**
   * Returns a logger with log level set to DEBUG.
   *
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @return Logger.
   */
  template<class Char, class Traits>
  proxy<DEBUG, Char, Traits> debug(std::basic_ostream<Char, Traits>& os) {
    return proxy<DEBUG, Char, Traits>(os);
  }

  /**
   * Returns a logger with log level set to DEBUG.
   *
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @param fun Source function.
   * @return Logger.
   */
  template<class Char, class Traits>
  proxy<DEBUG, Char, Traits> debug(
      std::basic_ostream<Char, Traits>& os, const detail::function& fun) {
    return proxy<DEBUG, Char, Traits>(os, fun);
  }

  /**
   * Returns a logger with log level set to DEBUG.
   *
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @param src Source location.
   * @return Logger.
   */
  template<class Char, class Traits>
  proxy<DEBUG, Char, Traits> debug(
      std::basic_ostream<Char, Traits>& os, const detail::source& src) {
    return proxy<DEBUG, Char, Traits>(os, src);
  }

  /**
   * Returns a logger with log level set to TRACE.
   *
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @param src Source location.
   * @return Logger.
   */
  template<class Char, class Traits>
  proxy<TRACE, Char, Traits> trace(std::basic_ostream<Char, Traits>& os) {
    return proxy<TRACE, Char, Traits>(os);
  }

  /**
   * Returns a logger with log level set to TRACE.
   *
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @param fun Source function.
   * @return Logger.
   */
  template<class Char, class Traits>
  proxy<TRACE, Char, Traits> trace(
      std::basic_ostream<Char, Traits>& os, const detail::function& fun) {
    return proxy<TRACE, Char, Traits>(os, fun);
  }

  /**
   * Returns a logger with log level set to TRACE.
   *
   * @tparam Char Character type.
   * @tparam Traits Character traits.
   * 
   * @param os Underlaying log stream.
   * @param src Source location.
   * @return Logger.
   */
  template<class Char, class Traits>
  proxy<TRACE, Char, Traits> trace(
      std::basic_ostream<Char, Traits>& os, const detail::source& src) {
    return proxy<TRACE, Char, Traits>(os, src);
  }
}

/**
 * Writes value to underlaying log stream.
 *
 * @tparam Char Character type.
 * @tparam Traits Character traits.
 *
 * @param p Proxy.
 * @param v Value.
 * @return Proxy.
 */
template<unsigned Level, class Char, class Traits, class Value>
const logg::detail::proxy<Level, Char, Traits, true>& operator<<(
    const logg::detail::proxy<Level, Char, Traits, true>& p, const Value& v) {
  p.os << v;
  return p;
}

/**
 * Discards value.
 *
 * @tparam Char Character type.
 * @tparam Traits Character traits.
 *
 * @param p Proxy.
 * @param v Value
 * @return Proxy.
 */
template<unsigned Level, class Char, class Traits, class Value>
const logg::detail::proxy<Level, Char, Traits, false>& operator<<(
    const logg::detail::proxy<Level, Char, Traits, false>& p,
    const Value& v) noexcept {
  return p;
}
