#include <stdio.h>
#include <time.h>
#include <winbase.h>

#include "logg/header.h"

using namespace logg::detail;

namespace {
  unsigned thread_id() {
    return GetCurrentThreadId();
  }
}

unsigned logg::detail::build_header(char* buf, unsigned size) {
  tm tmp;
  auto now = time(nullptr);

  // Write log timestamp to row header.
  auto off = strftime(buf, size, "%F %T", localtime_s(&now, &tmp));

  // As long as the timestamp did not fill the entire buffer we append the
  // thread id to the row header.
  if (off < size) {
    return off +  snprintf(buf + off, size - off, " [%u]", thread_id()) + 1;
  }

  return size;
}
