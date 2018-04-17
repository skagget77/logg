#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "logg/header.h"

#if defined(__linux__)
#include <sys/syscall.h>
#include <sys/types.h>
namespace {
  unsigned thread_id() {
    return syscall(SYS_gettid);
  }
}
#else
#error Unsupported POSIX system
#endif

using namespace logg::detail;

const char fatal[] = "FATAL";

unsigned logg::detail::build_header(char* buf, unsigned size) {
  tm tmp;
  auto now = time(nullptr);

  // Write log timestamp to row header.
  auto off = strftime(buf, size, "%F %T", localtime_r(&now, &tmp));

  // As long as the timestamp did not fill the entire buffer we append the
  // thread id to the row header.
  if (off < size) {
    return off +  snprintf(buf + off, size - off, " [%u]", thread_id());
  }

  return size;
}
