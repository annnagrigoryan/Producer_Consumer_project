#include "storage.h"
#include <iostream>

Storage::Storage(size_t maxIterCount, size_t minStorageSizeToProcess)
  : maxIterationCount_(maxIterCount)
  , minCountToProcess_(minStorageSizeToProcess)
  , iterationCount_(0)
{
  srand(time(NULL)); // for random number generation
}

void Storage::produce(int id)
{
  while (continueWorking()) {
    std::this_thread::sleep_for(std::chrono::seconds(2)); // for debugging purposes

    const int value = rand() % 10 + id;                   // a random number in [0, 9] + id of the producer
    std::lock_guard<std::mutex> lg(mutex_);               // lock the mutex to work with shared data

    if (!continueWorking())                               // at this point iterationCount_ could be reached to allowed max
      break;

    coll_.push(value);
    std::cout << "producer " << id << ": inserted " << value << std::endl;
    condVar_.notify_one();                               // unlock one of the waiting threads
  }
}

void Storage::consume()
{
  while (continueWorking()) {
    std::this_thread::sleep_for(std::chrono::seconds(2)); // for debugging purposes

    std::unique_lock<std::mutex> ul(mutex_);              // lock the mutex to work with shared data
    std::cout << "consumer is notified; storage size: " << coll_.size() << std::endl;
    condVar_.wait(ul, [&] { return coll_.size() >= minCountToProcess_; }); // will wait until the predicate condition is satisfied

    const size_t collSize = coll_.size();
    const int sum = calculateSumAndDestroy();
    ++iterationCount_;
    std::cout << "*******    consumer: size = " << collSize << "  sum = " << sum << "    *******" << std::endl;
  }
}

int Storage::calculateSumAndDestroy()
{
  int s = 0;
  while (!coll_.empty()) {
    s += coll_.front();
    coll_.pop();
  }
  return s;
}

bool Storage::continueWorking() const
{
  return iterationCount_ < maxIterationCount_;
}