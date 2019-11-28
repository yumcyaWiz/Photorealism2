#ifndef _PRL2_PARALLEL_H
#define _PRL2_PARALLEL_H

#include <algorithm>
#include <functional>
#include <future>
#include <thread>

#include "ThreadPool.h"

namespace Prl2 {

// For文を並列実行する
// job: 並列化する対象の関数
// nChunks: ループ分割数
// n: ループ数
inline void parallelFor1D(const std::function<void(int)>& job, int nChunks,
                          int n) {
  const int num_threads = std::max(1U, std::thread::hardware_concurrency());

  ThreadPool pool(num_threads);
  std::vector<std::future<void>> results;

  const int chunkSize = n / nChunks;
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

// 二重For文を並列実行する
// job: 並列化する対象の関数
// nChunks_x: X方向のループ分割数
// nChunks_y: Y方向のループ分割数
// nx: X方向のループ数
// ny: Y方向のループ数
inline void parallelFor2D(const std::function<void(int, int)>& job,
                          int nChunks_x, int nChunks_y, int nx, int ny) {
  const int num_threads = std::max(1U, std::thread::hardware_concurrency());

  ThreadPool pool(num_threads);
  std::vector<std::future<void>> results;

  const int chunkSize_x = nx / nChunks_x;
  const int chunkSize_y = ny / nChunks_y;
  for (int chunk_y = 0; chunk_y < nChunks_y; ++chunk_y) {
    for (int chunk_x = 0; chunk_x < nChunks_x; ++chunk_x) {
      results.push_back(
          pool.enqueue([chunk_x, chunk_y, chunkSize_x, chunkSize_y, job] {
            const int start_x = chunk_x * chunkSize_x;
            const int end_x = (chunk_x + 1) * chunkSize_x;
            const int start_y = chunk_y * chunkSize_y;
            const int end_y = (chunk_y + 1) * chunkSize_y;
            for (int y = start_y; y < end_y; ++y) {
              for (int x = start_x; x < end_x; ++x) {
                job(x, y);
              }
            }
          }));
    }
  }

  for (auto&& result : results) {
    result.get();
  }
}

}  // namespace Prl2

#endif