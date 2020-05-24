#include "storage.h"

int main()
{
  constexpr size_t maxIterationCount{ 3 };
  constexpr size_t minStorageSizeToProcess{ 8 };

  Storage st(maxIterationCount, minStorageSizeToProcess);

  std::thread p1(&Storage::produce, &st, 1);
  std::thread p2(&Storage::produce, &st, 2);
  std::thread p3(&Storage::produce, &st, 3);
  std::thread p4(&Storage::produce, &st, 4);

  std::thread c(&Storage::consume, &st);

  p1.join();
  p2.join();
  p3.join();
  p4.join();
  c.join();

  system("pause");
  return 0;
}