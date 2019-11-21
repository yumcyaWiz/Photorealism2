#ifndef _PRL2_PARALLEL_H
#define _PRL2_PARALLEL_H

#include <algorithm>
#include <thread>

#include "ThreadPool/ThreadPool.h"

namespace Prl2 {

void parallelFor1D() {
  const int num_threads = std::max(1U, std::thread::hardware_concurrency());

  ThreadPool pool(num_threads);
}

}  // namespace Prl2

#endif