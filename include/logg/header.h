#pragma once

namespace logg::detail {
  // Fills the specified buffer with the TT part of a TTCC log message.
  unsigned build_header(char* buf, unsigned size);
}
