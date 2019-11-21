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
    results.push_back(pool.enqueue([chunk_id, chunkSize, job] {
      const int start_i = chunk_id * chunkSize;
      const int end_i = (chunk_id + 1) * chunkSize;
      for (int i = start_i; i < end_i; ++i) {
        job(i);
      }
    }));
  }

  for (auto&& result : results) {
    result.get();
  }
}

inline void parallelFor2D(const std::function<void(int, int)>& job,
                          int chunkSize_x, int chunkSize_y, int nx, int ny) {
  const int num_threads = std::max(1U, std::thread::hardware_concurrency());

  ThreadPool pool(num_threads);
  std::vector<std::future<void>> results;

  const int nChunks_x = nx / chunkSize_x;
  const int nChunks_y = ny / chunkSize_y;
  for (int chunk_x = 0; chunk_x < nChunks_x; ++chunk_x) {
    for (int chunk_y = 0; chunk_y < nChunks_y; ++chunk_y) {
      results.push_back(
          pool.enqueue([chunk_x, chunk_y, chunkSize_x, chunkSize_y, job] {
            const int start_x = chunk_x * chunkSize_x;
            const int end_x = (chunk_x + 1) * chunkSize_x;
            const int start_y = chunk_y * chunkSize_y;
            const int end_y = (chunk_y + 1) * chunkSize_y;
            for (int x = start_x; x < end_x; ++x) {
              for (int y = start_y; y < end_y; ++y) {
                job(x, y);
              }
            }
          }));
    }
  }
}

}  // namespace Prl2

#endif