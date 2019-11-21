#ifndef _PRL2_PARALLEL_H
#define _PRL2_PARALLEL_H

#include <algorithm>
#include <functional>
#include <thread>

#include "ThreadPool/ThreadPool.h"

namespace Prl2 {

template <class R, class... ArgTypes>
void parallelFor1D(const std::function<R(ArgTypes...)>& job) {
  const int num_threads = std::max(1U, std::thread::hardware_concurrency());

  ThreadPool pool(num_threads);
}  // namespace Prl2

}  // namespace Prl2

#endif