#ifndef _PRL2_PARALLEL_H
#define _PRL2_PARALLEL_H

#include <algorithm>
#include <functional>
#include <future>
#include <thread>

#include "ThreadPool/ThreadPool.h"

namespace Prl2 {

inline void parallelFor1D(const std::function<void(int)>& job, int chunkSize,
                          int n) {
  const int num_threads = std::max(1U, std::thread::hardware_concurrency());

  ThreadPool pool(num_threads);
  std::vector<std::future<void>> results;

  const int nChunks = n / chunkSize;
  for (int chunk_id = 0; chunk_id < nChunks; ++chunk_id) {
    results.push_back(pool.enqueue([&] {
      const int start_i = chunk_id * chunkSize;
      const int end_i = (chunk_id + 1) * chunkSize;
      for (int i = start_i; i < end_i; ++i) {
        job(i);
      }
    }));
  }
}

}  // namespace Prl2

#endif