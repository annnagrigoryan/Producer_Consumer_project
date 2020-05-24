#pragma once

#include <queue>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>

class Storage {
public:
  Storage(size_t maxIterCount = 3, size_t threshold = 5);
  Storage(const Storage& other) = delete;
  Storage(Storage&& other) = delete;
  void operator=(const Storage& other) = delete;
  void operator=(Storage&& other) = delete;
  ~Storage() = default;

  void produce(int id);
  void consume();

private:
  int calculateSumAndDestroy();
  bool continueWorking() const;

private:
  const size_t maxIterationCount_;
  const size_t minCountToProcess_;

  std::mutex mutex_;
  std::condition_variable	condVar_;
  std::queue<int> coll_;
  std::atomic<int> iterationCount_;
};