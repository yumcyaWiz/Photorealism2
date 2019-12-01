#ifndef _PRL2_PARALLEL_H
#define _PRL2_PARALLEL_H

#include <functional>

#include "ThreadPool.h"

namespace Prl2 {

class Parallel {
 public:
  Parallel();

  // For文を並列実行する
  // job: 並列化する対象の関数
  // nChunks: ループ分割数
  // n: ループ数
  void parallelFor1D(const std::function<void(int)>& job, int nChunks, int n);

  // 二重For文を並列実行する
  // job: 並列化する対象の関数
  // nChunks_x: X方向のループ分割数
  // nChunks_y: Y方向のループ分割数
  // nx: X方向のループ数
  // ny: Y方向のループ数
  void parallelFor2D(const std::function<void(int, int)>& job, int nChunks_x,
                     int nChunks_y, int nx, int ny);

 private:
  ThreadPool pool;
};

}  // namespace Prl2

#endif